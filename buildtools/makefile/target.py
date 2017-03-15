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

import os
from makefile.rule import Rule

class Targets (list):

    def add (self, target):

        super(Targets, self).append(target)

        return target

class RecursiveDeps:

    def __init__ (self, target, recdeps):

        self.target = target
        self.recdeps = recdeps

    def __repr__ (self):

        return type(self).__name__ + '(' + repr(self.target) + ', ' \
            + repr(self.recdeps) + ')'

def write_recdeps_pretty (f, recdeps, depth = 0):

    f.write(' ' * (2 * depth) + repr(recdeps.target))

    if not(recdeps.target.fname is None):

        f.write(', fname = ' + recdeps.target.fname)

    f.write('\n')

    for recdep in recdeps.recdeps:

        write_recdeps_pretty(f, recdep, depth + 1)

class Target:

    def __init__ (self, name, rule=None):

        self.name = name

        self.fname = name

        self._direct_deps = []

        self._recdeps = RecursiveDeps(self, [])

        self._recdeps_list_stale = False

        if rule is None:

            self.rule = Rule(self)

        else:

            self.rule = rule

    def __str__ (self):

        return self.name

    def __repr__ (self):

        return type(self).__name__ + "('" + self.name + "')"

    def add_direct_dep (self, dep):

        self._direct_deps.append(dep)

        self._recdeps_list_stale = True

        return dep

    def get_deps_recursive (self):

        if self._recdeps_list_stale:

            recdeps = []

            for dep in self._direct_deps:

                recdeps.append(dep.get_deps_recursive())

            self._recdeps.recdeps = recdeps

        return self._recdeps

    def is_out_of_date (self):

        if not(os.path.isfile(self.fname)):

            return True

        for dep in self.get_deps_recursive():

            if dep.is_out_of_date():

                return True

            if dep.last_modified() >= self.last_modified():

                return True

        return False

class Phony (Target):

    def __init__ (self, name, rule=None):

        super(Phony, self).__init__(name, rule)

        self.fname = None

    def is_out_of_date (self):

        return True

class NoArch (Target):

    def __init__ (self, name, rule=None):

        super(NoArch, self).__init__(name, rule)

        self.fname = os.path.join('no-arch', name)

class ShareFile (NoArch):

    def __init__ (self, name, rule=None):

        super(ShareFile, self).__init__(name, rule)

        self.fname = os.path.join('share', name)

class Arch (Target):

    # XXX: Triplet is inserted into fname at the BuildState stage, not here.

    pass

class BinFile (Arch):

    def __init__ (self, name, rule=None):

        super(BinFile, self).__init__(name, rule)

        self.fname = os.path.join('bin', name)
