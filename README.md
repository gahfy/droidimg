# DroidImg

## Most important first

The development of this project is at the beginning, so going fast, and this `README.md` file is changing a lot, according to updates to the project.
Please rely on the `README.md` file which is included in the release you have downloaded.

## What is this program about

This program allows you to convert rasterized image to webp images that will be stored in the drawables folder of your Android project.

**_PLEASE CONSIDER THIS AS A PRE_ALPHA VERSION, OR WHATEVER YOU WANT TO CALL THE LEAST STABLE VERSION_**

## Usage

The program is used with the following arguments :

* **MANDATORY** The PNG input file
* `-o` or `--output-folder` : _Optional_ The output folder to which to add the `webp` images. If skipped, the current directory will be used
* `-n` or `--name` : _Optional_ The name of the images to create. If skipped, the name of the input image will be used.
* `-w` or `--width` : _Optional_ The width, in dp, of the image to create. If both this and height are skipped, we will use the size of the original image as `xxxhdpi` size. If skipped but height is set, the value that respects the proportions of the image will be used.
* `-h` or `--height` : _Optional_ The height, in dp, of the image to create. If skipped, behavior is same as `width` parameter.
* `-d` or `--destination` : _Optional_ The destination retrieved from config file (see below). Ignored if `-o` or `--output` is set.
* `-e` or `--exclude` : _Optional_ Should contain one or more of `l`, `m`, `h`, `x`, `xx` and `xxx` all separated by a comma, to exclude respectively conversion to `ldpi`, `mdpi`, `hdpi`, `xhdpi`, `xxhdpi`, `xxxhdpi`.

Just an exemple :

```
droidimg my_image.png -o path/to/module/src/main/resources -n nice_drawable -w 100 -e l,m
```

This command will take `my_image.png`, and save `webp` files into `path/to/module/src/main/resources` drawable folders (from `hdpi` to `xxxhdpi` as we exclude `ldpi` and `mdpi`), setting the name of the drawable to `nice_drawable` and its width to `100dp`. It will set the height so the proportions of the original image are preserved.

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

## Installation

### Linux and macOS

#### Installation of dependencies

You need [Libpng](http://www.libpng.org/pub/png/libpng.html) and [Libwebp](https://developers.google.com/speed/webp/download) to be able to compile this program.

**_Debian and Ubuntu_**

```
apt-get install libpng-dev libwebp-dev
```

**_macOS_**

```
brew install libpng libwebp
```

#### Download and extract archive

Download `droidimg-0.01.tar.gz` from the [release](https://github.com/gahfy/droidimg/releases/tag/v0.01)

Extract it with the following command:

```
tar -xzf droidimg-0.01.tar.gz
```

Then move inside the folder:

```
cd droidimg-0.01
```

#### Configuring

Then, run configure script with:

```
./configure
```

You may want to set `LIBPNG_CFLAGS`, `LIBPNG_LIBS`, `LIBWEBP_CFLAGS` and/or `LIBWEBP_LIBS` if you set custom installation directories for those libraries. Alternatively, you may rely on `pkg-config` to find the libraries if you set `PKG_CONFIG_PATH` properly.

#### Compiling

Then run

```
make
```

To compile the application.

#### Installing

Then run

```
make
```

To install the application. You may have to run `sudo make` regarding your permissions.

### Windows

#### From binaries

Binaries have been provided for windows in the [releases](https://github.com/gahfy/droidimg/releases).
Download the zip and extract it wherever you want.

#### Compile from sources

You can compile for Windows with the archive in the binaries.
To do so, just use [Cygwin](https://www.cygwin.com/), and install mingw-w64 packages for gcc, libpng, and libwebp.

Here is the command to configure that has been used to provide binaries:

##### Windows 32bits (i686)

```
CC=i686-w64-mingw32-gcc ./configure CFLAGS="-I/usr/i686-w64-mingw32/sys-root/mingw/include" LIBS="-L/usr/i686-w64-mingw32/sys-root/mingw/include -lpng -lwebp"
```

Missing dlls can be found in `/usr/i686-w64-mingw32/sys-root/mingw/bin`.

##### Windows 64bits (x86_64)

```
CC=x86_64-w64-mingw32-gcc ./configure CFLAGS="-I/usr/x86_64-w64-mingw32/sys-root/mingw/include" LIBS="-L/usr/x86_64-w64-mingw32/sys-root/mingw/include -lpng -lwebp"
```

Missing dlls can be found in `/usr/x86_64-w64-mingw32/sys-root/mingw/bin`.