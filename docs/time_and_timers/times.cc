#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

void times_test() {
  std::cout << "times()" << std::endl;
  long clock_ticks = sysconf(_SC_CLK_TCK);
  for (int count = 0; count < 3; ++count) {
    struct tms buf;
    clock_t ticks = times(&buf);
    std::cout << "Ticks: " << ticks
              << " Seconds: " << (ticks / clock_ticks)
              << " User time: " << buf.tms_utime
              << " System time: " << buf.tms_stime
              << " User time of children: " << buf.tms_cutime
              << " System time of children: " << buf.tms_cstime
              << std::endl;
    sleep(2);
  }
}

void getrusage_test() {
  std::cout << "getrusage()" << std::endl;
  for (int count = 0; count < 3; ++count) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "User time: " << usage.ru_utime.tv_sec
                               << "." << usage.ru_utime.tv_usec
              << " System time: " << usage.ru_stime.tv_sec
                               << "." << usage.ru_stime.tv_usec
              << std::endl;
    sleep(2);
  }
}

void gettimeofday_test() {
  std::cout << "gettimeofday()" << std::endl;
  for (int count = 0; count < 3; ++count) {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    std::cout << "gettimeofday seconds: " << tv.tv_sec
              << "." << tv.tv_usec
              << std::endl;
    sleep(2);
  }
}

void clock_test() {
  std::cout << "clock()" << std::endl;
  for (int count = 0; count < 3; ++count) {
    clock_t ticks = clock();
    std::cout << "Ticks: " << ticks
              << " Seconds: " << (ticks / CLOCKS_PER_SEC)
              << "." << (ticks % CLOCKS_PER_SEC)
              << std::endl;
    sleep(2);
  }
}

int main() {
  times_test();
  getrusage_test();
  gettimeofday_test();
  clock_test();
  return 0;
}