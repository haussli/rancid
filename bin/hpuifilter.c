/*
 * run telnet to connect to device specified on the command line.  the
 * point of vtfilter is to filter all the bloody vt100 (curses) escape codes
 * that the HP procurve switches belch.
 *
 * stdin/out should be connected to hlogin.
 */

#define DFLT_TO	60

#define HAVE_UNISTD_H 1
#define HAVE_SYSEXITS_H 1
#define HAVE_SYS_WAIT_H 1
#define RETSIGTYPE void
#define HAVE_STRING_H 1

#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <regex.h>

#include <termios.h>

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if HAVE_STRING_H
# include <string.h>
#endif
#if HAVE_STRINGS_H
# include <strings.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#if HAVE_SYSEXITS_H
# include <sysexits.h>
#else
					/* missing sysexits.h */
# define EX_OK		0
# define EX_USAGE	64		/* command line usage error */
# define EX_NOINPUT	66		/* cannot open input */
# define EX_TEMPFAIL	75		/* temp failure */
# define EX_OSERR	71		/* system error */
# define EX_CANTCREAT	73		/* can't create (user) output file */
# define EX_IOERR	74		/* input/output error */
# define EX_CONFIG	78		/* configuration error */
#endif

char		*progname;
int		debug = 0;

int		filter __P((char *, int));
void		usage __P((void));
RETSIGTYPE	reapchild __P((void));

