/*
 * unix_time.c
 *
 *  Created on: Mar 27, 2014
 *      Author: cchao
 */

#include "unix_time.h"
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

int time_year(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_year + 1900;
}

int time_month(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_mon + 1;
}

int time_mday(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_mday;
}

int time_wday(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_wday;
}

int time_hour(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_hour;
}

int time_minute(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_min;
}

int time_second(const struct timeval* tv) {
  return localtime(&tv->tv_sec)->tm_sec;
}

int time_msecond(const struct timeval* tv) {
  return (int) tv->tv_usec / 1000;
}

void time_date(const struct timeval* tv, int* year, int* month, int* mday) {
  const struct tm* tm = localtime(&tv->tv_sec);

  if (year != NULL) {
    *year = tm->tm_year + 1900;
  }

  if (month != NULL) {
    *month = tm->tm_mon + 1;
  }

  if (mday != NULL) {
    *mday = tm->tm_mday;
  }
}

void time_time(const struct timeval* tv, int* hour, int* minute, int* second,
               int* msecond) {
  const struct tm* tm = localtime(&tv->tv_sec);

  if (hour != NULL) {
    *hour = tm->tm_hour;
  }

  if (minute != NULL) {
    *minute = tm->tm_min;
  }

  if (second != NULL) {
    *second = tm->tm_sec;
  }

  if (msecond != NULL) {
    *msecond = (int) tv->tv_usec / 1000;
  }
}

long make_time(int year, int month, int mday, int hour, int minute, int second) {
  struct tm t;

  memset(&t, 0, sizeof(t));

  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = mday;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = second;

  return mktime(&t);
}

int time_mdiff(const struct timeval* tvStart, const struct timeval* tvEnd) {
  return ((long long)tvEnd->tv_sec - tvStart->tv_sec) * 1000
      + ((long long)tvEnd->tv_usec - tvStart->tv_usec) / 1000;
}

int now_is_during(int hour_start, int minute_start, int second_start,
                  int hour_end, int minute_end, int second_end) {
  return time_is_during(time(0), hour_start, minute_start, second_start,
                        hour_end, minute_end, second_end);
}

int time_is_during(long now, int hour_start, int minute_start, int second_start,
                   int hour_end, int minute_end, int second_end) {
  struct tm t;

  memset(&t, 0, sizeof(t));

  t.tm_year = 1970 - 1900;
  t.tm_mon = 1;
  t.tm_mday = 1;
  t.tm_hour = hour_start;
  t.tm_min = minute_start;
  t.tm_sec = second_start;

  time_t t_start = mktime(&t);

  t.tm_hour = hour_end;
  t.tm_min = minute_end;
  t.tm_sec = second_end;

  time_t t_end = mktime(&t);
  int inc_flag = 0;

  if (t_end < t_start) {
    t_end += 24 * 60 * 60;
    inc_flag = 1;
  }

  struct timeval tv;

  tv.tv_sec = now;
  tv.tv_usec = 0;

  t.tm_hour = time_hour(&tv);
  t.tm_min = time_minute(&tv);
  t.tm_sec = time_second(&tv);

  time_t t_now = mktime(&t);
  if (inc_flag && t.tm_hour <= hour_end) {
    t_now += 24 * 60 * 60;
  }

  if (t_now >= t_start && t_now < t_end) {
//    printf("t_end: %ld, t_start: %ld, t_now: %ld, inc_flag: %d\n", t_end, t_start, t_now, inc_flag);
    return 1;
  }

//  printf(">t_end: %ld, t_start: %ld, t_now: %ld, inc_flag: %d\n", t_end, t_start, t_now, inc_flag);

  return 0;
}

void date_parse(const char* date, int* year, int* month, int* mday) {
  char tmp[8] = { 0 };

  sscanf(date, "%s %d %d", tmp, mday, year);

  const char* MONTH[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

  for (int i = 0; i < 12; ++i) {
    if (!strcmp(MONTH[i], tmp)) {
      *month = i + 1;
      break;
    }
  }
}

void time_parse(const char* time, int* hour, int* minute, int* second) {
  sscanf(time, "%d:%d:%d", hour, minute, second);
}

long time_clock(struct timeval* clock) {
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);

  if (clock != NULL) {
    clock->tv_sec = ts.tv_sec;
    clock->tv_usec = ts.tv_nsec / 1000;
  }

  return ts.tv_sec;
}

