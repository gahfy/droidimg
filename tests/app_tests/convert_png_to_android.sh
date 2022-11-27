#!/bin/bash

../../src/droidimg img/droidbot.png -o ./ -e m,h,x,xx,xxx
ldpi_md5=($(md5sum drawable-ldpi/droidbot.webp))
echo $ldpi_md5
if [ "$ldpi_md5" != "61ec0e0111c6ee3e956b24ba69f3e262" ]; then
    echo "ldpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,h,x,xx,xxx
mdpi_md5=($(md5sum drawable-mdpi/droidbot.webp))
echo $mdpi_md5
if [ "$mdpi_md5" != "70315527365bdb869cc2ccbe8a8d32c3" ]; then
    echo "mdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,x,xx,xxx
hdpi_md5=($(md5sum drawable-hdpi/droidbot.webp))
echo $hdpi_md5
if [ "$hdpi_md5" != "2ba37ddd1460655dab8f808826f29d64" ]; then
    echo "hdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,xx,xxx
xhdpi_md5=($(md5sum drawable-xhdpi/droidbot.webp))
echo $xhdpi_md5
if [ "$xhdpi_md5" != "482d2c609d265421f90d0f19e003ee74" ]; then
    echo "xhdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,x,xxx
xxhdpi_md5=($(md5sum drawable-xxhdpi/droidbot.webp))
echo $xxhdpi_md5
if [ "$xxhdpi_md5" != "64b34281f127653e3eca20484e60d7c9" ]; then
    echo "xxhdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,x,xx
xxxhdpi_md5=($(md5sum drawable-xxxhdpi/droidbot.webp))
echo $xxxhdpi_md5
if [ "$xxxhdpi_md5" != "bfaee9cd6c518eaed1d7a62c0cdbd383" ]; then
    echo "xxxhdpi image is not the one expected.\n"
    exit 1
fi

rm -rf drawable-ldpi
rm -rf drawable-mdpi
rm -rf drawable-hdpi
rm -rf drawable-xhdpi
rm -rf drawable-xxhdpi
rm -rf drawable-xxxhdpi