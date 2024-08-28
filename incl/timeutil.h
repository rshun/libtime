#ifndef __TIMEUTIL_H
#define __TIMEUTIL_H

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifdef LINUX
#include <limits.h>
#endif

#ifndef SUCC
#define SUCC  (0)
#endif

#ifndef FAIL
#define FAIL  (-1)
#endif

#ifndef TIMESTPOFDAY
#define TIMESTPOFDAY (86400L)
#endif

#ifndef DATELEN
#define DATELEN (20)
#endif

#ifndef TIMELEN
#define TIMELEN (20)
#endif

#ifndef INVALID_DATE_VALUE
#define   INVALID_DATE_VALUE (LLONG_MAX)
#endif

#ifndef INVALID_DATE
#define INVALID_DATE (INT_MAX)
#endif

/*
Support Date Format: YYYYMMDD(default)
                     YYYY-MM-DD
                     YYYY.MM.DD
                     YYYY/MM/DD
                     MM/DD/YY
                     DD/MM/YY
                     MM/DD/YYYY
                     DD/MM/YYYY
Supoort Time Format: HHMMSS(default)
                     HHMMSSBBB
                     HH:MM:SS
                     HH:MM:SS:BBB

1. if the function's parameters include a date or time, they will first be automatically checked. If they are invalid, the default values will be used. If they are still invalid, an error will be return
2. if the return value is a date or time, the default value will be used
3. if the function is for date or time format conversion, both the source and target parameters must include format specifications
4. if both the day and month are less than 12, automatic detection for MM/DD/YY, DD/MM/YY, MM/DD/YYYY, and DD/MM/YYYY formats will result in an error
5. if the year format is 'YY', it defaults to '20YY'.
6. timeutil_getDateTime function returns a fixed format.
*/

/*
function: initial date format
parameter:
  1. date format
return:
  SUCC---success, FAIL---date format is illegal
*/
int timeutil_initDateFormat(const char* );

/*
function: return the current date format
parameter:
    1. date format
    2. the date format's length
return:
  SUCC--current date format,NULL--insufficient string length
*/
char* timeutil_getDateFormat(char* ,size_t );

/*
function: validate if the date is valid
parameter:
    1. date string
return:
    SUCC---date is legal, FAIL---date is illeagl
*/
int timeutil_checkDate(const char *);

/*
function: get current date
parameter:
    1. date string
    2. date string's length
return:
    SUCC---current date, NULL---insufficient string length
*/
char* timeutil_getDate(char* ,size_t );

/*
功能:转换带格式的日期
参数:
  参数1---源日期字符串
  参数2---源日期格式
  参数3---目标日期字符串
  参数4---目标日期字符串长度
  参数5---目标日期格式
返回: 正确---转换后的带格式日期,NULL---日期非法或目标长度不足
*/
char* timeutil_transDate(const char* ,const char* ,char* ,size_t ,const char* );

/*
功能:将时间戳转换成日期
参数:
  参数1--时间戳
  参数2--日期字符串
  参数3--参数2长度
返回: 正确---日期字符串,NULL---错误
*/
char* timeutil_convertTimestampToDate(time_t ,char* ,size_t );

/*
功能:将日期转换成时间戳(时间统一为08:00:00)
参数:
  参数1--日期时间字符串
返回: 正确---时间戳, FAIL---日期格式错误
*/
time_t timeutil_convertDateToTimestamp(const char* );

/*
功能:日期相加
参数:
  参数1--日期字符串
  参数2--天数(如果是负数，则日期相减)
  参数3--相加后的日期
  参数4--参数3的长度
返回:成功---相加后的日期,NULL---日期非法
*/
char* timeutil_addDate(const char *,int ,char* ,size_t );

/*
功能:日期减法
参数:
  参数1--日期字符串
  参数2--天数(如果是负数，则日期相加)
  参数3--减去天数后的日期
  参数4--参数3的长度
返回:成功---减去天数的日期,NULL---日期非法
*/
char* timeutil_subDate(const char *,int ,char* ,size_t );

/*
功能:两个日期相减后的天数
参数:
  参数1--日期字符串1
  参数2--日期字符串2
返回: INVALID_DATE---日期非法,其他---相减后的天数
*/
int timeutil_calcDateDiff(const char* ,const char* );

/*
功能:将日期字符串转换成长整型
参数:
  参数1--日期字符串
返回:成功---长整型的日期,FAIL---日期非法
*/
long timeutil_convertDateStrToLong(const char* );

/*
功能:将长整形日期转换成字符串
参数:
  参数1--长整型日期
  参数2--日期字符串
  参数3--日期字符串长度
返回:成功---字符串日期, NULL---日期非法或长度不足
*/
char* timeutil_convertLongDateToStr(long ,char* ,size_t );

/*
功能:获取当前时间戳
参数:
  参数1---无
返回:当前时间戳
*/
time_t timeutil_getTimestamp();

/*
功能: 初始化时间格式
参数:
  参数1--时间格式
返回: SUCC---成功, FAIL---日期格式非法
*/
int timeutil_initTimeFormat(const char* );

/*
功能: 返回当前时间格式
参数: 
    参数1--时间格式
    参数2--格式长度
返回: 成功---当前默认的时间格式, NULL---长度不足
*/
char* timeutil_getTimeFormat(char* ,size_t );

/*
功能:校验时间否合法
参数:
  参数1-时间字符串
返回:SUCC---时间合法,FAIL---时间非法
*/
int timeutil_checkTime(const char *);

/*
功能:获取时间
参数:
  参数1--时间字符串
  参数2--时间长度
返回:正确---时间字符串,NULL---错误
*/
char* timeutil_getTime(char* ,size_t );

/*
功能:转换带格式的时间
参数:
  参数1---源时间字符串
  参数2---源时间格式
  参数3---目标时间字符串
  参数4---目标时间字符串长度
  参数5---目标时间格式
返回: 正确---转换后的带格式时间,NULL---时间非法或目标长度不足
*/
char* timeutil_transTime(const char* ,const char* ,char* ,size_t ,const char* );

/*
功能:将时间戳转换成时间
参数:
  参数1--时间戳
  参数2--时间字符串
  参数3--参数2长度
返回: 正确---时间字符串,NULL---错误
*/
char* timeutil_convertTimestampToTime(time_t ,char* ,size_t );

/*
功能:返回毫秒级日期时间
参数:
  参数1--日期时间字符串
  参数2--参数1长度
返回:正确---毫秒级的日期时间(YYYY-MM-DD HH24:MM:SS:BBB),NULL---错误
*/
char* timeutil_getDateTime(char* ,size_t );

/*
功能:将时间戳转换成日期时间
参数:
  参数1--时间戳
  参数2--日期字符串
  参数3--参数2长度
返回:正确---日期时间字符串,NULL---错误
*/
char* timeutil_convertTimestampToDateTime(time_t ,char* ,size_t );

/* 
功能: 根据日期字符串获取星期
参数:
  参数1-日期时间字符串
返回: 成功---星期(0-星期天),FAIL---日期格式错误
*/
int timeutil_getWeek(const char* );

/*
功能: 获取当前年份
参数:
返回:成功---当前年份,FAIL---错误
*/
int timeutil_getCurrYear(void);

/*
功能: 获取当前月份
参数:
返回:成功---当前月份,FAIL---错误
*/
int timeutil_getCurrMonth(void);

/*
功能: 获取当前日期
参数:
返回:成功---当前日期,FAIL---错误
*/
int timeutil_getCurrDay(void);


#endif