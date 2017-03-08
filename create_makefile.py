#!/usr/bin/env python3

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

import sys, os

class FileRef:

    def __init__ (self, fname, direct_deps = [], directly_associated = []):

        self.fname = fname
        self.direct_deps = direct_deps
        self.directly_associated = directly_associated

    def __str__ (self):

        return self.fname

    def __repr__ (self):

        return type(self).__name__ + "('" + self.fname + "')"

    def __lt__ (self, other):

        return self.fname < str(other)

    def __eq__ (self, other):

        return self.fname == str(other)

    def get_all_deps (self):

        deps = []

        for fref in self.direct_deps + self.directly_associated:

            deps.append(str(fref))

            if isinstance(fref, Source):

                deps += fref.get_all_deps()

        return deps

class Source (FileRef):

    def __init__ (self, fname, search_assoc = None):

        super(Source, self).__init__(fname)

        self.search_assoc = search_assoc

        self.directly_associated_fnames = []

        if search_assoc:

            self.directly_associated_fnames = \
                self.get_directly_associated_fnames()

    def refresh_directly_associated (self):

        directly_associated = []

        for fname in self.directly_associated_fnames:

            directly_associated.append( \
                self.search_assoc[self.search_assoc.index(fname)])

        self.directly_associated = directly_associated

def c_style_local_include_fnames (self):

    local_inc_fnames = []

    with open(str(self), 'r') as f:

        for line in f:

            if line.startswith('#include "'):

                local_inc_fnames.append(os.path.join('src', line[10:-2]))

    local_inc_fnames.sort()

    return local_inc_fnames

class Cfile (Source):

    get_directly_associated_fnames = c_style_local_include_fnames

class Hfile (Source):
    
    get_directly_associated_fnames = c_style_local_include_fnames

class Buildable (FileRef):

    def __init__ (self, fname, direct_deps = []):

        super(Buildable, self).__init__(fname, direct_deps)

class Ofile (Buildable):

    pass

if len(sys.argv) != 2:

    sys.stderr.write('Usage: ' + sys.argv[0] + ' <BSDmakefile>\n')
    sys.exit(1)

project = os.path.basename(os.path.dirname(os.path.realpath(__file__)))

makefile = sys.stdout

if sys.argv[1] != '-':

    makefile = open(sys.argv[1], 'w')

cfiles = []
hfiles = []

for file in os.listdir('src'):

    f = os.path.join('src', file)

    if file.endswith('.c'):

        cfiles.append(Cfile(f, hfiles))

    elif file.endswith('.h'):

        hfiles.append(Hfile(f, hfiles))

cfiles.sort()
hfiles.sort()

for fref in cfiles + hfiles:

    fref.refresh_directly_associated()

ofiles = []

for cfile in cfiles:

    ofile = Ofile(os.path.splitext(os.path.basename(str(cfile)))[0] + '.o',
        [cfile])

    ofiles.append(ofile)

makefile.write('#\n')

with open('LICENSE', 'r') as flic:

    for line in flic:

        makefile.write('#' + (' ' if (len(line) > 1) else '') + line)

makefile.write('#\n')

makefile.write(
"""
PROJECT = {}

MAKE != sh -c '[ ! -z "$(MAKE)" ] && echo "$(MAKE)" || echo "make"'
CC != sh -c '[ ! -z "$(CC)" ] && echo "$(CC)" || echo "cc"'

INCDIRS = /usr/local/include
INC != sh -c "echo '$(INCDIRS)' | sed 's@[^ ]\{{1,\}}@-I&@g'"
CFLAGS = -g -O2 $(INC)
LIBDIRS = /usr/local/lib
LDFLAGS != sh -c "echo '$(LIBDIRS)' | sed 's@[^ ]\{{1,\}}@-L&@g'"
LDLIBS = -lSDL2

HOST_TRIPLET != sh -c '$(CC) -dumpmachine || echo $$(uname -m)-unknown-$$(uname -s)'

TARGET_TRIPLET != sh -c '[ ! -z "$(TARGET_TRIPLET)" ] && echo "$(TARGET_TRIPLET)" || echo "$(HOST_TRIPLET)"'

BUILDTYPE != sh -c '[ ! -z "$(BUILDTYPE)" ] && echo "$(BUILDTYPE)" || echo "release"'

CONFIGLVL1 = $(HOST_TRIPLET)
CONFIGLVL2 = $(CONFIGLVL1)/$(MAKE)+$(CC)
CONFIGLVL3 = $(CONFIGLVL2)/$(BUILDTYPE)
CONFIGLVL4 = $(CONFIGLVL3)/$(TARGET_TRIPLET)
CONFIGID = $(CONFIGLVL4)

OUTDIR = out/$(CONFIGID)
BINDIR = $(OUTDIR)/bin

BUILDS = build
OBJS = {}
OBJDIR = $(BUILDS)/$(CONFIGID)
OBJS != sh -c "echo '$(OBJS)' | sed 's@[^ ]\{{1,\}}@$(OBJDIR)/&@g'"

all: $(BINDIR)/$(PROJECT)
""".format(project, ' '.join([str(ofile) for ofile in ofiles])))

for ofile in ofiles:

    makefile.write(
"""
$(OBJDIR)/{}: {}
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c {} -o $@
""".format(str(ofile),
    ' '.join([str(dep) for dep in ofile.get_all_deps()]),
    ' '.join([str(ddep) for ddep in ofile.direct_deps])))

makefile.write(
"""
$(BINDIR)/$(PROJECT): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $> $^ -o $@ $(LDLIBS)

.PHONY: clean
clean:
	for obj in $(OBJS) ; do test -f $$obj && rm $$obj || true ; done
	test -d $(OBJDIR) && rmdir $(OBJDIR) || true

.PHONY: distclean_triplet
distclean_triplet: clean
	test -f $(BINDIR)/$(PROJECT) && rm $(BINDIR)/$(PROJECT) || true
	test -d $(BINDIR) && rmdir $(BINDIR) || true
	test -d $(OUTDIR) && rmdir $(OUTDIR) || true

.PHONY: distclean
distclean:
	rm -rf $(BUILDS)
	rm -rf out

.PHONY: run
run:
	$(BINDIR)/$(PROJECT)

.PHONY: targets
""")

makefile.close()
