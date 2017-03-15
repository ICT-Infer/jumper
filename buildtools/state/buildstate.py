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

class BuildState ():

    def __init__ (self, project_state, env, builddir, outdir):

        self.project_state = project_state
        self.env = env

        cwd_saved = os.getcwd()

        os.chdir(project_state.project_directory)

        self.builddir = os.path.realpath(builddir)
        self.outdir = os.path.realpath(outdir)

        os.chdir(cwd_saved)

    def __repr__ (self):

        ps_repr = self.project_state.__repr__()
        env_repr = self.env.__repr__()

        return type(self).__name__ + "(" + ps_repr + ", " \
            + env_repr + ", '" + self.builddir + "', '" + self.outdir + "')"
