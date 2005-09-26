/*
 * $Id$
 *
 * Copyright (C) 1997-2004 by Terrapin Communications, Inc.
 * All rights reserved.
 *
 * This software may be freely copied, modified and redistributed
 * without fee for non-commerical purposes provided that this license
 * remains intact and unmodified with any RANCID distribution.
 *
 * There is no warranty or other guarantee of fitness of this software.
 * It is provided solely "as is".  The author(s) disclaim(s) all
 * responsibility and liability with respect to this software's usage
 * or its effect upon hardware, computer systems, other software, or
 * anything else.
 *
 * Except where noted otherwise, rancid was written by and is maintained by
 * Henry Kilmer, John Heasley, Andrew Partan, Pete Whiting, and Austin Schutz.
 *
 * Run telnet or ssh to connect to device specified on the command line.  The
 * point of hpfilter is to filter all the bloody vt100 (curses) escape codes
 * that the HP procurve switches belch out, which are a real bitch to handle
 * in hlogin.
 */

#define DFLT_TO	60				/* default timeout */

#include "config.h"
#include "version.h"

#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <poll.h>
#include <regex.h>
#include <signal.h>
#include <sys/time.h>
#include <termios.h>

#define	BUFSZ	(LINE_MAX * 2)

char		*progname;
int		debug = 0;

int		filter __P((char *, int));
void		usage __P((void));
void		vers __P((void));
RETSIGTYPE	reapchild __P((void));

