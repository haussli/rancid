## Process this file with automake to produce Makefile.in
## A Makefile.in is supplied, in case you do not have automake.

## $Id: Makefile.am 2481 2012-05-24 17:39:33Z heas $
##
## Copyright (c) 1997-2012 by Terrapin Communications, Inc.
## All rights reserved.
##
## This code is derived from software contributed to and maintained by
## Terrapin Communications, Inc. by Henry Kilmer, John Heasley, Andrew Partan,
## Pete Whiting, Austin Schutz, and Andrew Fort.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. All advertising materials mentioning features or use of this software
##    must display the following acknowledgement:
##        This product includes software developed by Terrapin Communications,
##        Inc. and its contributors for RANCID.
## 4. Neither the name of Terrapin Communications, Inc. nor the names of its
##    contributors may be used to endorse or promote products derived from
##    this software without specific prior written permission.
## 5. It is requested that non-binding fixes and modifications be contributed
##    back to Terrapin Communications, Inc.
##
## THIS SOFTWARE IS PROVIDED BY Terrapin Communications, INC. AND CONTRIBUTORS
## ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
## TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
## PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COMPANY OR CONTRIBUTORS
## BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
## CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
# 
#  The expect login scripts were based on Erik Sherk's gwtn, by permission.
# 
#  The original looking glass software was written by Ed Kern, provided by
#  permission and modified beyond recognition.

AUTOMAKE_OPTIONS=foreign no-dependencies

pkglib_LIBRARIES = base.pm ios.pm junos.pm
RANLIB= :

#EXTRA_DIST= lg.cgi.in lgform.cgi.in rancid-cvs.in rancid-fe.in rancid-run.in
##dist_bin_SCRIPTS= $(bin_SCRIPTS:%=%.in)

CLEANFILES= rancid
CLEANFILES+= $(pkglib_LIBRARIES)

#clean:
#	rm -f Makefile env $(BIN_DATAS) $(BIN_PROGS)

# auto_edit does the autoconf variable substitution.  This allows the
# substitution to have the full expansion of the variables, e.g.: $sysconfdir
# will be /prefix/etc instead of ${prefix}/etc.
#
# This is a bit of a PITA, but is the method recommended by the autoconf
# documentation.
auto_edit = sed \
	-e 's,@prefix\@,$(prefix),g' \
	-e 's,@localstatedir\@,$(localstatedir),g' \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@EXPECT_PATH\@,$(EXPECT_PATH),g' \
	-e 's,@PERLV\@,$(PERLV),g' \
	-e 's,@PERLV_PATH\@,$(PERLV_PATH),g' \
	-e 's,@LG_PING_CMD\@,$(LG_PING_CMD),g' \
	-e 's,@ADMINMAILPLUS\@,$(ADMINMAILPLUS),g' \
	-e 's,@MAILPLUS\@,$(MAILPLUS),g' \
	-e 's,@PACKAGE\@,$(PACKAGE),g' \
	-e 's,@SVN_FSTYPE\@,$(SVN_FSTYPE),g' \
	-e 's,@VERSION\@,$(VERSION),g' \
	-e 's,@COPYYEARS\@,$(COPYYEARS),g'

base.pm: Makefile $(srcdir)/base.pm.in
	rm -f base.pm base.pm.tmp; \
	$(auto_edit) $(srcdir)/base.pm.in >base.pm.tmp; \
	chmod +x base.pm.tmp; \
	mv base.pm.tmp base.pm

ios.pm: Makefile $(srcdir)/ios.pm.in
	rm -f ios.pm ios.pm.tmp; \
	$(auto_edit) $(srcdir)/ios.pm.in >ios.pm.tmp; \
	chmod +x ios.pm.tmp; \
	mv ios.pm.tmp ios.pm

junos.pm: Makefile $(srcdir)/junos.pm.in
	rm -f junos.pm junos.pm.tmp; \
	$(auto_edit) $(srcdir)/junos.pm.in >junos.pm.tmp; \
	chmod +x junos.pm.tmp; \
	mv junos.pm.tmp junos.pm

#$(bin_PROGRAMS) $(bin_SCRIPTS): ../include/version.h
