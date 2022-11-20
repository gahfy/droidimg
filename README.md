# DroidImg

## What is this program about

This program is in two parts, a binary, and an Android Studio plugin, in order to improve rasterized image management for Android.

For now, only the program is available, the Android Studio plugin will be developed later.

**_PLEASE CONSIDER THIS AS A PRE_ALPHA VERSION, OR WHATEVER YOU WANT TO CALL THE LEAST STABLE VERSION_**

### Usage

The program is used with the following arguments :

* `-i` or `--input-file` : **MANDATORY** The PNG input file
* `-o` or `--output-folder` : _Optional_ The output folder to which to add the `webp` images. If skipped, the current directory will be used
* `-n` or `--name` : _Optional_ The name of the images to create. If skipped, the name of the input image will be used.
* `-w` or `--width` : _Optional_ The width, in dp, of the image to create. If both this and height are skipped, we will use the size of the original image as `xxxhdpi` size. If skipped but height is set, the value that respects the proportions of the image will be used.
* `-h` or `--height` : _Optional_ The height, in dp, of the image to create. If skipped, behavior is same as `width` parameter.

Just an exemple :

```
droidimg -i my_image.png -o path/to/module/src/main/resources -n nice_drawable -w 100
```

This command will take `my_image.png`, and save `webp` files into `path/to/module/src/main/resources` drawable folders (from `ldpi` to `xxxhdpi`), setting the name of the drawable to `nice_drawable` and its width to `100dp`. It will set the height so the proportions of the original image are preserved.

## Compile

### Linux and macOS

You need [Libpng](http://www.libpng.org/pub/png/libpng.html) and [Libwebp](https://developers.google.com/speed/webp/download) to be able to compile this program.

**_Debian and Ubuntu_**

```
apt-get install libpng-dev libwebp-dev
```

**_macOS_**

```
brew install libpng libwebp
```

Set their home directory to environment variables `LIBPNG_HOME` and `LIBWEBP_HOME`.

**YOU HAVE TO SET THEM, set them to `/usr` if you used apt or to `/usr/local` if you used homebrew.**

Then run `program/droidimg/compile.sh`

### Windows

Here are the steps to compile droidimg on Windows.

_As I said, I'm a beginner in C, and didn't work with Windows since decades. If you know how to compile without the need to keep the dlls, or if you know a better/easier way to compile, feel free to share_

#### Cygwin, MinGW-w64 and libpng

You will need [Cygwin](https://cygwin.com/).

Install the following packages in Cygwin:

* `mingw64-i686-gcc-core` in order to compile a 32 bits executable
* `mingw64-x86_64-gcc-core` in order to compile a 64 bits executable
* `mingw64-i686-libpng` in order to compile a 32 bits executable
* `mingw64-x86_64-libpng` in order to compile a 64 bits executable

_Unfortunately, `libwebp` in MinGW-W64 is too old, so we will download it ourselves_

#### Libwebp

Go to the [Downloads repository](https://storage.googleapis.com/downloads.webmproject.org/releases/webp/index.html) of WebP.

* Download `libwebp-1.0.3-windows-x86-no-wic.zip` in order to compile a 32 bits executable
* Create a folder `C:\cygwin64\temp` and extract libwebp inside in a folder named `libwebp` _(So that in `C:\cygwin64\temp\libwebp` you have folders named `bin`, `include`, `lib`, ...)_
* Download `libwebp-1.2.4-windows-x64-no-wic.zip` and extract it in a folder named `libwebp64` in `C:\cygwin64\temp` _(So that in `C:\cygwin64\temp\libwebp64` you have folders named `bin`, `include`, `lib`, ...)_

#### Cygwin terminal

Open a cygwin terminal, and clone the repository wherever you want. Then, go to `program/droidimg`.

Now, run `compile-cygwin-x86_64.sh` or `compile-cygwin-i686.sh` in order to compile for `64bits` or `32bits`.

That's all folks. Your program can now be run in Windows, you can find them in:

* `C:\cygwin64\home\username\droidimg\program\droidimg\x86_64` for the 64 bits version
* `C:\cygwin64\home\username\droidimg\program\droidimg\i686` for the 32 bits version

Feel free to rename and move those folder wherever you want, **but keep the dll present in that folder when you move it**.

## Use droidimg

