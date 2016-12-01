if PLATFORM_WIN32
AM_LDFLAGS = -module -avoid-version -no-undefined
else
AM_LDFLAGS = -module -avoid-version
endif

noinst_HEADERS = util.h


AM_CPPFLAGS = \
	-I$(top_builddir)		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/babl		\
	-I$(top_srcdir)/extensions

extdir = $(libdir)/babl-@BABL_API_VERSION@
ext_LTLIBRARIES = \
	cairo.la        \
	CIE.la          \
	naive-CMYK.la

cairo_la_SOURCES = cairo.c
CIE_la_SOURCES = CIE.c
naive_CMYK_la_SOURCES = naive-CMYK.c

LIBS = $(top_builddir)/babl/libbabl-@BABL_API_VERSION@.la $(MATH_LIB) \
	$(THREAD_LIB)
