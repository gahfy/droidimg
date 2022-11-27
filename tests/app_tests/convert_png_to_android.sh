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
if [ "$xhdpi_md5" != "89d5ad09cd5ac0637a30252b29508c91" ]; then
    echo "xhdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,x,xxx
xxhdpi_md5=($(md5sum drawable-xxhdpi/droidbot.webp))
if [ "$xxhdpi_md5" != "6a048a41fbd98737cab86a2c3f34f4b4" ]; then
    echo "xxhdpi image is not the one expected.\n"
    exit 1
fi

../../src/droidimg img/droidbot.png -o ./ -e l,m,h,x,xx
xxxhdpi_md5=($(md5sum drawable-xxxhdpi/droidbot.webp))
if [ "$xxxhdpi_md5" != "4356444b971d7c052ebfcfabd3cb5cf3" ]; then
    echo "xxxhdpi image is not the one expected.\n"
    exit 1
fi

rm -rf drawable-ldpi
rm -rf drawable-mdpi
rm -rf drawable-hdpi
rm -rf drawable-xhdpi
rm -rf drawable-xxhdpi
rm -rf drawable-xxxhdpi