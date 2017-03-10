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

debug = False

def invalid_set_of_args ():

    sys.stderr.write('Usage: ' + sys.argv[0] + ' -d | <Makefile>\n')
    sys.exit(1)

if len(sys.argv) != 2:

    invalid_set_of_args()

makefile = None

if sys.argv[1] == '-':

    makefile = sys.stdout

elif sys.argv[1][0] != '-':

    makefile = open(sys.argv[1], 'w')

else:

    if sys.argv[1] == '-d':

        debug = True

    else:

        invalid_set_of_args()

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

    def get_all_deps (self, cur_depth = 0):

        deps = []

        for fref in self.direct_deps + self.directly_associated:

            deps.append(str(fref))

            if isinstance(fref, Source):

                subdeps = fref.get_all_deps(cur_depth + 1)

                for subdep in subdeps:

                    if not(subdep in deps):

                        deps.append(subdep)

        return deps

class Source (FileRef):

    def __init__ (self, fname, search_assoc = None):

        super(Source, self).__init__(fname)

        self.search_assoc = search_assoc

        self.directly_associated_fnames = []

        if not(search_assoc is None):

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

                fname = os.path.join('src', line[10:-2])

                local_inc_fnames.append(fname)

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

    def mfstr (self):

        mf_ofile = str(self)
        mf_deps = ' '.join([str(dep) for dep in self.get_all_deps()])
        mf_ddeps = ' '.join([str(ddep) for ddep in self.direct_deps])

        return \
"""
$(OBJDIR)/{}: {}
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c {} -o $@
""".format(mf_ofile, mf_deps, mf_ddeps)

class Gennedfile (Buildable):

    def __init__ (self, fname, creator):

        super(Gennedfile, self).__init__(fname)

        self.creator = creator

    def mfstr (self):

        return \
"""
$(SHAREDIR)/{}: {}
	mkdir -p $(SHAREDIR)/{}
	./{} $@
""".format(str(self), creator, os.path.dirname(str(self)), creator)

def genfile (creator, subpath, stem, tgtext):

    return Gennedfile(os.path.join(subpath, stem + tgtext), creator)

project = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
projprefix = 'github-eriknstr'

cfiles = []
hfiles = []

dir = 'src'
for file in os.listdir(dir):

    f = os.path.join(dir, file)

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

mf_ofiles = ' '.join([os.path.join('$(OBJDIR)',str(ofile))
    for ofile in ofiles])

gennedfiles = []

subpath = 'img'
srcpath = os.path.join('assets', 'gen', subpath)
for file in os.listdir(srcpath):

    creator = os.path.join(srcpath, file)
    stem, srcext = os.path.splitext(file)
    gennedfiles.append(genfile(creator, subpath, stem, '.png'))

gennedfiles.sort()

mf_gennedfiles = ' '.join([str(gennedfile) for gennedfile in gennedfiles])

if debug:

    sys.stderr.write('No current debug info to show.\n')

    sys.exit(0)

makefile.write('#\n')

with open('LICENSE', 'r') as flic:

    for line in flic:

        makefile.write('#' + (' ' if (len(line) > 1) else '') + line)

makefile.write('#\n')

makefile.write(
"""
PROJECT = {}
PROJUID = {}-$(PROJECT)

MAKE != sh -c '[ ! -z "$(MAKE)" ] && echo "$(MAKE)" || echo "make"'
CC != sh -c '[ ! -z "$(CC)" ] && echo "$(CC)" || echo "cc"'

CFLAGS = -Wall -Werror -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS = -lSDL2 -lSDL2_image -lm

HOST_TRIPLET != sh -c '$(CC) -dumpmachine || echo $$(uname -m)-unknown-$$(uname -s)'

TARGET_TRIPLET != sh -c '[ ! -z "$(TARGET_TRIPLET)" ] && echo "$(TARGET_TRIPLET)" || echo "$(HOST_TRIPLET)"'

BUILDTYPE != sh -c '[ ! -z "$(BUILDTYPE)" ] && echo "$(BUILDTYPE)" || echo "debug"'
DEBUG != sh -c '[ "$(BUILDTYPE)" = "debug" ] && echo "-g -O0 -DDEBUG" || echo "-O2"'
CFLAGS += $(DEBUG)

CONFIGID = $(HOST_TRIPLET)/$(MAKE)+$(CC)/$(BUILDTYPE)/$(TARGET_TRIPLET)

OUT != sh -c '[ ! -z "$(OUT)" ] && echo "$(OUT)" || echo "out"'
RELAOUTDIR = $(PROJUID)/$(CONFIGID)
OUTDIR = $(OUT)/$(RELAOUTDIR)
RELABINDIR = $(RELAOUTDIR)/bin
BINDIR = $(OUT)/$(RELABINDIR)

RELASHAREDIR = $(PROJUID)/no-arch/share/$(PROJECT)
SHAREDIR = $(OUT)/$(RELASHAREDIR)
DYNASTS = LICENSE {}
DYNASTS != sh -c "echo '$(DYNASTS)' | sed 's@[^ ]\{{1,\}}@$(SHAREDIR)/&@g'"

BUILDS != sh -c '[ ! -z "$(BUILDS)" ] && echo "$(BUILDS)" || echo "build"'
RELAOBJDIR = $(PROJUID)/$(CONFIGID)
OBJDIR = $(BUILDS)/$(RELAOBJDIR)
OBJS = {}

MAIN_EXECUTABLE = $(BINDIR)/$(PROJECT)

all: $(MAIN_EXECUTABLE) $(DYNASTS)

$(SHAREDIR)/LICENSE: LICENSE
	mkdir -p $(SHAREDIR)
	cp $> $^ $@
""".format(project, projprefix, mf_gennedfiles, mf_ofiles))

for tgt in gennedfiles + ofiles:

    makefile.write(tgt.mfstr())

makefile.write(
"""
$(MAIN_EXECUTABLE): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $> $^ -o $@ $(LDLIBS)

.PHONY: clean
clean:
	for obj in $(OBJS) ; do test -f $$obj && rm $$obj || true ; done
	test -d $(OBJDIR) && cd $(BUILDS) && rmdir -p $(RELAOBJDIR) || true

.PHONY: distclean_triplet
distclean_triplet: clean
	test -f $(MAIN_EXECUTABLE) && rm $(MAIN_EXECUTABLE) || true
	test -d $(BINDIR) && cd $(OUT) && rmdir -p $(RELABINDIR) || true

.PHONY: distclean
distclean:
	test -d $(BUILDS)/$(PROJUID) && cd $(BUILDS) && rm -rf $(PROJUID) || true
	test -d $(OUT)/$(PROJUID) && cd $(OUT) && rm -rf $(PROJUID) || true

.PHONY: run
run: $(MAIN_EXECUTABLE) $(DYNASTS)
	cd $(SHAREDIR) && $(MAIN_EXECUTABLE)

.PHONY: targets
""")

makefile.close()
