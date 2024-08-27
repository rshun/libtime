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
功能: 初始化日期格式
参数:
  参数1--日期格式
返回: SUCC---成功, FAIL---日期格式非法
*/
int timeutil_initDateFormat(const char* );

/*
功能: 返回当前日期格式
参数: 
    参数1--日期格式
    参数2--格式长度
返回: 成功---当前默认的日期格式, NULL---长度不足
*/
char* timeutil_getDateFormat(char* ,size_t );

/*
功能:校验日期否合法
参数:
  参数1-日期字符串
返回:SUCC---日期合法,FAIL---日期非法
*/
int timeutil_checkDate(const char *);

/*
功能:获取当前日期
参数:
  参数1-日期字符串
  参数2-日期字符串长度
返回: 正确---当前日期,NULL---错误
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