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
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#include "mesh3d.h"
#include "obj.h"
#include "textui.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#include "crit.h"

#include "timedelta.inc"

static inline void seventyfive (
	SDL_Rect * rect, float texratio, int w, int h)
{
	float wratio = ((float) w) / h;

	if (wratio < texratio)
	{
		rect->w = 0.75 * w;
		rect->h = rect->w / texratio;
	}
	else
	{
		rect->h = 0.75 * h;
		rect->w = texratio * rect->h;
	}

	rect->x = (w - rect->w) / 2;
	rect->y = (h - rect->h) / 2;
}

int main (int argc, char *argv[])
{
#define CLEANUP quit_main

	// Status
	int exits = EXIT_SUCCESS;

	struct timespec tspec;
	nanoseconds tprev, tcurr, tdiff, tstart, tblink;
	bool tbstate;

#ifdef DEBUG
	fprintf(stderr, "This is a DEBUG build.\n\n");

	char cwd[PATH_MAX];
	CRIT(!getcwd(cwd, sizeof(cwd)), "get current working dir")

	fprintf(stderr, "Current working directory: %s.\n", cwd);
#endif

	CRIT(GETTIME(&tspec), "read clock")
	tcurr = get_ns(tspec);

	int fw, fh, fx, fy; // fullscreen
	int bt = 0, bl = 0, bb = 0, br = 0; // borders
	int w, h, x, y; // window

	CRIT_TTF(TTF_Init(), "initialize TTF")

#define CLEANUP cleanup_ttfinit

	TTF_Font * msgfont;
	CRIT_TTF(!(msgfont =
			TTF_OpenFont("fonts/open-sans/OpenSans-Regular.ttf",
				96)), "open font")

	CRIT_SDL(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER), "initialize SDL")

#define CLEANUP cleanup_sdlinit

	SDL_Window * win;
	CRIT_SDL(!(win = SDL_CreateWindow("jumper",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_FULLSCREEN_DESKTOP |
			SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE )),
		"create SDL window")

#define CLEANUP cleanup_sdlwindow

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

	CRIT_SDL(SDL_GetWindowBordersSize(win, &bt, &bl, &bb, &br),
		"get SDL window size")

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
	CRIT_SDL(!(rend = SDL_CreateRenderer(win, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
		"create SDL renderer")

#define CLEANUP cleanup_sdlrenderer

	SDL_Color black = {0, 0, 0};

	SDL_Surface * splashsurf;
	CRIT_SDL(!(splashsurf = IMG_Load("img/splash.png")),
		"create SDL surface")

	SDL_Texture * splashtex =
		SDL_CreateTextureFromSurface(rend, splashsurf);
	SDL_FreeSurface(splashsurf);
	CRIT_SDL(!splashtex, "create SDL texture")

#define CLEANUP cleanup_sdlsplash

	int stw, sth;
	SDL_QueryTexture(splashtex, NULL, NULL, &stw, &sth);
	float stratio = ((float) stw) / sth;

	SDL_Rect splashrect;

	seventyfive(&splashrect, stratio, w, h);

	SDL_Surface * msgsurf;
	CRIT_TTF(!(msgsurf = TTF_RenderText_Solid(msgfont, "LOADING", black)),
		"create SDL surface")

	SDL_Texture * msgtex = SDL_CreateTextureFromSurface(rend, msgsurf);
	SDL_FreeSurface(msgsurf);
	CRIT_SDL(!msgtex, "create SDL texture")

#define CLEANUP cleanup_sdlmsg

	int mtw, mth;
	SDL_QueryTexture(msgtex, NULL, NULL, &mtw, &mth);

#ifdef DEBUG
	fprintf(stderr, "msgtex: %d x %d\n", mtw, mth);
#endif

	//float mtratio = ((float) mtw) / mth;

	SDL_Rect msgrect = {0, 0, mtw, mth};

	//msgpos(&splashrect, mtw, mth, w, h);

	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_ShowWindow(win);
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, splashtex, NULL, &splashrect);
	SDL_RenderCopy(rend, msgtex, NULL, &msgrect);
	SDL_RenderPresent(rend);

	MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr)
#ifdef DEBUG
	fprintf(stderr, "Window ready after %" JPR_NS " ns.\n", tdiff);
#endif

	SDL_Delay(800);

	const char levelname[] = "Level 1";

	mesh3d levelmesh;
	if (loadmesh3d(&levelmesh, "meshes/level1"))
	{
		fprintf(stderr, "Failed to load mesh.\n");
		exits = EXIT_FAILURE;
		goto CLEANUP;
	}

	world level;
	init_world(&level, WTF_MINI);

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
	MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr)
#ifdef DEBUG
	fprintf(stderr, "About to enter first main loop "
			"after %" JPR_NS " ns.\n", tdiff);
#endif

	bool quit = false;
	SDL_Event evt;

main_menu:

#ifdef DEBUG
	fprintf(stderr, "Main menu.\n");
#endif

	MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr)
	tstart = tcurr;
	tdiff = 0;
	tblink = 0;
	tbstate = false;

	while (!quit)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (evt.type == SDL_JOYBUTTONUP ||
				evt.type == SDL_FINGERUP ||
				evt.type == SDL_KEYUP ||
				evt.type == SDL_MOUSEBUTTONUP)
			{
				goto here_we_go;
			}
		}

		SDL_RenderClear(rend);

		SDL_GetWindowSize(win, &w, &h);
		seventyfive(&splashrect, stratio, w, h);

		SDL_RenderCopy(rend, splashtex, NULL, &splashrect);

		tblink += tdiff;
		if (tblink >= INV_NANO / 2)
		{
			tblink = 0;

			if ((tbstate = !tbstate))
			{
				// Show
			}
			else
			{
				// Hide
			}
		}

		SDL_RenderPresent(rend);

		MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr)

/*
#ifdef DEBUG
	fprintf(stderr, "Iteration completed in %" JPR_NS " ns.\n", tdiff);
#endif
*/
	}

here_we_go:

#ifdef DEBUG
	fprintf(stderr, "Here we go!\n");
#endif

	MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr)
	tstart = tcurr;
	tdiff = 0;

	while (!quit)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (evt.type == SDL_JOYBUTTONUP ||
				evt.type == SDL_FINGERUP ||
				evt.type == SDL_KEYUP ||
				evt.type == SDL_MOUSEBUTTONUP)
			{
				goto main_menu;
			}
		}

		// TODO: Register and act upon collisions

		// TODO: step_simulation(&sim, tstart, tcurr, tdiff);

		SDL_RenderClear(rend);

		SDL_GetWindowSize(win, &w, &h);

		// TODO: Render game frame

		SDL_RenderPresent(rend);

		MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr)
#ifdef DEBUG
	fprintf(stderr, "Iteration completed in %" JPR_NS " ns.\n", tdiff);
#endif
	}

cleanup_sdlmsg:

	SDL_DestroyTexture(msgtex);

cleanup_sdlsplash:

	SDL_DestroyTexture(splashtex);

cleanup_sdlrenderer:

	SDL_DestroyRenderer(rend);

cleanup_sdlwindow:

	SDL_DestroyWindow(win);

cleanup_sdlinit:

	SDL_Quit();

cleanup_ttfinit:

	TTF_Quit();

quit_main:

	return exits;
}
