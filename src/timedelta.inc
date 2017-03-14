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

#include <time.h>

#ifdef __FreeBSD__
#define JUMPER_CLOCK CLOCK_REALTIME_PRECISE
#define _CLOCK_GETTIME
#elif __linux__
#define JUMPER_CLOCK CLOCK_REALTIME
#define _CLOCK_GETTIME
#else
#error "System currently not supported."
#endif

#ifdef _CLOCK_GETTIME
#define GETTIME(TGT) \
	clock_gettime(JUMPER_CLOCK, TGT)
#endif

#define MEASURE_TIMEDELTA_NS(tdiff, tprev, tcurr) \
	tprev = tcurr; \
	GETTIME(&tspec); \
	tcurr = get_ns(tspec); \
	tdiff = get_tdiff(tprev, tcurr);

static inline nanoseconds get_ns (struct timespec tspec)
{
	return ((nanoseconds) tspec.tv_nsec + (tspec.tv_sec * INV_NANO));
}

static inline nanoseconds get_tdiff (nanoseconds tprev, nanoseconds tcurr)
{
	return tcurr - tprev;
}
