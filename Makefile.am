## Process this file with automake to produce Makefile.in
## A Makefile.in is supplied, in case you do not have automake.

## Copyright (C) 1997-2003 by Henry Kilmer, Erik Sherk and Pete Whiting, Andrew
## Partan, and John Heasley.
## All rights reserved.
##
## This software may be freely copied, modified and redistributed without
## fee for non-commerical purposes provided that this copyright notice is
## preserved intact on all copies and modified copies.
##
## There is no warranty or other guarantee of fitness of this software.
## It is provided solely "as is". The author(s) disclaim(s) all
## responsibility and liability with respect to this software's usage
## or its effect upon hardware, computer systems, other software, or
## anything else.

AUTOMAKE_OPTIONS=foreign no-dependencies

@SET_MAKE@

EXTRA_DIST = BUGS CHANGES COPYING FAQ README Todo cloginrc.sample configure \
	install-sh \
	mkinstalldirs Makefile.in Todo
#DIST_COMMON = 

# '.' is here (and at the beginnging of the macro) so that distclean-recursive
# will run make distclean in . after the other dirs (preserving Makefile) and
# mkinstalldirs will create the install destination before descending into
# the subdirs.
SUBDIRS = . include etc bin man util

# install-exec-local:
# 	$(mkinstalldirs) $(prefix)/util/lg

# sysconfdir is here because etc/Makefile uses a script to be careful about
# installing rancid.conf, not over-writing an existing one.
install-data-hook:
	$(mkinstalldirs) $(prefix) $(sysconfdir); \
	for file in CHANGES FAQ README cloginrc.sample ; do \
	    $(INSTALL_DATA) $$file $(prefix); \
	done

