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

#ifndef _JUMPER_OBJ_H
#define _JUMPER_OBJ_H

#include "mesh3d.h"
#include "player.h"
#include "world.h"

typedef enum _objtag
{
	OT_WORLD,
	OT_PLAYER
} objtag;

typedef union _objval
{
	world * const w;
	player * const p;
} objval;

typedef struct _obj
{
	struct _obj * parent;
	char * name;
	objtag tag;
	mesh3d mesh;
	objval val;
} obj;

void def_obj (obj *, obj *, const char *, objtag, mesh3d *, objval);

#endif
