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

#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "mesh3d.h"
#include "obj.h"
#include "textui.h"

int main (int argc, char *argv[])
{
	// Status
	int exits = EXIT_SUCCESS;
	int ops = 0;

#ifdef DEBUG
	fprintf(stderr, "This is a DEBUG build.\n\n");
#endif

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Failed to initialize SDL. Error: %s.\n",
			SDL_GetError());
		exit(EXIT_FAILURE);
	}

	const char levelname[] = "Level 1";

	mesh3d levelmesh;
	if ((ops = loadmesh3d(&levelmesh, "meshes/level1")))
	{
		fprintf(stderr, "Failed to load mesh.\n");
		goto fail_main;
	}

	world level;
	init_world(&level, W_MINI);

	obj levelobj;
	def_obj(NULL, &levelobj, levelname, OT_WORLD,
		&levelmesh, (objval) &level);

#ifdef DEBUG
	tabulate_properties(stderr, &levelobj);
	//tabulate_properties(stderr, &player1obj);

/*
	fprintf
	(
		stderr,

		"Speed of light: c = %lu * 1e-9 m/s\n"
		"\n"

		"Player defaults\n"
		"===============\n"
		"Position x:                        %lu  [1e-9   m]\n"
		"Position y:                        %lu  [1e-9   m]\n"
		"Position z:                        %lu  [1e-9   m]\n"
		"---------------\n"
		"Velocity x:                        %lu  [1e-9 m/s]\n"
		"Velocity y:                        %lu  [1e-9 m/s]\n"
		"Velocity z:                        %lu  [1e-9 m/s]\n"
		"---------------\n"
		"Orientation xaxis: (%lu, %lu, %lu) [1e-9   m] x 3\n"
		"Orientation yaxis: (%lu, %lu, %lu) [1e-9   m] x 3\n"
		"Orientation zaxis: (%lu, %lu, %lu) [1e-9   m] x 3\n"
		"---------------\n"
		"Mass:                    %lu  [1e-9  kg]\n"
		"---------------\n"
		"Actions active:               (%u %u %u)\n"

		,SPEED_OF_LIGHT

		,PLAYER_DEFAULTS.pos.x
		,PLAYER_DEFAULTS.pos.y
		,PLAYER_DEFAULTS.pos.z
		,PLAYER_DEFAULTS.vel.x
		,PLAYER_DEFAULTS.vel.y
		,PLAYER_DEFAULTS.vel.z
		,PLAYER_DEFAULTS.lco.xaxis.x
		,PLAYER_DEFAULTS.lco.xaxis.y
		,PLAYER_DEFAULTS.lco.xaxis.z
		,PLAYER_DEFAULTS.lco.yaxis.x
		,PLAYER_DEFAULTS.lco.yaxis.y
		,PLAYER_DEFAULTS.lco.yaxis.z
		,PLAYER_DEFAULTS.lco.zaxis.x
		,PLAYER_DEFAULTS.lco.zaxis.y
		,PLAYER_DEFAULTS.lco.zaxis.z
		,PLAYER_DEFAULTS.mass * 1000
		,PLAYER_DEFAULTS.actions_active[0]
		,PLAYER_DEFAULTS.actions_active[1]
		,PLAYER_DEFAULTS.actions_active[2]
	);
*/
#endif

	goto normal_exit_main;

fail_main:
	exits = EXIT_FAILURE;

normal_exit_main:

	SDL_Quit();

	return exits;
}
