AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"desktop-icons-applet\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

#
# Dicons plugin
#
plugin_LTLIBRARIES = \
	libdicons.la

plugindir = \
	$(libdir)/xfce4/panel/plugins

libdicons_la_SOURCES = \
	desktop-icons-applet.c

libdicons_la_CFLAGS = \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCONF_CFLAGS) \
	$(LIBXFCE4PANEL_CFLAGS) \
	$(PLATFORM_CFLAGS)

libdicons_la_LDFLAGS = \
       -avoid-version \
       -module \
       -no-undefined \
       -export-symbols-regex '^xfce_panel_module_(preinit|init|construct)' \
       $(PLATFORM_LDFLAGS)

libdicons_la_LIBADD = \
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCONF_LIBS) \
	$(LIBXFCE4PANEL_LIBS)

#
# Desktop file
#
desktopdir =								\
	$(datadir)/xfce4/panel/plugins

desktop_DATA =								\
	desktop-icons-applet.desktop

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	desktop-icons-applet.desktop

CLEANFILES =								\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
