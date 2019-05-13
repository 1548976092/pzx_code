FROM debian:stable

ADD linuxcnc-dev /linuxcnc-dev

RUN apt-get update

# needed for compiling
RUN apt-get install autoconf build-essential libudev-dev pkg-config libmodbus-dev -y
RUN apt-get install libusb-1.0-0-dev -y
RUN apt-get install libudev-dev -y
RUN apt-get install libglib2.0-0 libglib2.0-bin libglib2.0-dev -y # for glib
RUN apt-get install gtk2-engines libswt-gtk2-4-jni -y # for gtk
RUN apt-get install procps -y # for ps
RUN apt-get install kmod -y # for lsmod
RUN apt-get install yapps2 -y
RUN apt-get install intltool -y
RUN apt-get install libpth-dev tcl8.6-dev tk8.6-dev bwidget libxaw7-dev python-dev libglu1-mesa-dev libxinerama-dev autoconf python-tk libglib2.0-dev libxft-dev gettext libtk-img tclx -y
RUN apt-get install python-gtk2 -y
RUN apt-get install libreadline-dev -y
RUN apt-get install libboost-python-dev -y

RUN export CPATH=$CPATH:/usr/include/gtk-2.0
RUN export CPATH=$CPATH:/usr/include/glib-2.0/
RUN export CPATH=$CPATH:/usr/lib/glib-2.0/include/
RUN export CPATH=$CPATH:/usr/include/pango-1.0/
RUN export CPATH=$CPATH:/usr/lib/gtk-2.0/include/
RUN export CPATH=$CPATH:/usr/include/atk-1.0/

WORKDIR /linuxcnc-dev/src
RUN ./autogen.sh

RUN ./configure sim --enable-simulator --with-realtime=uspace --disable-gtk --enable-non-distributable=yes
# 'make' not needed get's done by build ing the package anyway
RUN make
RUN make install-menus

# For Building Debian Packages
#RUN apt-get install dpkg-dev -y
#RUN apt-get install debhelper iptables libreadline-gplv2-dev asciidoc dblatex docbook-xsl dvipng ghostscript graphviz groff -y
#RUN apt-get install inkscape python-lxml source-highlight texlive-lang-cyrillic texlive-lang-french texlive-lang-german texlive-lang-polish -y
#RUN apt-get install texlive-lang-spanish w3c-linkchecker libgtk2.0-dev netcat desktop-file-utils -y

# needed at runtime
RUN apt-get install python-serial mesa-utils python-gtksourceview2 python-vte python-xlib -y
RUN apt-get install python-serial mesa-utils python-gnome2 python-glade2 python-imaging python-imaging-tk python-gtksourceview2 python-vte python-xlib -y
RUN apt-get install tclreadline python-configobj python-gtkglext1 -y
RUN apt-get install gir1.2-gst-plugins-base-1.0 gir1.2-gstreamer-1.0 gstreamer1.0-plugins-base  libcap2-bin libcdparanoia0 libgstreamer-plugins-base1.0-0 libgstreamer1.0-0 libopus0 liborc-0.4-0 libpam-cap libtheora0 libvisual-0.4-0 libvorbisenc2 python-gi python-gst-1.0 -y

# configure debian package build
WORKDIR /linuxcnc-dev/debian
RUN ./configure sim

# build debian package
#WORKDIR /linuxcnc-dev
#RUN dpkg-buildpackage -b -uc

# install linuxcnc
#WORKDIR /
#RUN dpkg -i linuxcnc-uspace_2.8.0~pre1_amd64.deb

# create user linuxcnc, run linuxcnc as root does not work.
RUN useradd -ms /bin/bash linuxcnc
USER linuxcnc
WORKDIR /home/linuxcnc
RUN echo '. /linuxcnc-dev/scripts/rip-environment' >> .bashrc

CMD bash
