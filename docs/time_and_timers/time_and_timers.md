# Overview of time and timers

## Real time and process time

Real time is defined as time measured from some fixed point, either from a standard point in the past (see the description of the Epoch and calendar time below), or from some point (e.g., the start) in the life of a process (elapsed time).

Process time is defined as the amount of CPU time used by a process.  This is sometimes divided into user and system components.  User CPU time is the time spent executing code in user mode.  System CPU time  is the time spent by the kernel executing in system mode on behalf of the process (e.g., executing system calls).  The time(1) command can be used to determine the amount of CPU time consumed during the execution of a program.  A program can determine the amount of CPU time it has consumed using times(2), getrusage(2), or clock(3).

```bash
time sleep 2
# real  0m2.001s
# user  0m0.001s
# sys   0m0.000s
```

```c
#include <unistd.h>
// clock tick - the number of clock tick per second

long clock_tick = sysconf(_SC_CLK_TCK);

#include <sys/times.h>

// CPU time used by a process and its children.
struct tms {
  clock_t tms_utime;  /* user time */
  clock_t tms_stime;  /* system time */
  clock_t tms_cutime; /* user time of children */
  clock_t tms_cstime; /* system time of children */
};

// Store the CPU time used by this process and all its dead children
// (and their dead children) in BUFFER. Return the elapsed real time,
// or (clock_t) -1 for errors.
// All times are in CLK_TCKths of a second.

// !!! The return value of *times* is measures has veried across kernel
// and that may overflow the range of *clock_t*, means that a portable
// application would be wise to avoid using this value, to measure
// changes in elapsed time, use *clock_gettime* instead.
clock_t times(struct tms *buf);
```

```c
#include <sys/time.h>
#include <sys/resource.h>

struct timeval {
  time_t tv_sec;        // seconds
  suseconds_t tv_usec;  // microseconds
};

struct rusage {
  struct timeval ru_utime; // user CPU time used
  struct timeval ru_stime; // system CPU time used
  long   ru_maxrss;        // maximum resident set size
  long   ru_ixrss;         // integral shared memory size
  long   ru_idrss;         // integral unshared data size
  long   ru_isrss;         // integral unshared stack size
  long   ru_minflt;        // page reclaims (soft page faults)
  long   ru_majflt;        // page faults (hard page faults)
  long   ru_nswap;         // swaps
  long   ru_inblock;       // block input operations
  long   ru_oublock;       // block output operations
  long   ru_msgsnd;        // IPC messages sent
  long   ru_msgrcv;        // IPC messages received
  long   ru_nsignals;      // signals received
  long   ru_nvcsw;         // voluntary context switches
  long   ru_nivcsw;        // involuntary context switches
};

// Get resource usage
// *who* which can be one of the following:
// RUSAGE_SELF
// RUSAGE_CHILDREN
// RUSAGE_TRHEAD
int getrusage(int who, struct rusage* usage);

// struct timezone {
//   int tz_minuteswest; // minutes west of Greenwich
//   int tz_dsttime;     // type of DST correction
// };

// requirements _DEFAULT_SOURCE or _BSD_SOURCE
// Get and set the time as well as a timezone.
// !!!  The use of the timezone structure is obsolete; the tz argument
// should normally be specified as NULL.
// !!! POSIX.1-2008 marks gettimeofday() as obsolete, recommending the
// use of clock_gettime(2) instead.
int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);

```

```c
#include <time.h>

// ISO/IEC 9899:1999 7.23.1: Compoents of time
// The macro *CLOCKS_PER_SEC* is an expression with type *clock_t* that
// the number per second of the value returned by the *clock* function.
//
// CAE XSH, Issue 4, Version 2: <time.h>
// The value of *CLOCKS_PER_SEC* is required to be 1 million on all
// XSI-conformant systems.
#define CLOCKS_PER_SEC ((clock_t) 1000000)

// Returns an approximation of processor time used by the program
// implemented by clock_gettime with CLOCK_PROCESS_CPUTIME_ID
clock_t clock(void);

// Get time in seconds, returns the time as the number of seconds
// since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
// The tloc argument is obsolescent and should always be NULL in new
// code.  When tloc is NULL, the call cannot fail.
time_t time(time_t* tloc);

struct timespec {
  time_t tv_sec;
  long tv_nsec;
};

// requirements _POSIX_C_SOURCE >= 199309L and link with -lrt

int clock_getres(clockid_t clockid, struct timespec *res);
int clock_gettime(clockid_t clockid, struct timespec *tp);
int clock_settime(clockid_t clockid, const struct timespec *tp);
```

