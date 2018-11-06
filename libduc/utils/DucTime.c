
#include <duc/time.h>

#ifndef _WIN32
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#endif

uint64_t Duc_GetTime(void)
{
	uint64_t ticks = 0;
#if defined(__linux__)
	struct timespec ts;

	if (!clock_gettime(CLOCK_MONOTONIC_RAW, &ts))
		ticks = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
#elif defined(_WIN32)
	ticks = (uint64_t) GetTickCount64();
#else
	struct timeval tv;

	if (!gettimeofday(&tv, NULL))
		ticks = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
	return ticks;
}

