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

pkgdata_DATA= lg.conf.sample rancid.conf.sample
dist_pkgdata_DATA= $(pkgdata_DATA)

install-data-local: all
	@if test -f $(sysconfdir)/rancid.conf ; then \
	    echo ""; \
	    echo "WARNING: *** $(sysconfdir)/rancid.conf exists.  See "; \
	    echo "         *** $(pkgdatadir)/rancid.conf.sample for new"; \
	    echo "         *** examples."; \
	    echo ""; \
	else \
	    $(INSTALL_DATA) rancid.conf.sample $(sysconfdir)/rancid.conf; \
	fi; \
	if test -f $(sysconfdir)/lg.conf ; then \
	    echo ""; \
	    echo "WARNING: *** $(sysconfdir)/lg.conf exists.  See "; \
	    echo "         *** $(pkgdatadir)/lg.conf.sample for new"; \
	    echo "         *** examples."; \
	    echo ""; \
	else \
	    $(INSTALL_DATA) lg.conf.sample $(sysconfdir)/lg.conf; \
	fi;

#clean:
#	rm -f Makefile rancid.conf $(BIN_DATAS) $(BIN_PROGS)

# auto_edit does the autoconf variable substitution.  This allows the
# substitution to have the full expansion of the variables, eg: $sysconfdir
# will be /prefix/etc instead of ${prefix}/etc.
#
# This is a bit of a PITA, but is the method recommended by the autoconf
# documentation.
auto_edit = sed \
	-e 's,@prefix\@,$(prefix),g' \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@EXPECT_PATH\@,$(EXPECT_PATH),g' \
	-e 's,@PERLV_PATH\@,$(PERLV_PATH),g' \
	-e 's,@ENV_PATH\@,$(ENV_PATH),g'

lg.conf.sample: Makefile $(srcdir)/lg.conf.sample.in
	rm -f lg.conf.sample lg.conf.sample.tmp; \
	$(auto_edit) $(srcdir)/lg.conf.sample.in >lg.conf.sample.tmp; \
	mv lg.conf.sample.tmp lg.conf.sample

rancid.conf.sample: Makefile $(srcdir)/rancid.conf.sample.in
	rm -f rancid.conf.sample rancid.conf.sample.tmp; \
	$(auto_edit) $(srcdir)/rancid.conf.sample.in >rancid.conf.sample.tmp; \
	mv rancid.conf.sample.tmp rancid.conf.sample
