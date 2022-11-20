rm -rf ./build
rm -rf ./i686
mkdir ./build
mkdir ./build/utils

mkdir ./i686

i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/binary_utils.c -o ./build/utils/binary_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/binary_utils.c -o ./build/utils/binary_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/memory_utils.c -o ./build/utils/memory_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/string_utils.c -o ./build/utils/string_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/android_utils.c -o ./build/utils/android_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 "-I/temp/libwebp/include" -c ./utils/webp_utils.c -o ./build/utils/webp_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/png_utils.c -o ./build/utils/png_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 -c ./utils/picture_utils.c -o ./build/utils/picture_utils.o
i686-w64-mingw32-gcc.exe -g -W -Wall -std=c99 "-I/temp/libwebp/include" -c ./droidimg.c -o ./build/droidimg.o
i686-w64-mingw32-gcc.exe ./build/droidimg.o ./build/utils/picture_utils.o ./build/utils/string_utils.o ./build/utils/android_utils.o ./build/utils/webp_utils.o ./build/utils/png_utils.o ./build/utils/binary_utils.o ./build/utils/memory_utils.o -o ./i686/droidimg.exe "-L/temp/libwebp/lib" -lpng -lwebp -static-libgcc

cp /usr/i686-w64-mingw32/sys-root/mingw/bin/libpng16-16.dll ./i686/libpng16-16.dll
cp /usr/i686-w64-mingw32/sys-root/mingw/bin/zlib1.dll ./i686/zlib1.dll
cp /usr/i686-w64-mingw32/sys-root/mingw/bin/libgcc_s_sjlj-1.dll ./i686/libgcc_s_sjlj-1.dll
cp /usr/i686-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll ./i686/libwinpthread-1.dll