
Debian
====================
This directory contains files used to package royalbitcoind/royalbitcoin-qt
for Debian-based Linux systems. If you compile royalbitcoind/royalbitcoin-qt yourself, there are some useful files here.

## royalbitcoin: URI support ##


royalbitcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install royalbitcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your royalbitcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/royalbitcoin128.png` to `/usr/share/pixmaps`

royalbitcoin-qt.protocol (KDE)

