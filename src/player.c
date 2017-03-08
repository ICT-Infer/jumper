/*
 * Copyright (c) 2017 Erik Nordstrøm <erik@nordstroem.no>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "player.h"

const player PLAYER_DEFAULTS =
{
	/*            pos */ {       0,       0,        0 },
	/*            vel */ {       0,       0,        0 },
	/*            lco */
	{
		/*  xaxis */ { 1 * 1e9,       0,        0 },
		/*  yaxis */ {       0, 1 * 1e9,        0 },
		/*  zaxis */ {       0,       0,  1 * 1e9 }
	},
	/*           mass */                     80 * 1e6  ,
	/* actions_active */ { AC_NONE, AC_NONE,  AC_NONE },
};
