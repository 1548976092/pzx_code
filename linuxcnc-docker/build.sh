#!/bin/bash

DIRECTORY="linuxcnc-dev"

if [ ! -d "$DIRECTORY" ]; then
    echo "downloading newest linuxcnc"
    git clone https://github.com/LinuxCNC/linuxcnc.git $DIRECTORY
else
    echo "using existing $DIRECTORY"
fi

docker build -t linuxcnc-dev .
