rm -rf ./build
mkdir ./build
mkdir ./build/utils

gcc -g -W -Wall -std=c99 -c ./utils/binary_utils.c -o ./build/utils/binary_utils.o
gcc -g -W -Wall -std=c99 -c ./utils/memory_utils.c -o ./build/utils/memory_utils.o
gcc -g -W -Wall -std=c99 -c ./utils/string_utils.c -o ./build/utils/string_utils.o
gcc -g -W -Wall -std=c99 -c ./utils/android_utils.c -o ./build/utils/android_utils.o
gcc -g -W -Wall -std=c99 "-I$LIBWEBP_HOME/include" -c ./utils/webp_utils.c -o ./build/utils/webp_utils.o
gcc -g -W -Wall -std=c99 "-I$LIBPNG_HOME/include" -c ./utils/png_utils.c -o ./build/utils/png_utils.o
gcc -g -W -Wall -std=c99 -c ./utils/picture_utils.c -o ./build/utils/picture_utils.o
gcc -g -W -Wall -std=c99 "-I$LIBPNG_HOME/include" "-I$LIBWEBP_HOME/include" -c ./droidimg.c -o ./build/droidimg.o
gcc ./build/droidimg.o ./build/utils/picture_utils.o ./build/utils/string_utils.o ./build/utils/android_utils.o ./build/utils/webp_utils.o ./build/utils/png_utils.o ./build/utils/binary_utils.o ./build/utils/memory_utils.o -o droidimg "-L$LIBWEBP_HOME/lib" "-L$LIBPNG_HOME/lib" -lpng -lwebp