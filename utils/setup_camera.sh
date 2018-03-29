#!/bin/sh

echo "Enter vendor id of the camera"
read vendorId

echo "Enter product id of the camera"
read productId

echo "ACTION==\"add\", KERNEL==\"video[0-9]*\", SUBSYSTEM==\"video4linux\", SUBSYSTEMS==\"usb\", ATTRS{idVendor}==\"$vendorId\", ATTRS{idProduct}==\"$productId\", SYMLINK+=\"fourtran_camera\"" >> /etc/udev/rules.d/83-webcam.rules
