# DroidImg

## Compile

You need [Libpng](http://www.libpng.org/pub/png/libpng.html) and [Libwebp](https://developers.google.com/speed/webp/download) to be able to compile this program.

Set their home directory to environment variables `LIBPNG_HOME` and `LIBWEBP_HOME`.
Then run `compile.sh`

## Use droidimg

Arguments:

* `-i` or `--input-file` : **MANDATORY** The PNG input file
* `-o` or `--output-folder` : _Optional_ The output folder to which to add the `webp` images. If skipped, the current directory will be used
* `-n` or `--name` : _Optional_ The name of the images to create. If skipped, the name of the input image will be used.
* `-w` or `--width` : _Optional_ The width, in dp, of the image to create. If both this and height are skipped, we will use the size of the original image as `xxxhdpi` size. If skipped but height is set, the value that respects the proportions of the image will be used.
* `-h` or `--height` : _Optional_ The height, in dp, of the image to create. If skipped, behavior is same as `width` parameter.