#!/usr/bin/env sh

set -e

err ()
{
  echo "$0: ERROR:" $@ 1>&2
  exit 1
}

BUILDTYPE=
CC=
MAKE=

most_recent_set_of_values ()
{
  echo 1>&2
  echo "Most recent set values passed for environment variables:" 1>&2
  echo "----------------------------------------------- IMPLICIT" 1>&2
  env
  echo "----------------------------------------------- EXPLICIT" 1>&2
  echo "BUILDTYPE=$BUILDTYPE" 1>&2
  echo "CC=$CC" 1>&2
  echo "MAKE=$MAKE" 1>&2
  echo "" 1>&2
}

trap most_recent_set_of_values EXIT

[ -f make.sh ] || err "Could not find file \`make.sh'."

host_os=`uname -o`

./make.sh distclean

[ -d out ]   && err "Directory \`out' exists after distclean."
[ -d build ] && err "Directory \`build' exists after distclean."

# cc + make

BUILDTYPE=
CC=cc
MAKE=make
BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

BUILDTYPE=release
CC=cc
MAKE=make
BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

                  CC=cc MAKE=make ./make.sh
BUILDTYPE=release CC=cc MAKE=make ./make.sh

[ -d out ]   || err "Directory \`out' does not exist after build."
[ -d build ] || err "Directory \`build' does not exist after build."

if [ "$host_os" = "FreeBSD" ] ; then

  # clang34 + bmake

  BUILDTYPE=
  CC=clang34
  MAKE=bmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  BUILDTYPE=release
  CC=clang34
  MAKE=bmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  # gcc49 + gmake

  BUILDTYPE=
  CC=gcc49
  MAKE=gmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  BUILDTYPE=release
  CC=gcc49
  MAKE=gmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  # pcc + fmake

  BUILDTYPE=
  CC=pcc
  MAKE=fmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  BUILDTYPE=release
  CC=pcc
  MAKE=fmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  # clang39 + dmake

  BUILDTYPE=
  CC=clang39
  MAKE=dmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

  BUILDTYPE=release
  CC=clang39
  MAKE=dmake
  BUILDTYPE=$BUILDTYPE CC=$CC MAKE=$MAKE ./make.sh

fi
