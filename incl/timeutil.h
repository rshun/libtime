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
#define INVALID_DATE_VALUE (LLONG_MAX)
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

支持日期格式: YYYYMMDD(默认),YYYY-MM-DD,YYYY.MM.DD,YYYY/MM/DD,MM/DD/YY,DD/MM/YY,MM/DD/YYYY,DD/MM/YYYY
支持时间格式: HHMMSS(默认),HHMMSSBBB,HH:MM:SS,HH:MM:SS:BBB

1. 如果入参若包含日期或时间,首先会自动检测, 如果不合法,则取默认值,如果仍然不合法则报错
2. 如果出参是日期或时间, 则取默认值
3. 如果函数是日期或时间格式转换,则入参的源和目标都需要带格式说明
4. 如果日期和月份均小于12,MM/DD/YY,DD/MM/YY,MM/DD/YYYY,DD/MM/YYYY自动检测都会报错
5. 如果年份格式是YY, 默认是20YY
6. timeutil_getDateTime 函数返回固定格式
*/

/*
function: initial date format
parameter:
  1. date format
return:
  SUCC---success, FAIL---date format is illegal

功能: 初始化日期格式
参数:
  参数1--日期格式
返回: SUCC---成功, FAIL---日期格式非法
*/
int timeutil_initDateFormat(const char* );

/*
function: return the current date format
parameter:
    1. date format
    2. the date format's length
return:
  SUCC--current date format,NULL--insufficient string length

功能: 返回当前日期格式
参数: 
    参数1--日期格式
    参数2--格式长度
返回: 成功---当前默认的日期格式, NULL---长度不足
*/
char* timeutil_getDateFormat(char* ,size_t );

/*
function: validate if the date is valid
parameter:
    1. date string
return:
    SUCC---date is legal, FAIL---date is illeagl

功能:校验日期否合法
参数:
  参数1-日期字符串
返回:SUCC---日期合法,FAIL---日期非法
*/
int timeutil_checkDate(const char *);

/*
function: get current date
parameter:
    1. date string
    2. date string's length
return:
    SUCC---current date, NULL---insufficient string length

功能:获取当前日期
参数:
  参数1-日期字符串
  参数2-日期字符串长度
返回: 正确---当前日期,NULL---错误
*/
char* timeutil_getDate(char* ,size_t );

/*
function: change date from format
parameter:
  1. source date string
  2. source date length
  3. target date string
  4. target date length
  5. target date format
return:
  SUCC---format date afte translation, NULL---invalid date or target length is insufficient

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
function: convert to date from timestamp
parameter:
  1. timestamp
  2. date string
  3. date string length
return:
  SUCC---date string, NULL---timestamp is error or date string's length is insufficient

功能:将时间戳转换成日期
参数:
  参数1--时间戳
  参数2--日期字符串
  参数3--参数2长度
返回: 正确---日期字符串,NULL---错误
*/
char* timeutil_convertTimestampToDate(time_t ,char* ,size_t );

/*
function: convert to timestamp from date(time is 08:00:00)
parameter:
  1. date string 
return:
  SUCC---timestamp, FAIL---invalid date string.

功能:将日期转换成时间戳(时间统一为08:00:00)
参数:
  参数1--日期时间字符串
返回: 正确---时间戳, FAIL---日期格式错误
*/
time_t timeutil_convertDateToTimestamp(const char* );

/*
function: add days to date
parameter:
  1. date string
  2. days(if negative, date is subtracted)
  3. date after adding
  4. length of parameter 3
return:
  SUCC---date after adding, NULL---invalid date

功能:日期相加
参数:
  参数1--日期字符串
  参数2--天数(如果是负数，则日期相减)
  参数3--相加后的日期
  参数4--参数3的长度
返回: SUCC---相加后的日期,NULL---日期非法
*/
char* timeutil_addDate(const char *,int ,char* ,size_t );

/*
function: subtract days from date
parameter:
  1. date string
  2. days(if negative, date is added)
  3. date after subtracting
  4. length of parameter 3
return:
  SUCC---date after subtracting, NULL---invalid date

功能:日期减法
参数:
  参数1--日期字符串
  参数2--天数(如果是负数，则日期相加)
  参数3--减去天数后的日期
  参数4--参数3的长度
返回: SUCC---减去天数的日期,NULL---日期非法
*/
char* timeutil_subDate(const char *,int ,char* ,size_t );

/*
function: calculate the number of days between two dates
parameter:
  1. date string 1
  2. date string 2
return:
  INVALID_DATE---invalid date, other---number of days after subtraction

功能:两个日期相减后的天数
参数:
  参数1--日期字符串1
  参数2--日期字符串2
返回: INVALID_DATE---日期非法,其他---相减后的天数
*/
int timeutil_calcDateDiff(const char* ,const char* );

/*
function: convert date to long
parameter:
  1. date string
return:
  SUCC---long date, FAIL---invalid date

功能:将日期字符串转换成长整型
参数:
  参数1--日期字符串
返回: SUCC---长整型的日期,FAIL---日期非法
*/
long timeutil_convertDateStrToLong(const char* );

