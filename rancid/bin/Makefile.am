## Process this file with automake to produce Makefile.in
## A Makefile.in is supplied, in case you do not have automake.

## Copyright (C) 1997-2004 by Henry Kilmer, Pete Whiting, Andrew Partan,
## and John Heasley.
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
##
## The login expect scripts were based on Erik Sherk's gwtn, by permission.

#AUTOMAKE_OPTIONS=foreign no-dependencies
AUTOMAKE_OPTIONS=foreign

bin_PROGRAMS = hpuifilter
bin_SCRIPTS = cat5rancid control_rancid \
	alogin arancid clogin create_cvs blogin brancid do-diffs \
	elogin erancid f10rancid flogin francid jlogin jrancid jerancid \
	hlogin hrancid htlogin htrancid \
	mrancid nlogin nrancid nslogin nsrancid par prancid \
	rancid-fe rancid rename rivlogin rivrancid rrancid tntlogin \
	tntrancid xrancid zrancid

hpuifilter_SOURCES = hpuifilter.c

#CPPFLAGS += @PG_CPPFLAGS@
#INCLUDES += -I$(top_srcdir)/include @PG_CPPFLAGS@
#INCLUDES += -I$(top_srcdir)/include

#CFLAGS += -g
CFLAGS = -g -O0

YFLAGS = -d
#LFLAGS = -i

# no idea why automake doesnt clean these targets
#CLEANFILES= y.tab.c y.tab.h lex.yy.c conf.h conf.c conflex.c

#clean:
#	rm -f Makefile env $(BIN_DATAS) $(BIN_PROGS)
