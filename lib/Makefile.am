## Process this file with automake to produce Makefile.in
## A Makefile.in is supplied, in case you do not have automake.

AUTOMAKE_OPTIONS=foreign no-dependencies

my_pkglibdir = $(pkglibdir)
my_pkglib_DATA = rancid.pm rancidcontrib.pm \
		 acos.pm aeos.pm arbor.pm arcos.pm avocent.pm \
		 bigip.pm ciscowlc.pm \
		 dell.pm dnos9.pm dnos10.pm edgemax.pm edgerouter.pm exos.pm \
		 fortigate.pm foundry.pm frr.pm fss2.pm fxos.pm \
		 ios.pm iossb.pm iosshtech.pm iosxr.pm junos.pm mrv.pm nxos.pm \
		 panos.pm rbt.pm routeros.pm smc.pm sros.pm vrp.pm wavesvros.pm 
pkglib_in = $(my_pkglib_DATA:%=%.in)

RANLIB= :

EXTRA_DIST= $(pkglib_in)
##dist_bin_SCRIPTS= $(bin_SCRIPTS:%=%.in)

CLEANFILES= rancid
CLEANFILES+= $(my_pkglib_DATA)

#clean:
#	rm -f Makefile env $(BIN_DATAS) $(BIN_PROGS)

# auto_edit does the autoconf variable substitution.  This allows the
# substitution to have the full expansion of the variables, e.g.: $sysconfdir
# will be /prefix/etc instead of ${prefix}/etc.
#
# This is a bit of a PITA, but is the method recommended by the autoconf
# documentation.
auto_edit = sed \
	-e 's,@bindir\@,$(bindir),g' \
	-e 's,@prefix\@,$(prefix),g' \
	-e 's,@localstatedir\@,$(localstatedir),g' \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@EXPECT_PATH\@,$(EXPECT_PATH),g' \
	-e 's,@ENV_PATH\@,$(ENV_PATH),g' \
	-e 's,@PERLV\@,$(PERLV),g' \
	-e 's,@PERLV_PATH\@,$(PERLV_PATH),g' \
	-e 's,@PYTHON3\@,$(PYTHON3),g' \
	-e 's,@PYTHON3_PATH\@,$(PYTHON3_PATH),g' \
	-e 's,@LG_PING_CMD\@,$(LG_PING_CMD),g' \
	-e 's,@ADMINMAILPLUS\@,$(ADMINMAILPLUS),g' \
	-e 's,@MAILPLUS\@,$(MAILPLUS),g' \
	-e 's,@PACKAGE\@,$(PACKAGE),g' \
	-e 's,@RCSSYS\@,$(RCSSYS),g' \
	-e 's,@SENDMAIL\@,$(SENDMAIL),g' \
	-e 's,@SVN_FSTYPE\@,$(SVN_FSTYPE),g' \
	-e 's,@VERSION\@,$(VERSION),g' \
	-e 's,@COPYYEARS\@,$(COPYYEARS),g' \
	-e '/@copyright@/r ../COPYING' \
	-e '/@copyright@/d'

$(pkglib_in:%.in=%): Makefile $(top_builddir)/config.status
	rm -f $@ $@.tmp; \
	$(auto_edit) $(srcdir)/$@.in >$@.tmp; \
	chmod +r $@.tmp; \
	mv $@.tmp $@
# XXX there must be a better way to produce these dependencies, perhaps in
#     configure.
acos.pm: acos.pm.in
aeos.pm: aeos.pm.in
arbor.pm: arbor.pm.in
arcos.pm: arcos.pm.in
avocent.pm: avocent.pm.in
bigip.pm: bigip.pm.in
ciscowlc.pm: ciscowlc.pm.in
dell.pm: dell.pm.in
dnos9.pm: dnos9.pm.in
dnos10.pm: dnos10.pm.in
edgemax.pm: edgemax.pm.in
edgerouter.pm: edgerouter.pm.in
exos.pm: exos.pm.in
fortigate.pm: fortigate.pm.in
foundry.pm: foundry.pm.in
frr.pm: frr.pm.in
fss2.pm: fss2.pm.in
fxos.pm: fxos.pm.in
ios.pm: ios.pm.in
iossb.pm: iossb.pm.in
iosshtech.pm: iosshtech.pm.in
iosxr.pm: iosxr.pm.in
junos.pm: junos.pm.in
rancid.pm: rancid.pm.in
rancidcontrib.pm: rancidcontrib.pm.in
rancid.py: rancid.py.in
mrv.pm: mrv.pm.in
nxos.pm: nxos.pm.in
panos.pm: panos.pm.in
rbt.pm: rbt.pm.in
routeros.pm: routeros.pm.in
smc.pm: smc.pm.in
sros.pm: sros.pm.in
vrp.pm: vrp.pm.in
wavesvros.pm: wavesvros.pm.in