/*
function: convert long date to string
parameter:
  1. long date
  2. date string
  3. date string length
return:
  SUCC---string date, NULL---invalid date or length is insufficient

功能:将长整形日期转换成字符串
参数:
  参数1--长整型日期
  参数2--日期字符串
  参数3--日期字符串长度
返回: SUCC---字符串日期, NULL---日期非法或长度不足
*/
char* timeutil_convertLongDateToStr(long ,char* ,size_t );

/*
function: get current timestamp
parameter:
  1. none
return:
  current timestamp

功能:获取当前时间戳
参数:
  参数1---无
返回:当前时间戳
*/
time_t timeutil_getTimestamp();

/*
function: initial time format
parameter:
  1. time format
return:
  SUCC---success, FAIL---time format is illegal

功能: 初始化时间格式
参数:
  参数1--时间格式
返回: SUCC---成功, FAIL---日期格式非法
*/
int timeutil_initTimeFormat(const char* );

/*
function: return the current time format
parameter:
  1. time format
  2. the time format's length
return:
  SUCC---current time format, NULL---insufficient string length

功能: 返回当前时间格式
参数: 
    参数1--时间格式
    参数2--格式长度
返回: SUCC---当前默认的时间格式, NULL---长度不足
*/
char* timeutil_getTimeFormat(char* ,size_t );

/*
function: validate time string
parameter:
  1. time string
return:
  SUCC---time is legal, FAIL---time is illegal

功能:校验时间否合法
参数:
  参数1-时间字符串
返回: SUCC---时间合法,FAIL---时间非法
*/
int timeutil_checkTime(const char *);

/*
function: get current time
parameter:
  1. time string
  2. time string length
return:
  SUCC---current time, NULL---insufficient string length

功能:获取时间
参数:
  参数1--时间字符串
  参数2--时间长度
返回: SUCC---时间字符串,NULL---错误
*/
char* timeutil_getTime(char* ,size_t );

/*
function: change time format
parameter:
  1. source time string
  2. source time length
  3. target time string
  4. target time length
  5. target time format
return:
  SUCC---time after format change, NULL---time is invalid or target length is insufficient

功能:转换带格式的时间
参数:
  参数1---源时间字符串
  参数2---源时间格式
  参数3---目标时间字符串
  参数4---目标时间字符串长度
  参数5---目标时间格式
返回: SUCC---转换后的带格式时间,NULL---时间非法或目标长度不足
*/
char* timeutil_transTime(const char* ,const char* ,char* ,size_t ,const char* );

/*
function: convert timestamp to time
parameter:
  1. timestamp
  2. time string
  3. time string length
return:
  SUCC---time string, NULL---timestamp is invalid or time string length is insufficient

功能:将时间戳转换成时间
参数:
  参数1--时间戳
  参数2--时间字符串
  参数3--参数2长度
返回: SUCC---时间字符串,NULL---错误
*/
char* timeutil_convertTimestampToTime(time_t ,char* ,size_t );

/*
function: return the current time in milliseconds
parameter:
  1. datetime string
  2. time string
return:
  SUCC---current datetime string in milliseconds, FAIL---error

功能:返回毫秒级日期时间
参数:
  参数1--日期时间字符串
  参数2--参数1长度
返回:正确---毫秒级的日期时间(YYYY-MM-DD HH24:MM:SS:BBB),NULL---错误
*/
char* timeutil_getDateTime(char* ,size_t );

/*
function: convert timestamp to datetime
parameter:
  1. timestamp
  2. datetime string
  3. datetime string length
return:
  SUCC---datetimeString, FAIL---error

功能:将时间戳转换成日期时间
参数:
  参数1--时间戳
  参数2--日期字符串
  参数3--参数2长度
返回:SUCC---日期时间字符串,NULL---错误
*/
char* timeutil_convertTimestampToDateTime(time_t ,char* ,size_t );

/* 
function: get week from date
parameter:
  1. date string
return:
  SUCC---week(0-Sunday), FAIL---date format is invalid


功能: 根据日期字符串获取星期
参数:
  参数1-日期时间字符串
返回: SUCC---星期(0-星期天),FAIL---日期格式错误
*/
int timeutil_getWeek(const char* );

/*
function: get current year
parameter:
return:
  SUCC---current year, FAIL---error

功能: 获取当前年份
参数:
返回: SUCC---当前年份,FAIL---错误
*/
int timeutil_getCurrYear(void);

/*
function: get current month
parameter:
return:
  SUCC---current month, FAIL---error

功能: 获取当前月份
参数:
返回: SUCC---当前月份,FAIL---错误
*/
int timeutil_getCurrMonth(void);

/*
function: get current day
parameter:
return:
  SUCC---current day, FAIL---error
功能: 获取当前日期
参数:
返回: SUCC---当前日期,FAIL---错误
*/
int timeutil_getCurrDay(void);


#endif