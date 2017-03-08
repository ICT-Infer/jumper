#!/usr/bin/env sh

if [ -z "$MAKE" ] ; then
  MAKE=make
fi
if [ -z "$CC" ] ; then
  CC=cc
fi

if [ -f Makefile ] ; then
  ./create_makefile.py Makefile.new || exit 1
  cmp Makefile Makefile.new
  if [ "$?" -ne "0" ] ; then
    echo "Makefile updated" 1>&2
    mv Makefile.new Makefile
  else
    rm Makefile.new
  fi
else
  ./create_makefile.py Makefile || exit 1
  echo "Makefile updated" 1>&2
fi

CC="$CC" ${MAKE} $@
