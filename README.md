# jumper

Video game.

## Table of Contents

* [Install dependencies](#install-dependencies)
* [Build](#build)
* [Build platforms tested](#build-platforms-tested)
  - [FreeBSD 11.0-RELEASE](#freebsd-110-release)
  - [Fedora 25](#fedora-25)
* [Run](#run)
* [Clean](#clean)
* [Distclean triplet](#distclean-triplet)
* [Distclean](#distclean)

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

```bash
./build.sh
```

Build script honors the `CC` and `MAKE` environment variables.
If no value has been provided for these, they will default to
`cc` and `make` respectively.

## Build platforms tested

The build process has been tested with the following configurations:

### FreeBSD 11.0-RELEASE

* Base `make` + base `cc` (FreeBSD clang 3.8.0).
  - `./build.sh` ok
  - `make clean` ok
  - `make distclean_triplet` ok
  - `make distclean` ok
* Ports `bmake` (portable version of NetBSD make 20161126) +
  ports `clang34` (LLVM clang 3.4.2)
  - `MAKE=bmake CC=clang34 ./build.sh` ok
  - `CC=clang34 bmake clean` ok
  - `CC=clang34 bmake distclean_triplet` ok
  - `bmake distclean` ok
* Ports `gmake` (GNU Make 4.2.1\_1) + ports `gcc49` (GCC 4.9.4)
  - `MAKE=gmake CC=gcc49 ./build.sh` ok
  - `CC=gcc49 gmake clean` ok
  - `CC=gcc49 gmake distclean_triplet` ok
  - `gmake distclean` ok
* Ports `fmake` (Legacy FreeBSD pmake r250982) +
  ports `pcc` (Portable C Compiler 1.1.0)
  - `MAKE=fmake CC=pcc ./build.sh` ok, but `pcc` does not support dumpmachine.
  - `CC=pcc fmake clean` ok, but `fmake` mentions "no input files"
  - `CC=pcc fmake distclean_triplet` ok, but `fmake` mentions "no input files"
  - `fmake distclean` ok
* Ports `dmake` (4.12.20150309) + ports `clang39` (LLVM clang 3.9.1)
  - `MAKE=dmake CC=clang39 ./build.sh` failed;
    `dmake` did not like the `Makefile`

### Fedora 25

* Packages `make` (GNU Make 4.1) + packages `gcc` (GCC 6.3.1)
  - `./build.sh` ok
  - `make clean` ok
  - `make distclean_triplet` ok
  - `make distclean` ok

## Run

If you defined the `CC` or `MAKE` environment variables
during the build stage, adapt accordingly.

```bash
./out/$( cc -dumpmachine )/bin/jumper
```

## Clean

Clean for current host platform. If you defined the `CC` or `MAKE`
environment variables during the build stage, adapt accordingly.

```bash
make clean
```

## Distclean triplet

Distclean for current host platform. If you defined the `CC` or `MAKE`
environment variables during the build stage, adapt accordingly.

```bash
make distclean_triplet
```

## Distclean

Full distclean. Should work regardless of which version of make
and which version of 

```bash
make distclean
```
