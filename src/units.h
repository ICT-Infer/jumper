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

#include <stdint.h>

/*
 *  2^8 =                  256
 * 2^16 =                65536
 * 2^32 =           4294967296
 * 2^64 = 18446744073709551616
 *
 * mega  1e6
 * kilo  1e3
 * milli 1e-3
 * micro 1e-6
 * nano  1e-9
 *
 */

// Derived SI units

/*
 * micro vs kilo unit - max unsigned values
 *
 * (2^32 * 1e-6 / 1e3) =              4.294967296 k
 * (2^64 * 1e-6 / 1e3) = 18 446 744 073.709551616 k
 *
 */
typedef uint64_t micrograms;

/*
 * nano vs base unit - max unsigned values
 *
 * (2^32 * 1e-9) =              4.294967296
 * (2^64 * 1e-9) = 18 446 744 073.709551616
 */
typedef uint64_t nanometers;
typedef uint64_t nanoseconds;
typedef uint64_t nanometers_per_second;

extern const nanometers_per_second SPEED_OF_LIGHT;

// Custom units

/*
 * Consider a punch to the face as the lowest amount of damage possible.
 * Then 2^8 = 256 repetitions or times that amount of damage would surely
 * be fatal for a regular person not wearing any form of armor.
 *
 * Furthermore, 2^16 = 65536 units of damage should be considered
 * a pretty solid amount of armor, and also not unheard of for
 * weapons/ammo intended to pierce armor to deal.
 *
 * Lastly, the upper ends of military grade armor and damage
 * should be able to withstand/deal damage above that amount.
 */
typedef  uint8_t creature_health;
typedef  uint8_t    basic_damage;
typedef uint16_t  standard_armor;
typedef uint16_t    heavy_damage;
typedef uint32_t   milspec_armor;
typedef uint32_t  milspec_damage;

// 256 lives ought to be enough for anyone.
typedef uint8_t lives;

/*
 * Say that the game graphics is rendering at 144 FPS, then
 *
 * 2^16 frames / (144 fps) ~=      455.11 seconds ~=  7.5 minutes
 * 2^32 frames / (144 fps) ~= 29826161.78 seconds ~= 11.3 months
 *
 * The amount of simulation FPS is likely to be set lower (15 FPS perhaps?)
 * but in case the combination of simulation simplicity and hardware strength
 * allows for running simulations at the same amount of FPS as the graphics,
 * we'll use the same integer sizes for the simulations that we do for gfx.
 */
typedef uint16_t gfx_frames_mini;
typedef uint16_t sim_ticks_mini;
typedef uint32_t gfx_frames_extended;
typedef uint32_t sim_ticks_extended;
