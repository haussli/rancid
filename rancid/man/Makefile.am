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

man_gen_MANS = rancid.conf.5 lg.conf.5 lg_intro.1
man_nogen_MANS = alogin.1 blogin.1 clogin.1 control_rancid.1 \
		cssrancid.1 rancid.1 \
		rancid_intro.1 cloginrc.5 router.db.5 \
		drancid.1 elogin.1 \
		f10rancid.1 flogin.1 hlogin.1 hrancid.1 htlogin.1 htrancid.1 \
		jlogin.1 \
		jrancid.1 jerancid.1 f10rancid.1 francid.1 cat5rancid.1 \
		erancid.1 mrancid.1 nlogin.1 nrancid.1 nslogin.1 nsrancid.1 \
		par.1 prancid.1 \
		rancid-cvs.1 rancid-run.1 rivlogin.1 rivrancid.1 \
		tntlogin.1 tntrancid.1 xrancid.1 zrancid.1

man_MANS = $(man_nogen_MANS) $(man_gen_MANS)

EXTRA_DIST = $(man_nogen_MANS)

#CLEANFILES = Makefile rancid.conf.5 lg.conf.5 lg_intro.1

all:

distclean: clean
