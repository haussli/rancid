## Process this file with automake to produce Makefile.in
## A Makefile.in is supplied, in case you do not have automake.

## Copyright (C) 1997-2001 by Henry Kilmer, Erik Sherk and Pete Whiting.
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

#AUTOMAKE_OPTIONS=foreign no-dependencies
AUTOMAKE_OPTIONS=foreign

# PREFIX = @prefix@

bindir=${prefix}/util/lg

bin_SCRIPTS = lg.cgi lgform.cgi

#CPPFLAGS += @PG_CPPFLAGS@
#INCLUDES += -I$(top_srcdir)/include @PG_CPPFLAGS@
#INCLUDES += -I$(top_srcdir)/include

#CFLAGS += -g
CFLAGS = -g -O0

YFLAGS = -d
#LFLAGS = -i

EXTRA_DIST = README index.html lgnotes.html

# no idea why automake doesnt clean these targets
#CLEANFILES= y.tab.c y.tab.h lex.yy.c conf.h conf.c conflex.c

all:

install-data-local: all
	for file in *README* *.html; do \
	    $(INSTALL_DATA) $$file $(bindir); \
	done; \
	if test -f $(bindir)/lg.conf ; then \
	    echo ""; \
	    echo "WARNING: *** $(bindir)/lg.conf exists: installing as lg.conf.new."; \
	    echo "         *** review lg.conf.new for new/deprecated switches"; \
	    echo ""; \
	    $(INSTALL_DATA) lg.conf $(bindir)/lg.conf.new; \
	else \
	    $(INSTALL_DATA) lg.conf $(bindir); \
	fi
