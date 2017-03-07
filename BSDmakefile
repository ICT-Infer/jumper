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

PROJECT = jumper

INCDIRS = /usr/local/include
INC != sh -c "echo '${INCDIRS}' | sed 's@[^ ]\{1,\}@-I&@g'"
CFLAGS = -g -O2 $(INC)
LIBDIRS = /usr/local/lib
LDFLAGS != sh -c "echo '${LIBDIRS}' | sed 's@[^ ]\{1,\}@-L&@g'"
LDLIBS = -lSDL2

TRIPLET != $(CC) -dumpmachine
OUTDIR = out/$(TRIPLET)
BINDIR = $(OUTDIR)/bin
OBJS = main.o player.o world.o simulation.o toolassist.o
OBJDIR = build/$(TRIPLET)
OBJS != sh -c "echo '${OBJS}' | sed 's@[^ ]\{1,\}@${OBJDIR}/&@g'"

all: $(BINDIR)/$(PROJECT)

# https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=217613
$(OBJDIR)/main.o: src/main.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c src/main.c -o $(.TARGET)

$(OBJDIR)/player.o: src/player.c src/player.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c src/player.c -o $(.TARGET)

$(OBJDIR)/simulation.o: src/simulation.c src/simulation.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c src/simulation.c -o $(.TARGET)

$(OBJDIR)/toolassist.o: src/toolassist.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c src/toolassist.c -o $(.TARGET)

$(OBJDIR)/world.o: src/world.c src/world.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c src/world.c -o $(.TARGET)

$(BINDIR)/$(PROJECT): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $(.ALLSRC) -o $(.TARGET) $(LDLIBS)