int
main(int argc, char **argv)
{
    extern char		*optarg;
    extern int		optind;
    char		ch,
			hbuf[BUFSZ],		/* hlogin buffer */
			*hbufp,
			tbuf[BUFSZ],		/* telnet/ssh buffer */
			*tbufp;
    int			bytes,			/* bytes read/written */
			child,
			rval = EX_OK,
			r[2],			/* recv pipe */
			s[2];			/* send pipe */
    ssize_t		hlen = 0,		/* len of hbuf */
			tlen = 0;		/* len of tbuf */
    struct pollfd	pfds[4];
    struct termios	tios;

    /* get just the basename() of our exec() name and strip a .* off the end */
    if ((progname = strrchr(argv[0], '/')) != NULL)
	progname += 1;
    else
	progname = argv[0];
    if (strrchr(progname, '.') != NULL)
	*(strrchr(progname, '.')) = '\0';

    while ((ch = getopt(argc, argv, "dhv")) != -1 )
	switch (ch) {
	case 'd':
	    debug++;
	    break;
	case 'v':
	    vers();
	    return(EX_OK);
	case 'h':
	default:
	    usage();
	    return(EX_USAGE);
	}

    if (argc - optind < 2) {
	usage();
	return(EX_USAGE);
    }

    /* reap our children */
    signal(SIGCHLD, (void *) reapchild);
    signal(SIGHUP, (void *) reapchild);
    signal(SIGINT, (void *) reapchild);
    signal(SIGTERM, (void *) reapchild);

    /* create 2 pipes for send/recv and then fork and exec telnet/ssh */
    for (child = 3; child < 10; child++)
	close(child);
    if (pipe(s) || pipe(r)) {
	fprintf(stderr, "%s: pipe() failed: %s\n", progname,
		strerror(errno));
	return(EX_TEMPFAIL);
    }

    /* if a tty, make it raw as the hp echos _everything_, including
     * passwords.
     */
    if (isatty(fileno(stdin))) {
	if (tcgetattr(fileno(stdin), &tios)) {
	    fprintf(stderr, "%s: tcgetattr() failed: %s\n", progname,
		strerror(errno));
	    return(EX_OSERR);
	}
	tios.c_lflag &= ~ECHO;
	tios.c_lflag &= ~ICANON;
#ifdef VMIN
	tios.c_cc[VMIN] = 1;
	tios.c_cc[VTIME] = 0;
#endif
	if (tcsetattr(fileno(stdin), TCSANOW, &tios)) {
	    fprintf(stderr, "%s: tcsetattr() failed: %s\n", progname,
		strerror(errno));
	    return(EX_OSERR);
	}
    }

    /* zero the buffers */
    memset(hbuf, 0, BUFSZ);
    memset(tbuf, 0, BUFSZ);

    if ((child = fork()) == -1) {
	fprintf(stderr, "%s: fork() failed: %s\n", progname,
		strerror(errno));
	return(EX_TEMPFAIL);
    }

    if (child == 0) {
	/* close the parent's side of the pipes; we write r[1], read s[0] */
	close(s[1]);
	close(r[0]);
	/* close stdin/out/err and attach them to the pipes */
	if (dup2(s[0], 0) == -1 || dup2(r[1], 1) == -1 || dup2(r[1], 2) == -1) {
	    fprintf(stderr, "%s: dup2() failed: %s\n", progname,
		    strerror(errno));
	    return(EX_OSERR);
	}
	close(s[0]);
	close(r[1]);

	/* exec telnet/ssh */
	execvp(argv[optind], argv + optind);
	fprintf(stderr, "%s: execvp() failed: %s\n", progname, strerror(errno));
	return(EX_TEMPFAIL);
	/*NOTREACHED*/
    } else {
	/* parent */
	if (debug)
	    fprintf(stderr, "child %d\n", child);

	/* close the child's side of the pipes; we write s[1], read r[0] */
	close(s[0]);
	close(r[1]);

	/* make FDs non-blocking */
	if (fcntl(s[1], F_SETFL, O_NONBLOCK) ||
	    fcntl(r[0], F_SETFL, O_NONBLOCK) ||
	    fcntl(0, F_SETFL, O_NONBLOCK) ||
	    fcntl(1, F_SETFL, O_NONBLOCK)) {
	    fprintf(stderr, "%s: fcntl(NONBLOCK) failed: %s\n", progname,
		strerror(errno));
	    exit(EX_OSERR);
	}

	/* loop to read on stdin and r[0] */
	hbufp = hbuf; tbufp = tbuf;

#define	POLLEXP	(POLLERR | POLLHUP | POLLNVAL)
	pfds[0].fd = fileno(stdin);
	pfds[0].events = POLLIN | POLLEXP;
	pfds[1].fd = fileno(stdout);
	pfds[2].fd = r[0];
	pfds[2].events = POLLIN | POLLEXP;
	pfds[3].fd = s[1];

	while (1) {
	    /* if we have stuff in our buffer(s), we select on writes too */
	    if (hlen)
	        pfds[3].events = POLLOUT | POLLEXP;
	    else
	        pfds[3].events = POLLEXP;

	    if (tlen)
	        pfds[1].events = POLLOUT | POLLEXP;
	    else
	        pfds[1].events = POLLEXP;

	    bytes = poll(pfds, 4, (DFLT_TO * 1000));
	    if (bytes == 0)
		/* timeout */
		continue;
	    if (bytes == -1) {
		switch (errno) {
		case EINTR:
		    break;
		default:
		    rval = EX_IOERR;
		    break;
		}
		continue;
	    }

	    /* write buffers first */
	    /* write hbuf (stdin) -> s[1] */
	    if ((pfds[3].revents & POLLOUT) && hlen) {
		if ((bytes = write(s[1], hbuf, hlen)) < 0) {
		    fprintf(stderr, "%s: write() failed: %s\n", progname,
			    strerror(errno));
		    close(pfds[3].fd);
		} else if (bytes > 0) {
		    strcpy(hbuf, hbuf + bytes);
		    hlen -= bytes;
		}
	    } else if (pfds[3].revents & POLLEXP)
		break;

	    /* write tbuf -> stdout */
	    if ((pfds[1].revents & POLLOUT) && tlen) {
		/* if there is an escape char that didnt get filter()'d,
		 * we need to write only up to that point and wait for
		 * the bits that complete the escape sequence.  if at least
		 * two bytes follow it, write it anyway as filter() didnt
		 * match it.
		 */
		bytes = tlen;
		if ((tbufp = index(tbuf, 0x1b)) != NULL)
		    if (tlen - (tbufp - tbuf) < 2)
			bytes = tbufp - tbuf;

		if ((bytes = write(pfds[1].fd, tbuf, bytes)) < 0) {
		    fprintf(stderr, "%s: write() failed: %s\n", progname,
			    strerror(errno));
		    close(pfds[1].fd);
		} else if (bytes > 0) {
		    strcpy(tbuf, tbuf + bytes);
		    tlen -= bytes;
		}
	    } else if (pfds[1].revents & POLLEXP)
		break;

	    if (pfds[0].revents & POLLIN) {
		/* read stdin into hbuf */
		if (BUFSZ - hlen > 1) {
		    bytes = read(pfds[0].fd, hbuf + hlen, (BUFSZ - 1) - hlen);
		    if (bytes > 0) {
			hlen += bytes;
			hbuf[hlen] = '\0';
		    } else if (bytes == 0 && errno != EAGAIN) {
			/* EOF or read error */
			pfds[0].events = 0;
			close(pfds[0].fd);
		    }
		}
	    }
	    if (pfds[2].revents & POLLIN) {
		/* read telnet/ssh into tbuf, then filter */
		if (BUFSZ - tlen > 1) {
		    bytes = read(pfds[2].fd, tbuf + tlen, (BUFSZ - 1) - tlen);
		    if (bytes > 0) {
			tbuf[tlen + bytes] = '\0';
			tlen = filter(tbuf, tlen + bytes);
		    } else if (bytes == 0 && errno != EAGAIN) {
			/* EOF or read error */
			pfds[2].events = 0;
			close(pfds[2].fd);
		    }
		}
	    }
	}

	close(0);
	close(1);
	close(s[1]);
	close(r[0]);
    }

    if (! kill(child, SIGINT))
	reapchild();

    return(rval);
}

