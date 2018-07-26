#!/bin/sh
while [ 1 ]
do
        if ls /dev/ttyUSB3 | grep /dev/ttyUSB3
        then echo 0 > /sys/class/leds/minibox_lte:status/brightness
        else
        echo 1 > /sys/class/leds/minibox_lte:status/brightness
        fi
done
