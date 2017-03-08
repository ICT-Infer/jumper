# jumper

Video game.

## Table of Contents

* [Install dependencies](#install-dependencies)
* [Build](#build)
  - [Debug-build](#debug-build)
  - [Release-build](#release-build)
* [Build platforms tested](#build-platforms-tested)
  - [FreeBSD 11.0-RELEASE](#freebsd-110-release)
  - [Fedora 25](#fedora-25)
* [Run](#run)
* [Clean](#clean)
  - [Clean for target platform](#clean-for-target-platform)
  - [Distclean for target platform](#distclean-for-target-platform)
  - [Full distclean](#full-distclean)

## Install dependencies

FreeBSD 11.0-RELEASE:

```bash
doas pkg install sdl2
```

Fedora 25:

```bash
sudo dnf install gcc make SDL2 SDL2-devel
```

## Build

Build script honors the `CC` and `MAKE` environment variables.
If no value has been provided for these, they will default to
`cc` and `make` respectively.

### Debug-build

```bash
./make.sh
```

### Release-build

```bash
BUILDTYPE=release ./make.sh
```

## Build platforms tested

The build process has been tested with the following configurations:

### FreeBSD 11.0-RELEASE

* Base `make` + base `cc` (FreeBSD clang 3.8.0).
  - `./make.sh` ok
  - `./make.sh clean` ok
  - `./make.sh distclean_triplet` ok
  - `./make.sh distclean` ok
* Ports `bmake` (portable version of NetBSD make 20161126) +
  ports `clang34` (LLVM clang 3.4.2)
  - `MAKE=bmake CC=clang34 ./make.sh` ok
  - `MAKE=bmake CC=clang34 ./make.sh clean` ok
  - `MAKE=bmake CC=clang34 ./make.sh distclean_triplet` ok
  - `MAKE=bmake ./make.sh distclean` ok
* Ports `gmake` (GNU Make 4.2.1\_1) + ports `gcc49` (GCC 4.9.4)
  - `MAKE=gmake CC=gcc49 ./make.sh` ok
  - `MAKE=gmake CC=gcc49 ./make.sh clean` ok
  - `MAKE=gmake CC=gcc49 ./make.sh distclean_triplet` ok
  - `MAKE=gmake ./make.sh distclean` ok
* Ports `fmake` (Legacy FreeBSD pmake r250982) +
  ports `pcc` (Portable C Compiler 1.1.0)
  - `MAKE=fmake CC=pcc ./make.sh` ok, but `pcc` does not support dumpmachine.
  - `MAKE=fmake CC=pcc ./make.sh clean` ok, but
    `fmake` mentions "no input files"
  - `MAKE=fmake CC=pcc ./make.sh distclean_triplet` ok, but
    `fmake` mentions "no input files"
  - `MAKE=fmake ./make.sh distclean` ok
* Ports `dmake` (4.12.20150309) + ports `clang39` (LLVM clang 3.9.1)
  - `MAKE=dmake CC=clang39 ./make.sh` failed;
    `dmake` did not like the `Makefile`

### Fedora 25

* Packages `make` (GNU Make 4.1) + packages `gcc` (GCC 6.3.1)
  - `./make.sh` ok
  - `./make.sh clean` ok
  - `./make.sh distclean_triplet` ok
  - `./make.sh distclean` ok

## Run

If you defined the `CC` or `MAKE` environment variables
during the build stage, ensure that the same values are
provided still.

```bash
./make.sh run
```

## Clean

If you defined the `CC` or `MAKE` environment variables
during the build stage, ensure that the same values are
provided still.

### Clean for target platform

```bash
./make.sh clean
```

### Distclean for target platform

```bash
./make.sh distclean_triplet
```

### Full distclean

```bash
./make.sh distclean
```