int
main(int argc, char **argv)
{
    extern char		*optarg;
    extern int		optind;
    char		ch,
			hbuf[LINE_MAX * 2],	/* hlogin buffer */
			*hbufp,
			tbuf[LINE_MAX * 2],	/* telnet buffer */
			*tbufp;
    int			child,
			r[2],			/* recv pipe */
			s[2];			/* send pipe */
    ssize_t		hlen = 0,		/* len of hbuf */
			tlen = 0;		/* len of tbuf */
    struct timeval	to = { DFLT_TO, 0 };
    fd_set		rfds,
			wfds;
    struct termios	tios;

    /* get just the basename() of our exec() name and strip a .* off the end */
    if ((progname = strrchr(argv[0], '/')) != NULL)
	progname += 1;
    else
	progname = argv[0];
    if (strrchr(progname, '.') != NULL)
	*(strrchr(progname, '.')) = '\0';

    while ((ch = getopt(argc, argv, "dh")) != -1 )
	switch (ch) {
	case 'd':
	    debug++;
	case 'h':
	default:
	    usage();
	    return(EX_USAGE);
	}

    if (argc - optind != 2) {
	usage();
	return(EX_USAGE);
    }

    /* reap our children */
    signal(SIGCHLD, (void *) reapchild);
    signal(SIGHUP, (void *) reapchild);
    signal(SIGINT, (void *) reapchild);
    signal(SIGKILL, (void *) reapchild);

    /* create 2 pipes for send/recv and then fork and exec telnet */
    for (child = 3; child < 10; child++)
	close(child);
    if (pipe(s) || pipe(r)) {
	fprintf(stderr, "%s: pipe() failed: %s\n", progname,
		sys_errlist[errno]);
	return(EX_TEMPFAIL);
    }

    /* if a tty, make it raw as the hp echos _everything_ */
    if (isatty(0)) {
	if (tcgetattr(0, &tios)) {
	    fprintf(stderr, "%s: tcgetattr() failed: %s\n", progname,
		sys_errlist[errno]);
	    return(EX_OSERR);
	}
	tios.c_lflag &= ~ECHO;
	tios.c_lflag &= ~ICANON;
	if (tcsetattr(0, TCSANOW, &tios)) {
	    fprintf(stderr, "%s: tcsetattr() failed: %s\n", progname,
		sys_errlist[errno]);
	    return(EX_OSERR);
	}
    }

    if ((child = fork()) == -1) {
	fprintf(stderr, "%s: fork() failed: %s\n", progname,
		sys_errlist[errno]);
	return(EX_TEMPFAIL);
    }

    /* zero the buffers */
    bzero(hbuf, LINE_MAX * 2);
    bzero(tbuf, LINE_MAX * 2);

    if (child == 0) {
	/* close the parent's side of the pipes; we write r[1], read s[0] */
	close(s[1]);
	close(r[0]);
	/* close stdin/out/err and attach them to the pipes */
	if (dup2(s[0], 0) == -1 || dup2(r[1], 1) == -1 || dup2(r[1], 2) == -1) {
	    fprintf(stderr, "%s: dup2() failed: %s\n", progname,
		sys_errlist[errno]);
	    return(EX_OSERR);
	}
	close(s[0]);
	close(r[1]);
	/* exec telnet */
	/*if (execlp("telnet", "telnet", argv[optind], NULL)) {*/
	if (execvp(argv[optind], argv + optind)) {
	    fprintf(stderr, "%s: execlp() failed: %s\n", progname,
		sys_errlist[errno]);
	    return(EX_TEMPFAIL);
	}
	/* not reached */
    } else {
	/* parent */
	if (debug)
	    fprintf(stderr, "child %d\n", child);

	/* close the child's side of the pipes; we write s[1], read r[0] */
	close(s[0]);
	close(r[1]);

	/* make FDs non-blocking */
	fcntl(s[1], F_SETFL, O_NONBLOCK);
	fcntl(r[0], F_SETFL, O_NONBLOCK);
	fcntl(0, F_SETFL, O_NONBLOCK);
	fcntl(1, F_SETFL, O_NONBLOCK);

	/* loop to read on stdin and r[0] */
	FD_ZERO(&rfds); FD_ZERO(&wfds);
	hbufp = hbuf; tbufp = tbuf;

	while (1) {
	    FD_SET(0, &rfds); FD_SET(r[0], &rfds);
	    /* once we have stuff in our buffer(s), we select on writes too */
	    if (hlen)
		 FD_SET(s[1], &wfds);
	    if (tlen)
		 FD_SET(1, &wfds);

	    switch (select(6, &rfds, &wfds, NULL, &to)) {
	    case 0:
		/* timeout */
			/* HEAS: what do i do here? */
		break;
	    case -1:
		switch (errno) {
		case EINTR:		/* interrupted syscall */
		    break;
		default:
		    exit(EX_IOERR);
		}
		break;
	    default:
		/* which FD is ready?  if we have stuff to write, check those
		 * FDs first.
		 */
		/* write hbuf -> s[1] */
		if (FD_ISSET(s[1], &wfds)) {
		    if ((hlen = write(s[1], hbufp, hlen))) {
			hbufp += hlen;
			if (*hbufp == '\0') {
			    *hbuf = '\0';
			    hbufp= hbuf;
			}
		    }
		    hlen = strlen(hbuf);
		}
		/* write tbuf -> stdout */
		if (FD_ISSET(1, &wfds)) {
		    if ((tlen = write(1, tbufp, tlen))) {
		    tbufp += tlen;
		    if (*tbufp == '\0') {
			*tbuf = '\0';
			tbufp= tbuf;
		    }
		    }
		    tlen = strlen(tbuf);
		}
		if (FD_ISSET(0, &rfds)) {
		    /* read stdin.  hbuf has to be "empty" */
		    if (! hlen) {
		    hlen = read(0, hbuf, LINE_MAX * 2 - 1);
		    hbuf[hlen] = '\0';
		    FD_SET(1, &wfds);
		    }
		} else if (FD_ISSET(r[0], &rfds)) {
		    /* read telnet, filter.  tbuf has to be "empty" */
		    if (! tlen) {
		    tlen = read(r[0], tbuf, LINE_MAX * 2 - 1);
		    tbuf[tlen] = '\0';
		    tlen = filter(tbuf, tlen);
		    FD_SET(s[1], &wfds);
		    }
		}

		break;
	    }
	}
    }

    kill(child, SIGQUIT);
    return(EX_OK);
}

#define	N_REG		10		/* number of regexes in reg[][] */

int
filter(buf, len)
    char	*buf;
    int		len;
{
    static regmatch_t	pmatch[1];
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
			};
    char		ebuf[256];
    size_t		nmatch = 1;
    int			err,
			x;
    static int		init = 0;

    if (index(buf, 0x1b) == 0 || len == 0)
	return(len);

    for (x = 0; x < N_REG; x++) {
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

    init++;

    return(strlen(buf));
}

RETSIGTYPE 
reapchild(void)
{
    int         status;
    pid_t       pid;
    
    /* HEAS: this needs to deal with/without wait3 via HAVE_WAIT3 */
    while ((pid = wait3(&status, WNOHANG, 0)) > 0)
	if (debug)
            fprintf(stderr, "reap child %d\n", pid);
    
    exit(1);

    /* not reached */
}   

void
usage(void)
{
    fprintf(stderr,
"usage: %s [-h] <telnet|ssh> <hostname>
", progname);
    return;
}
