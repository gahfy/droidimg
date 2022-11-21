rm -rf ./build
rm -rf ./x86_64
mkdir ./build
mkdir ./build/utils

mkdir ./x86_64

x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/config_utils.c -o ./build/utils/config_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/binary_utils.c -o ./build/utils/binary_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/binary_utils.c -o ./build/utils/binary_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/memory_utils.c -o ./build/utils/memory_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/string_utils.c -o ./build/utils/string_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/android_utils.c -o ./build/utils/android_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 "-I/temp/libwebp64/include" -c ./utils/webp_utils.c -o ./build/utils/webp_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/png_utils.c -o ./build/utils/png_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/picture_utils.c -o ./build/utils/picture_utils.o
x86_64-w64-mingw32-gcc.exe -g -W -Wall -std=c99 "-I/temp/libwebp64/include" -c ./droidimg.c -o ./build/droidimg.o
x86_64-w64-mingw32-gcc.exe ./build/utils/config_utils.o ./build/droidimg.o ./build/utils/picture_utils.o ./build/utils/string_utils.o ./build/utils/android_utils.o ./build/utils/webp_utils.o ./build/utils/png_utils.o ./build/utils/binary_utils.o ./build/utils/memory_utils.o -o ./x86_64/droidimg.exe "-L/temp/libwebp64/lib" -lpng -lwebp -static-libgcc

cp /usr/x86_64-w64-mingw32/sys-root/mingw/bin/libpng16-16.dll ./x86_64/libpng16-16.dll
cp /usr/x86_64-w64-mingw32/sys-root/mingw/bin/zlib1.dll ./x86_64/zlib1.dll
