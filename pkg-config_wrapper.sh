#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/fred/Dropbox/Taf/Safexis/qt5/raspi/sysroot
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/fred/Dropbox/Taf/Safexis/qt5/raspi/sysroot/usr/lib/pkgconfig:/home/fred/Dropbox/Taf/Safexis/qt5/raspi/sysroot/usr/share/pkgconfig:/home/fred/Dropbox/Taf/Safexis/qt5/raspi/sysroot/usr/lib/arm-linux-gnueabihf/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
