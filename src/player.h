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

#include "units.h"

typedef enum _actions_performable
{
	WALK_POSITIVE,
	WALK_NEGATIVE,
	RUN_POSITIVE,
	RUN_NEGATIVE,

	JUMP
} actions_performable;

typedef struct _player
{
	// Velocity
	nanometers_per_second vx;
	nanometers_per_second vy;
	nanometers_per_second vz;

	// Heading
	nanometers hx;
	nanometers hy;
	nanometers hz;

	micrograms mass;

	/*
	 * XXX: Array length for actions active corresponds to
	 *      number of actions performable after the fact
	 *      that some of the options are mutually exclusive
	 *      has been taken into account.
	 */
	actions_performable actions_active[3];
} player;

player player_defaults = {0, 0, 0, 1000000000, 0, 0, 80, 0};
