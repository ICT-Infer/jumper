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
doas pkg install sdl2 sdl2_image sdl2_ttf
```

Fedora 25:

```bash
sudo dnf install gcc make SDL2 SDL2-devel \
	SDL2_image SDL2_image-devel SDL2_ttf SDL2_ttf-devel
```

## Build

Build script honors the `CC` and `MAKE` environment variables.
If no value has been provided for these, they will default to
`cc` and `make` respectively.

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
./make.sh
```

### Release-build

```bash
BUILDTYPE=release ./make.sh
```

## Build platforms tested

Currently the following build platforms are tested by manually
triggering the script `buildtools/allbuilds.sh`.

TODO: Continuous Integration proper.

### FreeBSD 11.0-RELEASE

* Base `make` + base `cc` (FreeBSD clang 3.8.0).
* Base `make` + ports `clang39` (LLVM clang 3.9.1).
* Ports `bmake` (portable version of NetBSD make 20161126) +
  ports `clang34` (LLVM clang 3.4.2).
* Ports `gmake` (GNU Make 4.2.1\_1) + ports `gcc49` (GCC 4.9.4).
* Ports `fmake` (Legacy FreeBSD pmake r250982) +
  ports `pcc` (Portable C Compiler 1.1.0).

### Fedora 25

* Packages `make` (GNU Make 4.1) + packages `gcc` (GCC 6.3.1).

## Run

If you defined the `CC` or `MAKE` environment variables
during the build stage, ensure that the same values are
provided still.

```bash
./make.sh run
```

## Clean

### Clean for target platform

If you defined the `CC` or `MAKE` environment variables
during the build stage, ensure that the same values are
provided still.

```bash
./make.sh clean
```

### Distclean for target platform

If you defined the `CC` or `MAKE` environment variables
during the build stage, ensure that the same values are
provided still.

```bash
./make.sh distclean_triplet
```

### Full distclean

The values of `CC` and `MAKE` should not make a difference here.

```bash
./make.sh distclean
```