int
filter(buf, len)
    char	*buf;
    int		len;
{
    static regmatch_t	pmatch[1];
#define	N_REG		13		/* number of regexes in reg[][] */
    static regex_t	preg[N_REG];
    static char		reg[N_REG][50] = {	/* vt100/220 escape codes */
				"\e7\e\\[1;24r\e8",		/* ds */
				"\e8",				/* fs */

				"\e\\[2J",
				"\e\\[2K",			/* kE */

				"\e\\[[0-9]+;[0-9]+r",		/* cs */
				"\e\\[[0-9]+;[0-9]+H",		/* cm */

				"\e\\[\\?6l",
				"\e\\[\\?7l",			/* RA */
				"\e\\[\\?25h",			/* ve */
				"\e\\[\\?25l",			/* vi */
				"\e\\[K",			/* ce */

				"\e\\[0m",			/* me */
				"\eE",			/* replace w/ CR */
			};
    char		ebuf[256];
    size_t		nmatch = 1;
    int			err,
			x;
    static int		init = 0;

    if (index(buf, 0x1b) == 0 || len == 0)
	return(len);

    for (x = 0; x < N_REG - 2; x++) {
	if (! init) {
	    if ((err = regcomp(&preg[x], reg[x], REG_EXTENDED))) {
		regerror(err, &preg[x], ebuf, 256);
		fprintf(stderr, "%s: regex compile failed: %s\n", progname,
			ebuf);
		abort();
	    }
	}
	if ((err = regexec(&preg[x], buf, nmatch, pmatch, 0))) {
	    if (err != REG_NOMATCH) {
		regerror(err, &preg[x], ebuf, 256);
		fprintf(stderr, "%s: regexec failed: %s\n", progname, ebuf);
		abort();
	    }
	} else {
	    strcpy(buf + pmatch[0].rm_so, buf + pmatch[0].rm_eo);
	    x = 0;
	}
    }

    /* replace \eE w/ CR NL */
    if (! init++) {
	for (x = N_REG - 2; x < N_REG; x++)
	    if ((err = regcomp(&preg[x], reg[x], REG_EXTENDED))) {
		regerror(err, &preg[x], ebuf, 256);
		fprintf(stderr, "%s: regex compile failed: %s\n", progname,
			ebuf);
		abort();
	    }
    }
    for (x = N_REG - 2; x < N_REG; x++) {
	if ((err = regexec(&preg[x], buf, nmatch, pmatch, 0))) {
	    if (err != REG_NOMATCH) {
		regerror(err, &preg[x], ebuf, 256);
		fprintf(stderr, "%s: regexec failed: %s\n", progname, ebuf);
		abort();
	    }
	} else {
	    *(buf + pmatch[0].rm_so) = '\r';
	    *(buf + pmatch[0].rm_so + 1) = '\n';
	    strcpy(buf + pmatch[0].rm_so + 2, buf + pmatch[0].rm_eo);
	    x = N_REG - 2;
	}
    }
    return(strlen(buf));
}

RETSIGTYPE
reapchild(void)
{
    int         status;
    pid_t       pid;

    /* XXX this needs to deal with/without wait3 via HAVE_WAIT3 */
    while ((pid = wait3(&status, WNOHANG, 0)) > 0)
	if (debug)
            fprintf(stderr, "reap child %d\n", pid);

    /*exit(1);*/
return;

    /* not reached */
}

void
usage(void)
{
    fprintf(stderr,
"usage: %s [-hv] <telnet|ssh> [<ssh options>] <hostname> [<telnet_port>]\n",
	progname);
    return;
}

void
vers(void)
{
    fprintf(stderr, "%s: %s version %s\n", progname, package, version);
    return;
}
