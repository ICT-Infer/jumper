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

class Targets (list):

    def add (self, target):

        super(Targets, self).append(target)

        return target

class Target:

    def __init__ (self, name):

        self.name = name

        self._direct_deps = []

        self._recursive_deps = { 'self': self, 'deps': [] }

        self._recursive_deps_list_stale = False

    def __str__ (self):

        return self.name

    def __repr__ (self):

        return type(self).__name__ + "('" + self.name + "')"

    def add_direct_dep (self, dep):

        self._direct_deps.append(dep)

        self._recursive_deps_list_stale = True

        return dep

    def get_deps_recursive (self):

        if self._recursive_deps_list_stale:

            deps_recursive = []

            for dep in self._direct_deps:

                deps_recursive.append(dep.get_deps_recursive())

            self._recursive_deps['deps'] = deps_recursive

        return self._recursive_deps

class Phony (Target):

    pass

class NoArch (Target):

    pass

class ShareFile (NoArch):

    pass

class Arch (Target):

    pass

class BinFile (Arch):

    pass
