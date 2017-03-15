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

flags = []
longopts = []
varcount = 0
targets_requested = []

debug = False # don't change this value -- use debug flag '-d'.

if __name__ == '__main__':

    usage = 'Usage: ' + sys.argv[0] \
        + ' [-h] | [-d]' \
        + ' | [<variable>=<value> ...] [<target> ...]'

    def improper_usage ():

        sys.stderr.write(usage + '\n')
        exit(1)

    for arg in sys.argv[1:]:

        if arg[0:1] == '--':

            if varcount or len(targets_requested):

                improper_usage()

            longopts.append(arg)

        elif arg[0] == '-':

            if varcount or len(targets_requested):

                improper_usage()

            flags.append(arg)

        elif '=' in arg:

            if len(targets_requested):

                improper_usage()

            k, v = arg.split('=', 1)

            os.environ[k] = v
            varcount += 1

        else:

            targets_requested.append(arg)

    if '-h' in flags or '--help' in longopts:

        print(usage)
        exit(0)

    if '-d' in flags or '--debug' in longopts:

        debug = True

from state.projectstate import ProjectState
from state.buildstate import BuildState
import project as p

curr_state_project = ProjectState(p.__file__,
    p.project_name, p.project_name_prefix, p.project_directory, p.get_targets)

if debug:

    curr_state_project.targets.write_all_recdeps_pretty(sys.stderr)

    sys.stderr.write('\n')

builddir = None
outdir = None

home = os.path.expanduser("~")
subdir = p.project_name_prefix + '-' + p.project_name
try_builddir = os.path.join(home, 'build', subdir)
tb_allow = os.path.join(home, 'build', '.allow_from_any')
try_outdir = os.path.join(home, 'out', subdir)
to_allow = os.path.join(home, 'out', '.allow_from_any')

if os.path.isfile(tb_allow):

    builddir = try_builddir

else:

    builddir = os.path.join(curr_state_project.project_directory, 'build')

if debug:

    sys.stderr.write("Using builddir `" + builddir + "'.\n")

if os.path.isfile(to_allow):

    outdir = try_outdir

else:

    outdir = os.path.join(curr_state_project.project_directory, 'out')

if debug:

    sys.stderr.write("Using outdir `" + outdir + "'.\n\n")

curr_state_build = BuildState(curr_state_project, os.environ, builddir, outdir)

#if :
#
#    prev_state_build
#
#    if curr_state_build.equal(prev_state_build):
#
#        sys.stderr.write(sys.argv[0] + ': Nothing to be done for "' +)

if debug:

    sys.stderr.write(repr(curr_state_build) + '\n\n')

if __name__ == '__main__':

    raise NotImplementedError
