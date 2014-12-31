MULTIMEDIA_CONTROL_FLAGS = @MULTIMEDIA_CONTROL_FLAGS@
MULTIMEDIA_CONTROL_LIBS = @MULTIMEDIA_CONTROL_LIBS@

AM_CPPFLAGS = @GTK_CFLAGS@ @MULTIMEDIA_CONTROL_FLAGS@
LDADD = @GTK_LIBS@ @MULTIMEDIA_CONTROL_LIBS@

libexec_PROGRAMS = multimedia-control-applet

multimedia_control_applet_SOURCES = main.c applet.h 

appletdir       = $(datadir)/mate-panel/applets
applet_in_files = org.mate.applets.MultimediaControlApplet.mate-panel-applet.in
applet_DATA     = $(applet_in_files:.mate-panel-applet.in=.mate-panel-applet)

servicedir       = $(datadir)/dbus-1/services
service_in_files = org.mate.panel.applet.MultimediaControlApplet.service.in
service_DATA     = $(service_in_files:.service.in=.service)