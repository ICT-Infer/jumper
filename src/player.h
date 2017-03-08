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

#ifndef _JUMPER_PLAYER_H
#define _JUMPER_PLAYER_H

#include "units.h"

typedef enum _actions_performable
{
	AC_NONE = 0,

	AC_WALK_POSITIVE,
	AC_WALK_NEGATIVE,
	AC_RUN_POSITIVE,
	AC_RUN_NEGATIVE,

	AC_JUMP
} actions_performable;

typedef struct _player
{
	// Position in parent coordinate system
	position3d pos;

	// Velocity per second in parent coordinate system
	velocity3d vel;

	// Local coordinate-system orientation in parent coordinate system
	axes3d lco;

	micrograms mass;

	/*
	 * XXX: Array length for actions active corresponds to
	 *      number of actions performable after the fact
	 *      that some of the options are mutually exclusive
	 *      has been taken into account.
	 */
	actions_performable actions_active[3];
} player;

extern const player PLAYER_DEFAULTS;

#endif
