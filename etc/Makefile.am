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

#AUTOMAKE_OPTIONS=foreign no-dependencies
AUTOMAKE_OPTIONS=foreign

install-data-local: all
	if test -f $(bindir)/env ; then \
	    echo ""; \
	    echo "WARNING: *** Found $(bindir)/env from a previous installation."; \
	    echo "         *** Beginning with RANCID 2.3, the configuration file "; \
	    echo "         *** is now $(sysconfdir)/rancid.conf.  I moved the file for you."; \
	    echo ""; \
	    echo "mv $(bindir)/env $(sysconfdir)/rancid.conf"; \
	    mv $(bindir)/env $(sysconfdir)/rancid.conf; \
	fi; \
	if test -f $(bindir)/env.new ; then \
	    echo "mv $(bindir)/env.new $(sysconfdir)/rancid.conf.new"; \
	    mv $(bindir)/env.new $(sysconfdir)/rancid.conf.new; \
	fi; \
	if test -f $(sysconfdir)/rancid.conf ; then \
	    echo ""; \
	    echo "WARNING: *** $(sysconfdir)/rancid.conf exists: installing as rancid.conf.new."; \
	    echo "         *** review rancid.conf.new for new/deprecated switches"; \
	    echo ""; \
	    $(INSTALL_DATA) rancid.conf $(sysconfdir)/rancid.conf.new; \
	else \
	    $(INSTALL_DATA) rancid.conf $(sysconfdir); \
	fi; \
	if test -f $(sysconfdir)/lg.conf ; then \
	    echo ""; \
	    echo "WARNING: *** $(sysconfdir)/lg.conf exists: installing as lg.conf.new."; \
	    echo "         *** review lg.conf.new for new/deprecated switches"; \
	    echo ""; \
	    $(INSTALL_DATA) lg.conf $(sysconfdir)/lg.conf.new; \
	else \
	    $(INSTALL_DATA) lg.conf $(sysconfdir); \
	fi

#clean:
#	rm -f Makefile rancid.conf $(BIN_DATAS) $(BIN_PROGS)
