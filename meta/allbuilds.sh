#!/usr/bin/env sh

#
# Copyright (c) 2017 Erik Nordstrøm <erik@nordstroem.no>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

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
  echo "CC=$CC" 1>&2
  echo "MAKE=$MAKE" 1>&2
  echo "BUILDTYPE=$BUILDTYPE" 1>&2
  echo "" 1>&2
}

trap most_recent_set_of_values EXIT

[ -f make.sh ] || err "Could not find file \`make.sh'."

host_os=`uname -o`

perform_build ()
{
  CC=$1
  MAKE=$2

  BUILDTYPE=
  CC=$CC MAKE=$MAKE BUILDTYPE=$BUILDTYPE ./make.sh distclean_triplet
  CC=$CC MAKE=$MAKE BUILDTYPE=$BUILDTYPE ./make.sh

  BUILDTYPE=release
  CC=$CC MAKE=$MAKE BUILDTYPE=$BUILDTYPE ./make.sh distclean_triplet
  CC=$CC MAKE=$MAKE BUILDTYPE=$BUILDTYPE ./make.sh
}

perform_build cc make

[ -d out ]   || err "Directory \`out' does not exist after build."
[ -d build ] || err "Directory \`build' does not exist after build."

if [ "$host_os" = "FreeBSD" ] ; then

  perform_build clang34 bmake

  perform_build   gcc49 gmake

  perform_build     pcc fmake

  perform_build clang39 dmake

fi
