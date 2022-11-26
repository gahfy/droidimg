#!/bin/sh

../../src/droidimg img/droidbot.png -o ./
ldpi_md5=$(md5 -q drawable-ldpi/droidbot.webp)
mdpi_md5=$(md5 -q drawable-mdpi/droidbot.webp)
hdpi_md5=$(md5 -q drawable-hdpi/droidbot.webp)
xhdpi_md5=$(md5 -q drawable-xhdpi/droidbot.webp)
xxhdpi_md5=$(md5 -q drawable-xxhdpi/droidbot.webp)
xxxhdpi_md5=$(md5 -q drawable-xxxhdpi/droidbot.webp)

if [ "$ldpi_md5" != "f9d5dd124f3bfb249f6f72ade3b257a3" ]; then
    echo "ldpi image is not the one expected.\n"
    exit 1
fi

if [ "$mdpi_md5" != "74a082867eb646436868b0be6f57741d" ]; then
    echo "mdpi image is not the one expected.\n"
    exit 1
fi

if [ "$hdpi_md5" != "b5e481a597ca5e3a62ab27f85fa0dbad" ]; then
    echo "hdpi image is not the one expected.\n"
    exit 1
fi

if [ "$xhdpi_md5" != "72ca6322765b4cbc78804adeb11488f6" ]; then
    echo "hdpi image is not the one expected.\n"
    exit 1
fi

if [ "$xxhdpi_md5" != "3d8ec07d57b93528b7cb7c973e0e33d0" ]; then
    echo "hdpi image is not the one expected.\n"
    exit 1
fi

if [ "$xxxhdpi_md5" != "524a63c66e0081d2ece7b8cff7031323" ]; then
    echo "hdpi image is not the one expected.\n"
    exit 1
fi

rm -rf drawable-ldpi
rm -rf drawable-mdpi
rm -rf drawable-hdpi
rm -rf drawable-xhdpi
rm -rf drawable-xxhdpi
rm -rf drawable-xxxhdpi