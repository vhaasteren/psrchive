
nodist_include_HEADERS = psrchive_version.h
.PHONY: psrchive_version.h

psrchive_version.h:
	GIT_HASH=`cd $(top_srcdir); git describe --always --dirty --match 'NOT A TAG'`; echo "#define PSRCHIVE_COMMIT_HASH \"$$GIT_HASH\"" > psrchive_version.tmp
	if diff -q psrchive_version.tmp psrchive_version.h >/dev/null 2>&1; then rm psrchive_version.tmp; else mv psrchive_version.tmp psrchive_version.h; fi

CLEANFILES = psrchive_version.h

SUBDIRS	= . config packages Util Base More Management

include $(top_srcdir)/config/Makefile.root

ACLOCAL_AMFLAGS = -I config
AM_CPPFLAGS = @EPSIC_CFLAGS@

# make clean deletes the local_include directory (and any out-of-date headers)
clean-local: clean-local-include
.PHONY: clean-local-include
clean-local-include:
	-rm -rf local_include

