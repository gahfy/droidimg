# DroidImg

## What is this program about

This program is in two parts, a binary, and an Android Studio plugin, in order to improve rasterized image management for Android.

For now, only the program is available, the Android Studio plugin will be developed later.

**_PLEASE CONSIDER THIS AS A PRE_ALPHA VERSION, OR WHATEVER YOU WANT TO CALL THE LEAST STABLE VERSION_**

## Licence

I will use a proper licence when I will get time for it. For now, consider you can do whatever your imagination can think about with this code and that there is no warranty in any form.

## Usage

The program is used with the following arguments :

* `-i` or `--input-file` : **MANDATORY** The PNG input file
* `-o` or `--output-folder` : _Optional_ The output folder to which to add the `webp` images. If skipped, the current directory will be used
* `-n` or `--name` : _Optional_ The name of the images to create. If skipped, the name of the input image will be used.
* `-w` or `--width` : _Optional_ The width, in dp, of the image to create. If both this and height are skipped, we will use the size of the original image as `xxxhdpi` size. If skipped but height is set, the value that respects the proportions of the image will be used.
* `-h` or `--height` : _Optional_ The height, in dp, of the image to create. If skipped, behavior is same as `width` parameter.
* `-d` or `--destination` : _Optional_ The destination retrieved from config file (see below). Ignored if `-o` or `--output` is set.

Just an exemple :

```
droidimg -i my_image.png -o path/to/module/src/main/resources -n nice_drawable -w 100
```

This command will take `my_image.png`, and save `webp` files into `path/to/module/src/main/resources` drawable folders (from `ldpi` to `xxxhdpi`), setting the name of the drawable to `nice_drawable` and its width to `100dp`. It will set the height so the proportions of the original image are preserved.

### Destinations

In order to avoid writing very long path everytime you use the app, you can set destinations.

To do so, all you have to do is to write a file named `.droidimg.config` in the root of your `$HOME` directory, with destinations written like this:

```
destination.myapp : /Users/myname/work/AppName/appmodule/src/main/res
destination.greatmodule : /Users/myname/work/AppName/greatmodule/src/main/res
destination.mysecondapp : /Users/myname/work/SecondAppName/appmodule/src/main/res
```

or (for Windows) :

```
destination.myapp : C:/Users/myname/work/AppName/appmodule/src/main/res
destination.greatmodule : C:\Users\myname\work\AppName\greatmodule\src\main\res
destination.mysecondapp : C:\Users\myname/work/SecondAppName/appmodule/src/main/res
```

_As you can see, you can mix solidus and reverse solidus as much as you want. This is true only for Windows_

**At the beginning of the line, and between name of destination and PATH, you can have as many spaces and tabs as you want. For path name, it should end directly with a new line, otherwise spaces will be considered as part of the folder name.**

Then, running the command like `droidimg -d greatmodule` would have the same effect as running it with `droidimg -o /Users/myname/work/AppName/greatmodule/src/main/res`.

VERY IMPORTANT : The paths in this file must be absolute path, without any environment variable, or `~`.

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

Then run `cd program/droidimg && ./compile.sh` (it is important for now that you are in the folder for compiling)

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

Then run `sed -i 's/\r$//g' compile-cygwin-x86_64.sh` and `sed -i 's/\r$//g' compile-cygwin-i686.sh`.

Now, run `compile-cygwin-x86_64.sh` or `compile-cygwin-i686.sh` in order to compile for `64bits` or `32bits`.

That's all folks. Your program can now be run in Windows, you can find them in:

* `C:\cygwin64\home\username\droidimg\program\droidimg\x86_64` for the 64 bits version
* `C:\cygwin64\home\username\droidimg\program\droidimg\i686` for the 32 bits version

Feel free to rename and move those folder wherever you want, **but keep the dll present in that folder when you move it**.