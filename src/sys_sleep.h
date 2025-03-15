// high-perf functions

#include <stdint.h>

// exports

uint64_t time_ns();

#ifdef _WIN32
#define sys_sleep(ms) timerSleep((ms)/1000.0) // SleepEx((ms), FALSE)
#define sys_yield()   SwitchToThread()
#else
#define sys_sleep(ms) usleep((ms)*1000)
#define sys_yield()   usleep(0)
#endif


// impl

#ifdef _WIN32
#include <winsock2.h>

static uint64_t nanotimer(uint64_t *out_freq) {
    if( out_freq ) {
        LARGE_INTEGER li;
        QueryPerformanceFrequency(&li);
        *out_freq = li.QuadPart;
    }
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (uint64_t)li.QuadPart;
}

void timerSleep(double seconds) {
    // src: https://blog.bearcats.nl/accurate-sleep-function/

    static HANDLE timer = 0; do_once timer = CreateWaitableTimerA(NULL, FALSE, NULL);
    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;
    
    while (seconds - estimate > 1e-7) {
        double toWait = seconds - estimate;
        LARGE_INTEGER due = {0};
        due.QuadPart = -(int64_t)(toWait * 1e7);
        SetWaitableTimerEx(timer, &due, 0, NULL, NULL, NULL, 0);
        uint64_t start = time_ns();
        WaitForSingleObject(timer, INFINITE);
        uint64_t end = time_ns();

        double observed = (end - start) / 1e9;
        seconds -= observed;

        ++count;
        double error = observed - toWait;
        double delta = error - mean;
        mean += delta / count;
        m2   += delta * (error - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    // spin lock
    uint64_t start = time_ns();
    while ((time_ns() - start) < (seconds * 1e9));
}

#else

#include <sys/time.h>

#define TIMER_E3 1000ULL
#define TIMER_E6 1000000ULL
#define TIMER_E9 1000000000ULL

#ifdef CLOCK_MONOTONIC_RAW
#define TIME_MONOTONIC CLOCK_MONOTONIC_RAW
#elif defined CLOCK_MONOTONIC
#define TIME_MONOTONIC CLOCK_MONOTONIC
#else
// #define TIME_MONOTONIC CLOCK_REALTIME // untested
#endif

static uint64_t nanotimer(uint64_t *out_freq) {
    if( out_freq ) {
#if defined TIME_MONOTONIC
        *out_freq = TIMER_E9;
#else
        *out_freq = TIMER_E6;
#endif
    }
#if defined TIME_MONOTONIC
    struct timespec ts;
    clock_gettime(TIME_MONOTONIC, &ts);
    return (TIMER_E9 * (uint64_t)ts.tv_sec) + ts.tv_nsec;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (TIMER_E6 * (uint64_t)tv.tv_sec) + tv.tv_usec;
#endif
}

#endif



uint64_t time_ns() {
    static __thread uint64_t epoch = 0;
    static __thread uint64_t freq = 0;
    if( !freq ) {
        epoch = nanotimer(&freq);
    }

    uint64_t a = nanotimer(NULL) - epoch;
    uint64_t b = 1000000000ULL;
    uint64_t c = freq;

    // Computes (a*b)/c without overflow, as long as both (a*b) and the overall result fit into 64-bits.
    // [ref] https://github.com/rust-lang/rust/blob/3809bbf47c8557bd149b3e52ceb47434ca8378d5/src/libstd/sys_common/mod.rs#L124
    uint64_t q = a / c;
    uint64_t r = a % c;
    return q * b + r * b / c;
}















/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#define Uint64              uint64_t
#define SDL_GetTicksNS()    time_ns()
#define SDL_NS_PER_MS       1000000
#define SDL_SYS_DelayNS(ns) sys_sleep(((ns) / SDL_NS_PER_MS))

#if (defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))
    #define SDL_CPUPauseInstruction() __asm__ __volatile__("pause\n")  /* Some assemblers can't do REP NOP, so go with PAUSE. */
#elif (defined(__arm__) && defined(__ARM_ARCH) && __ARM_ARCH >= 7) || defined(__aarch64__)
    #define SDL_CPUPauseInstruction() __asm__ __volatile__("yield" ::: "memory")
#elif (defined(__powerpc__) || defined(__powerpc64__))
    #define SDL_CPUPauseInstruction() __asm__ __volatile__("or 27,27,27");
#elif (defined(__riscv) && __riscv_xlen == 64)
    #define SDL_CPUPauseInstruction() __asm__ __volatile__(".insn i 0x0F, 0, x0, x0, 0x010");
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
    #define SDL_CPUPauseInstruction() _mm_pause()  /* this is actually "rep nop" and not a SIMD instruction. No inline asm in MSVC x86-64! */
#elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
    #define SDL_CPUPauseInstruction() __yield()
#elif defined(__WATCOMC__) && defined(__386__)
    extern __inline void SDL_CPUPauseInstruction(void);
    #pragma aux SDL_CPUPauseInstruction = ".686p" ".xmm2" "pause"
#else
    #define SDL_CPUPauseInstruction()
#endif

void SDL_DelayPrecise(Uint64 ns)
{
    Uint64 current_value = SDL_GetTicksNS();
    const Uint64 target_value = current_value + ns;

    // Sleep for a short number of cycles when real sleeps are desired.
    // We'll use 1 ms, it's the minimum guaranteed to produce real sleeps across
    // all platforms.
    const Uint64 SHORT_SLEEP_NS = 1 * SDL_NS_PER_MS;

    // Try to sleep short of target_value. If for some crazy reason
    // a particular platform sleeps for less than 1 ms when 1 ms was requested,
    // that's fine, the code below can cope with that, but in practice no
    // platforms behave that way.
    Uint64 max_sleep_ns = SHORT_SLEEP_NS;
    while (current_value + max_sleep_ns < target_value) {
        // Sleep for a short time
        SDL_SYS_DelayNS(SHORT_SLEEP_NS);

        const Uint64 now = SDL_GetTicksNS();
        const Uint64 next_sleep_ns = (now - current_value);
        if (next_sleep_ns > max_sleep_ns) {
            max_sleep_ns = next_sleep_ns;
        }
        current_value = now;
    }

    // Do a shorter sleep of the remaining time here, less the max overshoot in
    // the first loop. Due to maintaining max_sleep_ns as
    // greater-than-or-equal-to-1 ms, we can always subtract off 1 ms to get
    // the duration overshot beyond a 1 ms sleep request; if the system never
    // overshot, great, it's zero duration. By choosing the max overshoot
    // amount, we're likely to not overshoot here. If the sleep here ends up
    // functioning like SDL_DelayNS(0) internally, that's fine, we just don't
    // get to do a more-precise-than-1 ms-resolution sleep to undershoot by a
    // small amount on the current system, but SDL_DelayNS(0) does at least
    // introduce a small, yielding delay on many platforms, better than an
    // unyielding busyloop.
    //
    // Note that we'll always do at least one sleep in this function, so the
    // minimum resolution will be that of SDL_SYS_DelayNS()
    if (current_value < target_value && (target_value - current_value) > (max_sleep_ns - SHORT_SLEEP_NS)) {
        const Uint64 delay_ns = (target_value - current_value) - (max_sleep_ns - SHORT_SLEEP_NS);
        SDL_SYS_DelayNS(delay_ns);
        current_value = SDL_GetTicksNS();
    }

    // We've likely undershot target_value at this point by a pretty small
    // amount, but maybe not. The footgun case if not handled here is where
    // we've undershot by a large amount, like several ms, but still smaller
    // than the amount max_sleep_ns overshot by; in such a situation, the above
    // shorter-sleep block didn't do any delay, the if-block wasn't entered.
    // Also, maybe the shorter-sleep undershot by several ms, so we still don't
    // want to spin a lot then. In such a case, we accept the possibility of
    // overshooting to not spin much, or if overshot here, not at all, keeping
    // CPU/power usage down in any case. Due to scheduler sloppiness, it's
    // entirely possible to end up undershooting/overshooting here by much less
    // than 1 ms even if the current system's sleep function is only 1
    // ms-resolution, as SDL_GetTicksNS() generally is better resolution than 1
    // ms on the systems SDL supports.
    while (current_value + SHORT_SLEEP_NS < target_value) {
        SDL_SYS_DelayNS(SHORT_SLEEP_NS);
        current_value = SDL_GetTicksNS();
    }

    // Spin for any remaining time
    while (current_value < target_value) {
        SDL_CPUPauseInstruction();
        current_value = SDL_GetTicksNS();
    }
}

#define sys_sleep_precisens SDL_DelayPrecise

