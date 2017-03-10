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

#ifndef _JUMPER_WORLD_H
#define _JUMPER_WORLD_H

#include "units.h"

typedef enum _timeframe
{
	WTF_MINI,
	WTF_EXTENDED
} timeframe;

typedef struct _world
{
	// World coordinate-system
	axes3d wc;

	timeframe tf;

	union
	{
		gfx_frames_mini     mgfxf;
		gfx_frames_extended egfxf;
	};

	union
	{
		sim_ticks_mini     msimt;
		sim_ticks_extended esimt;
	};
} world;

void init_world (world *, timeframe tf);

#endif
