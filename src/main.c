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
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "mesh3d.h"
#include "obj.h"
#include "textui.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main (int argc, char *argv[])
{
	// Status
	int exits = EXIT_SUCCESS;
	int ops = 0;

	int fw, fh, fx, fy; // fullscreen
	int bt = 0, bl = 0, bb = 0, br = 0; // borders
	int w, h, x, y; // window

#ifdef DEBUG
	fprintf(stderr, "This is a DEBUG build.\n\n");
#endif

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER))
	{
		fprintf(stderr, "Failed to initialize SDL. Error: %s.\n",
			SDL_GetError());
		exits = EXIT_FAILURE;
		goto quit_main;
	}

	SDL_Window * win;
	if (!(win = SDL_CreateWindow("jumper",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_HIDDEN)))
	{
		fprintf(stderr, "Failed to create SDL window. Error: %s.\n",
			SDL_GetError());
		exits = EXIT_FAILURE;
		goto cleanup_sdlinit;
	}

	SDL_GetWindowSize(win, &fw, &fh);
	SDL_GetWindowPosition(win, &fx, &fy);

#ifdef DEBUG
	fprintf(stderr, "Fullscreen: w = %d, h = %d, x = %d, y = %d.\n",
		fw, fh, fx, fy);
#endif

	w = 0.9 * fw;
	h = 0.9 * fh;

	SDL_SetWindowFullscreen(win, 0);
	SDL_SetWindowSize(win, w, h);

	if (SDL_GetWindowBordersSize(win, &bt, &bl, &bb, &br))
	{
		fprintf(stderr, "WARN: Failed get SDL window size. "
				"Error: %s.\n", SDL_GetError());
	}

	if (!bt)
	{
		/*
		 * Assume some reasonable values. Note: User might
		 * actually just have window decorations turned off.
		 */

		bt = 24; bl = 5; bb = 5; br = 5;
	}

	x = fx + 0.05 * (fw - (bl + br));
	y = fy + 0.05 * (fh - (bt + bb));

	SDL_SetWindowPosition(win, x, y);

#ifdef DEBUG
	fprintf(stderr, "Window: w = %d, h = %d, "
			"bt = %d, bl = %d, bb = %d, br = %d, "
			"x = %d, y = %d.\n",
		w, h, bt, bl, bb, br, x, y);
#endif

	SDL_Renderer * rend;
	if (!(rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC)))
	{
		fprintf(stderr, "Failed to create SDL renderer. Error: %s.\n",
			SDL_GetError());
		exits = EXIT_FAILURE;
		goto cleanup_sdlwindow;
	}

	SDL_Surface * surface;
	if (!(surface = IMG_Load("img/splash.png")))
	{
		fprintf(stderr, "Failed to create SDL surface. Error: %s.\n",
			SDL_GetError());
		exits = EXIT_FAILURE;
		goto cleanup_sdlrenderer;
	}

	SDL_Texture * splashtex = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	if (!splashtex)
	{
		fprintf(stderr, "Failed to create SDL texture. Error: %s.\n",
			SDL_GetError());
		exits = EXIT_FAILURE;
		goto cleanup_sdlrenderer;
	}

	SDL_Rect splashrect;

	{
		int stw, sth;
		SDL_QueryTexture(splashtex, NULL, NULL, &stw, &sth);
		float stratio = ((float) stw) / sth;

		if (stratio > 1)
		{
			splashrect.w = 0.75 * w;
			splashrect.h = splashrect.w / stratio;
		}
		else
		{
			splashrect.h = 0.75 * h;
			splashrect.w = stratio * splashrect.h;
		}
	}

	splashrect.x = (w - splashrect.w) / 2;
	splashrect.y = (h - splashrect.h) / 2;

	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_ShowWindow(win);
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, splashtex, NULL, &splashrect);
	SDL_RenderPresent(rend);

#ifdef DEBUG
	fprintf(stderr, "Window ready.\n");
#endif

	SDL_Delay(800);

	const char levelname[] = "Level 1";

	mesh3d levelmesh;
	if ((ops = loadmesh3d(&levelmesh, "meshes/level1")))
	{
		fprintf(stderr, "Failed to load mesh.\n");
		exits = EXIT_FAILURE;
		SDL_DestroyTexture(splashtex);
		goto cleanup_sdlrenderer;
	}

	world level;
	init_world(&level, W_MINI);

	obj levelobj;
	def_obj(NULL, &levelobj, levelname, OT_WORLD,
		&levelmesh, (objval) &level);

#ifdef DEBUG
	tabulate_properties(stderr, &levelobj);
	//fprintf(stderr, "\n");
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

	SDL_DestroyTexture(splashtex);

#ifdef DEBUG
	fprintf(stderr, "Splash screen finished.\n");
#endif

/*
	while (1)
	{
		// TODO: goto cleanup_sdlwindow;
	}
*/

cleanup_sdlrenderer:

	SDL_DestroyRenderer(rend);

cleanup_sdlwindow:

	SDL_DestroyWindow(win);

cleanup_sdlinit:

	SDL_Quit();

quit_main:

	return exits;
}
