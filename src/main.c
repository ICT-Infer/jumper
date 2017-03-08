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

#include "units.h"
#include "player.h"

int main (int argc, char *argv[])
{
#ifdef DEBUG
	fprintf(stderr, "This is a DEBUG build.\n\n");
#endif

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Failed to initialize SDL. Error: %s\n",
			SDL_GetError());
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG
	fprintf
	(
		stderr,
		"Speed of light: c = %lu * 1e-9 m/s\n"
		"\n"
		"Player defaults\n"
		"---------------\n"
		"Velocity x: %lu * 1e-9 m/s\n"
		"Velocity y: %lu * 1e-9 m/s\n"
		"Velocity z: %lu * 1e-9 m/s\n"
		"Pointing x:  %lu * 1e-9 m\n"
		"Pointing y:  %lu * 1e-9 m\n"
		"Pointing z:  %lu * 1e-9 m\n"
		"Mass:       %lu * 1e-9 kg\n"
		,SPEED_OF_LIGHT
		,PLAYER_DEFAULTS.vx
		,PLAYER_DEFAULTS.vy
		,PLAYER_DEFAULTS.vz
		,PLAYER_DEFAULTS.px
		,PLAYER_DEFAULTS.py
		,PLAYER_DEFAULTS.pz
		,PLAYER_DEFAULTS.mass * 1000
	);
#endif

	SDL_Quit();

	return EXIT_SUCCESS;
}
