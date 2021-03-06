# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.66])
AC_INIT([mate-applet-multimedia-control], [0.1], [mati86dl@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

AM_CONFIG_HEADER([config.h])

# prefix
if test "x$prefix" = "xNONE" ; then
	prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE" ; then
	exec_prefix=$prefix
fi
PREFIX=$prefix
AC_SUBST([PREFIX])
BINDIR=$exec_prefix/bin
AC_SUBST([BINDIR])
SBINDIR=$exec_prefix/sbin
AC_SUBST([SBINDIR])
DATAROOTDIR=$prefix/share
AC_SUBST([DATAROOTDIR])
LIBEXECDIR=$prefix/libexec
if test "$libexecdir" != '${exec_prefix}/libexec'; then
        LIBEXECDIR=$libexecdir
fi
AC_SUBST([LIBEXECDIR])

### Checks for programs

# GCC
AC_PROG_CC
MAKEFILE_CC=`which gcc`
AC_SUBST([MAKEFILE_CC])

### Checks for libraries.
LIBMATEPANELAPPLET="no"
PKG_CHECK_MODULES([LIBMATEPANELAPPLET_4], [libmatepanelapplet-4.0], [LIBMATEPANELAPPLET="libmatepanelapplet-4.0"], [
        PKG_CHECK_MODULES([LIBMATEPANELAPPLET_3], [libmatepanelapplet-3.0], [LIBMATEPANELAPPLET="libmatepanelapplet-3.0"], [
                PKG_CHECK_MODULES([LIBMATEPANELAPPLET_2], [libmatepanelapplet-2.0], [LIBMATEPANELAPPLET="libmatepanelapplet-2.0"], [
                        AC_MSG_ERROR(["*** libmatepanelapplet not found."])
                ])
        ])
])

### Check for packages
PKG_CHECK_MODULES([GTK], [gtk+-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([MPRIS2CLIENT], [libmpris2client])

# Makefiles
MULTIMEDIA_CONTROL_FLAGS=`$PKG_CONFIG --cflags-only-I $LIBMATEPANELAPPLET libmpris2client`
MULTIMEDIA_CONTROL_LIBS=`$PKG_CONFIG --libs $LIBMATEPANELAPPLET libmpris2client`
MULTIMEDIA_CONTROL_LIBS="$MULTIMEDIA_CONTROL_LIBS -lX11"
AC_SUBST([MULTIMEDIA_CONTROL_FLAGS])
AC_SUBST([MULTIMEDIA_CONTROL_LIBS])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_FILES([src/org.mate.applets.MultimediaControlApplet.mate-panel-applet])
AC_CONFIG_FILES([src/org.mate.panel.applet.MultimediaControlApplet.service])

# Grand finale
AC_OUTPUT

echo
echo "Configuration complete."
echo
echo "Now, run make."
echo 
