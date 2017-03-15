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

targets_requested = []

if __name__ == '__main__':

    usage = 'Usage: ' + sys.argv[0] \
        + ' [<variable>=<value> ...] [<target> ...]'

    for arg in sys.argv:

        if arg == '-h' or arg == '--help':

            print(usage)
            exit(0)

    for arg in sys.argv[1:]:

        if '=' in arg:

            if len(targets_requested):

                sys.stderr.write(usage + '\n')
                exit(1)

            k, v = arg.split('=', 1)
            print(v)

            os.environ[k] = v

        else:

            targets_requested.append(arg)

from state.projectstate import ProjectState
from state.buildstate import BuildState
import project as p

curr_state_project = ProjectState(p.__file__,
    p.project_name, p.project_name_prefix, p.project_directory, p.get_targets)

builddir = 'build'
outdir = 'out'

home = os.path.expanduser("~")
try_builddir = os.path.join(home, 'build')
tb_allow = os.path.join(try_builddir, '.allow_from_any')
try_outdir = os.path.join(home, 'out')
to_allow = os.path.join(try_outdir, '.allow_from_any')

if os.path.isdir(try_builddir) and os.path.isfile(tb_allow):

    builddir = try_builddir

if __name__ == '__main__':

    sys.stderr.write("Using builddir `" + builddir + "'.\n")

if os.path.isdir(try_outdir) and os.path.isfile(to_allow):

    outdir = try_outdir

if __name__ == '__main__':

    sys.stderr.write("Using outdir `" + outdir + "'.\n")

curr_state_build = BuildState(curr_state_project, os.environ, builddir, outdir)

#if :
#
#    prev_state_build
#
#    if curr_state_build.equal(prev_state_build):
#
#        sys.stderr.write(sys.argv[0] + ': Nothing to be done for "' +)

sys.stderr.write(curr_state_build.__repr__() + '\n')

raise NotImplementedError
