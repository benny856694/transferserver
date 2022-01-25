/**
 * @file    unix_time.h
 * @brief   unix时间相关实用方法。
 * @version 1.0.0
 * @date    2014-3-27
 * @author  cchao
 */

#ifndef UNIX_TIME_H_
#define UNIX_TIME_H_

#ifdef __cplusplus
extern "C"
{
#endif

// 时间1是否大于等于时间2
#define GE(time1, time2) \
  ((time1).tv_sec * 1000 + (time1).tv_usec / 1000 >= \
      (time2).tv_sec * 1000 + (time2).tv_usec / 1000)

struct timeval;

/// 提取年份。
int time_year(const struct timeval* tv);

/// 提取月份。
int time_month(const struct timeval* tv);

/// 提取以月份为单位的日期。
int time_mday(const struct timeval* tv);

/// 提取周为单位的日期。
int time_wday(const struct timeval* tv);

/// 提取时。
int time_hour(const struct timeval* tv);

/// 提取分。
int time_minute(const struct timeval* tv);

/// 提取秒。
int time_second(const struct timeval* tv);

/// 提取毫秒。
int time_msecond(const struct timeval* tv);

/// 提取日期。允许输出参数为NULL。
void time_date(const struct timeval* tv, int* year, int* month, int* mday);

/// 提取时间。允许输出参数为NULL。
void time_time(const struct timeval* tv, int* hour, int* minute, int* second, int* msecond);

/**
 * @brief   由年月日时分秒等信息构造unix时间。
 * @return  1970年1月1日0时0分0秒至今所经过的秒数。出错时返回-1。
 */
long make_time(int year, int month, int mday, int hour, int minute, int second);

/**
 * @brief   计算两个时间差。
 * @return  返回差值（单位：毫秒）。
 */
int time_mdiff(const struct timeval* tvStart, const struct timeval* tvEnd);

/**
 * @brief   判断当前系统时间是否位于某个时段。
 * @return  返回1时表示当前系统时间位于该时段，否则返回0。
 */
int now_is_during(int hour_start, int minute_start, int second_start,
                  int hour_end, int minute_end, int second_end);

/**
 * @brief   判断某个时间是否位于某个时段。
 * @return  返回1时表示给定时间位于该时段，否则返回0。
 */
int time_is_during(long now,
                   int hour_start, int minute_start, int second_start,
                   int hour_end, int minute_end, int second_end);

/**
 * @brief   从__DATE__宏定义解析日期。
 * @param   date 须为__DATE__或与其格式一致。
 * @param   year [out] 年。
 * @param   month [out] 月。
 * @param   mday [out] 日。
 * @note    输入输出参数均不允许为NULL。
 */
void date_parse(const char* date, int* year, int* month, int* mday);

/**
 * @brief   从__TIME__宏定义解析时间。
 * @param   time 须为__TIME__或与其格式一致。
 * @param   hour [out] 时。
 * @param   minute [out] 分。
 * @param   second [out] 秒。
 * @note    输入输出参数均不允许为NULL。
 */
void time_parse(const char* time, int* hour, int* minute, int* second);

/**
 * @brief   获取基于CPU时钟的时间。
 * @param   clock [out] 输出时间。允许为NULL。
 * @return  返回基于CPU时钟经过的秒数。
 * @note    该时间为自系统启动以来的计时，不会随系统时间改变而发生跳跃。
 */
long time_clock(struct timeval* clock);

#ifdef __cplusplus
}
#endif

#endif /* UNIX_TIME_H_ */
