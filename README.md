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

You may want to set `--with-png-dir` and `--with-webp-dir` if you set custom installation directories for those libraries.

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

Binaries have been provided for windows.
Download the zip and extract it wherever you want.