#!/bin/sh

../../src/droidimg img/droidbot.png -o ./ -e m,h,x,xx,xxx
ldpi_md5=($(md5sum drawable-ldpi/droidbot.webp))
if [ "$ldpi_md5" != "f9d5dd124f3bfb249f6f72ade3b257a3" ]; then
    echo "ldpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,h,x,xx,xxx
mdpi_md5=($(md5sum drawable-mdpi/droidbot.webp))
if [ "$mdpi_md5" != "74a082867eb646436868b0be6f57741d" ]; then
    echo "mdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,x,xx,xxx
hdpi_md5=($(md5sum drawable-hdpi/droidbot.webp))
if [ "$hdpi_md5" != "b5e481a597ca5e3a62ab27f85fa0dbad" ]; then
    echo "hdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,xx,xxx
xhdpi_md5=($(md5sum drawable-xhdpi/droidbot.webp))
if [ "$xhdpi_md5" != "72ca6322765b4cbc78804adeb11488f6" ]; then
    echo "xhdpi image is not the one expected.\n"
    echo "$xhdpi_md5"
    echo "72ca6322765b4cbc78804adeb11488f6"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,x,xxx
xxhdpi_md5=($(md5sum drawable-xxhdpi/droidbot.webp))
if [ "$xxhdpi_md5" != "3d8ec07d57b93528b7cb7c973e0e33d0" ]; then
    echo "xxhdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,x,xx
xxxhdpi_md5=($(md5sum drawable-xxxhdpi/droidbot.webp))
if [ "$xxxhdpi_md5" != "524a63c66e0081d2ece7b8cff7031323" ]; then
    echo "xxxhdpi image is not the one expected.\n"
    exit 1
fi

rm -rf drawable-ldpi
rm -rf drawable-mdpi
rm -rf drawable-hdpi
rm -rf drawable-xhdpi
rm -rf drawable-xxhdpi
rm -rf drawable-xxxhdpi