## The hardware clock

Most computers have a (battery-powered) hardware clock which the kernel reads at boot time in order to initialize the software clock.  For further details, see rtc(4) and hwclock(8).

## The software clock, HZ and jiffies

The accuracy of various system calls that set timeouts, (e.g., select(2), sigtimedwait(2)) and measure CPU time (e.g., getrusage(2)) is limited by the resolution of the software clock, a clock maintained by the kernel which measures time in jiffies.  The size of a jiffy is determined by the value of the kernel constant HZ.

The  value  of  HZ varies across kernel versions and hardware platforms.  On i386 the situation is as follows: on kernels up to and including 2.4.x, HZ was 100, giving a jiffy value of 0.01 seconds;starting with 2.6.0, HZ was raised to 1000, giving a jiffy of 0.001 seconds.  Since kernel 2.6.13, the HZ value is a kernel configuration parameter and can be 100, 250 (the default) or 1000, yielding  a jiffies value of, respectively, 0.01, 0.004, or 0.001 seconds.  Since kernel 2.6.20, a further frequency is available: 300, a number that divides evenly for the common video frame rates (PAL, 25 HZ; NTSC, 30 HZ).

The times(2) system call is a special case. It reports times with a granularity defined by the kernel constant USER_HZ. User-space applications can determine the value of this constant using sysconf(_SC_CLK_TCK).

## System and process clocks: time namespace

The kernel supports a range of clocks that measure various kinds of elapsed and virtual (i.e., consumed CPU) time. These clocks are described in clock_gettime(2). A few of the clocks are settable using clock_settime(2). The values of certain clocks are virtualized by time namespaces; see time_namespaces(7).

## High-resolution timers

Before Linux 2.6.21, the accuracy of timer and sleep system calls (see below) was also limited by the size of the jiffy.

Since Linux 2.6.21, Linux supports high-resolution timers (HRTs), optionally configurable via CONFIG_HIGH_RES_TIMERS. On a system that supports HRTs, the accuracy of sleep and timer system calls is no longer constrained by the jiffy, but instead can be as accurate as the hardware allows (microsecond accuracy is typical of modern hardware). You can determine whether high-resolution timers are supported by checking the resolution returned by a call to clock_getres(2) or looking at the "resolution" entries in /proc/timer_list.

HRTs are not supported on all hardware architectures. (Support is provided on x86, arm, and powerpc, among others.)

## The Epoch

UNIX systems represent time in seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).

A program can determine the calendar time via the clock_gettime(2) CLOCK_REALTIME clock, which returns time (in seconds and nanoseconds) that have elapsed since the Epoch; time(2) provides similar information, but only with accuracy to the nearest second. The system time can be changed using clock_settime(2).

## Broken-down time

Certain library functions use a structure of type tm to represent broken-down time, which stores time value separated out into distinct components (year, month, day, hour, minute, second, etc.).

This structure is described in ctime(3), which also describes functions that convert between calendar time and broken-down time. Functions for converting between broken-down time and printable string representations of the time are described in ctime(3), strftime(3), and strptime(3).

## Sleeping and setting time

Various system calls and functions allow a program to sleep (suspend execution) for a specified period of time; see nanosleep(2), clock_nanosleep(2), and sleep(3).

Various system calls allow a process to set a timer that expires at some point in the future, and optionally at repeated intervals; see alarm(2), getitimer(2), timerfd_create(2), and timer_create(2).

## Timer slack

Since Linux 2.6.28, it is possible to control the "timer slack" value for a thread. The timer slack is the length of time by which the kernel may delay the wake-up of certain system calls that block with a timeout. Permitting this delay allows the kernel to coalesce wake-up events, thus possibly reducing the number of system wake-ups and saving power. For more details, see the description of PR_SET_TIMERSLACK in prctl(2).
