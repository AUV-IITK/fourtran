#!/bin/bash
## This script copies all the udev rules specified in the utils/udev directory to /etc/udev/rules.d/

# For color print statements
SWITCH="\033["
RED="${SWITCH}0;31m"     # red color
NC="${SWITCH}0m"     # no color
GREEN="${SWITCH}0;32m"    # green color

# check if script run as root or not
if (( $EUID != 0 )); then
       echo -e "${RED}[ERROR] Please run the script as root${NC}"
        exit
else
    cd $(dirname $0)

    # Copy webcamera udev rules
    sudo cp -i ./udev/83-webcam.rules /etc/udev/rules.d/
    sudo cp -i ./udev/72-micro-devel.rules /etc/udev/rules.d/

    echo -e "${GREEN}[INFO] Copied udev rules to /etc/udev/rules.d successfully${NC}"

fi
