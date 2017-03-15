# jumper

Video game.

## Table of Contents

* [Install dependencies](#install-dependencies)
* [Build](#build)
  - [Debug-build](#debug-build)
  - [Release-build](#release-build)
* [Build platforms tested](#build-platforms-tested)
* [Run](#run)
* [Clean](#clean)
  - [Clean for target platform](#clean-for-target-platform)
  - [Distclean for target platform](#distclean-for-target-platform)
  - [Full distclean](#full-distclean)

## Install dependencies

FreeBSD 11.0-RELEASE:

```bash
doas pkg install python3 sdl2 sdl2_image sdl2_ttf
```

Fedora 25:

```bash
sudo dnf install python3 gcc SDL2 SDL2-devel \
	SDL2_image SDL2_image-devel SDL2_ttf SDL2_ttf-devel
```

## Build

By default, the build script will use `cc` for compiling C programs.
To override which C compiler is used, set the `CC` environment variable
or set value for `CC` in the form of a `key=value` pair in the list of
arguments passed to `make.py`. All variable assignments need to come
before any targets appear in the list of arguments.

If you would like to put build files and out files under
`$HOME/build` and `$HOME/out` respectively, you may do the
following:

```bash
mkdir -p $HOME/build && touch $HOME/build/.allow_from_any
mkdir -p $HOME/out && touch $HOME/out/.allow_from_any
```

This is useful for example if you want to synchronize projects
across machines without synchronizing the build files and out files.

Another thing it's useful for is that having one common pair of
build and out file directories allows you to mount a RAM disk
for all of these writes should you desire to do so.

Then to perform a debug or release build, type the command as shown below.

### Debug-build

```bash
./buildtools/make.py
```

### Release-build

```bash
./buildtools/make.py release
```

## Build platforms tested

Currently this project is built using the following
operating systems and C compilers.

TODO: Continuous Integration proper.

### FreeBSD 11.0-RELEASE

* `CC=cc ./buildtools/make.py` (Base *FreeBSD clang 3.8.0*).
* `CC=clang39 ./buildtools/make.py` (Ports *LLVM clang 3.9.1*).
* `CC=clang34 ./buildtools/make.py` (Ports *LLVM clang 3.4.2*).
* `CC=gcc49 ./buildtools/make.py` (Ports *GCC 4.9.4*).
* `CC=pcc ./buildtools/make.py` (Ports *Portable C Compiler 1.1.0*).

### Fedora 25

* `CC=gcc ./buildtools/make.py` (*Packages* GCC 6.3.1).

## Run

NOTE: If you defined `CC` during the build stage,
ensure that the same value is provided still.

```bash
./buildtools/make.py run
```

## Clean

### Clean for target platform

NOTE: If you defined `CC` during the build stage,
ensure that the same value is provided still.

```bash
./buildtools/make.py clean
```

### Distclean for target platform

NOTE: If you defined `CC` during the build stage,
ensure that the same value is provided still.

```bash
./buildtools/make.py distclean_triplet
```

### Full distclean

The value of `CC` does not make a difference here.

```bash
./buildtools/distclean
```
