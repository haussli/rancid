## Process this file with automake to produce Makefile.in
## A Makefile.in is supplied, in case you do not have automake.

AUTOMAKE_OPTIONS=foreign no-dependencies

@SET_MAKE@

man_gen_MANS = env.5
man_nogen_MANS = do-diffs.1 clogin.1 rancid_intro.1 \
		cloginrc.5 router.db.5

man_MANS = $(man_gen_MANS) $(man_nogen_MANS)

EXTRA_DIST = $(man_nogen_MANS)

