#include "timeutil.h"
#ifdef WIN32
#include <stdint.h>
#include "windows.h"
#endif

// default date and time format
char TIMEUTIL_DATEFORMAT[DATELEN]="YYYYMMDD";
char TIMEUTIL_TIMEFORMAT[TIMELEN]="HHMMSS";

typedef struct
{
  int year;
  int month;
  int day;
}Date,*DatePtr;

typedef struct
{
  int hour;
  int minute;
  int second;
  int millisecond;
}Time,*TimePtr;

typedef int (*ParseDateTimeStrToStruct)(const char*,const char*, void* );
typedef int (*ConvertToStrFromStruct)(const char* ,char* ,size_t ,void* );

typedef struct {
  char* dateTimeFormatStr;
  char* dateTimeInputFormat;
  ParseDateTimeStrToStruct parseFunc;
  ConvertToStrFromStruct ConvertFunc;
} DateTimeFormatParser;

static int parseYMD(const char* datestr,const char* inputformat, void* data)
{
DatePtr date = data;

/* check digit */
for(int i=0;i<strlen(datestr);i++){
  if ((i != 4) && (i != 7)){
      if (!isdigit(datestr[i]))
        return FAIL;
  }
}

return sscanf(datestr,inputformat,&date->year,&date->month,&date->day);
}

/*
MM/DD/YY,MM/DD/YYYY
*/
static int parseMDY(const char* datestr,const char* inputformat, void* data) 
{
DatePtr date = data;
int ret = 0;

/* check digit */
for(int i=0;i<strlen(datestr);i++){
  if ((i != 2) && (i != 5)){
      if (!isdigit(datestr[i]))
        return FAIL;
  }
}

ret = sscanf(datestr,inputformat, &date->month, &date->day, &date->year);

if (strlen(datestr) == 8)
  date->year += 2000;

return ret;  
}

/*
DD/MM/YY,DD/MM/YYYY
*/
static int parseDMY(const char* datestr,const char* inputformat,  void* data) 
{
DatePtr date = data;
int ret = 0;

/* 校验日期是否均为数字 */
for(int i=0;i<strlen(datestr);i++){
  if ((i != 2) && (i != 5)){
      if (!isdigit(datestr[i]))
        return FAIL;
  }
}

ret = sscanf(datestr,inputformat, &date->day, &date->month, &date->year);

if (strlen(datestr) == 8)
  date->year += 2000;

return ret;
}

/* 
    build date string
*/
static int buildYMD(const char* inputformat,char* datestr,size_t len,void* data)
{
DatePtr date = data;

return snprintf(datestr,len,inputformat,date->year,date->month,date->day);
}

/*
MM/DD/YYYY和MM/DD/YY
*/
static int buildMDY(const char* inputformat,char* datestr,size_t len,void* data)
{
DatePtr date = data;

if (strlen(datestr) == 8)
  return snprintf(datestr,len,inputformat,date->month,date->day,date->year % 100);
else
  return snprintf(datestr,len,inputformat,date->month,date->day,date->year);
}

/*
DD/MM/YYYY和DD/MM/YY
*/
static int buildDMY(const char* inputformat,char* datestr,size_t len,void* data)
{
DatePtr date = data;

if (strlen(datestr) == 8)
  return snprintf(datestr,len,inputformat,date->day,date->month,date->year % 100);
else
  return snprintf(datestr,len,inputformat,date->day,date->month,date->year);
}

/*
时间解析及构建函数
*/
static int parseHMS(const char* timestr,const char* inputformat, void* data)
{
TimePtr time = data;

time->millisecond = 0;

return sscanf(timestr,inputformat,&time->hour,&time->minute,&time->second);
}

static int parseHMSB(const char* timestr,const char* inputformat,  void* data)
{
TimePtr time = data;

return sscanf(timestr,inputformat,&time->hour,&time->minute,&time->second,&time->millisecond);
}

static int buildHMS(const char* inputformat,char* timestr,size_t len, void* data)
{
TimePtr time = data;

return snprintf(timestr,len,inputformat,time->hour,time->minute,time->second);
}

static int buildHMSB(const char* inputformat,char* timestr,size_t len, void* data)
{
TimePtr time = data;

return snprintf(timestr,len,inputformat,time->hour,time->minute,time->second,time->millisecond);
}

DateTimeFormatParser parseDate[] = {
    {"YYYYMMDD","%04d%02d%02d",parseYMD,buildYMD},
    {"YYYY-MM-DD","%04d-%02d-%02d",parseYMD,buildYMD},
    {"YYYY/MM/DD","%04d/%02d/%02d",parseYMD,buildYMD},
    {"YYYY.MM.DD","%04d.%02d.%02d",parseYMD,buildYMD},
    {"MM/DD/YYYY","%02d/%02d/%d",parseMDY,buildMDY},
    {"DD/MM/YYYY","%02d/%02d/%d",parseDMY,buildDMY},
    {"MM/DD/YY","%02d/%02d/%d",parseMDY,buildMDY},
    {"DD/MM/YY","%02d/%02d/%d",parseDMY,buildDMY}
};

DateTimeFormatParser parseTime[] = {
  {"HHMMSS","%02d%02d%02d",parseHMS,buildHMS},
  {"HH:MM:SS","%02d:%02d:%02d",parseHMS,buildHMS},
  {"HHMMSSBBB","%02d%02d%02d%03d",parseHMSB,buildHMSB},
  {"HH:MM:SS:BBB","%02d:%02d:%02d:%03d",parseHMSB,buildHMSB},
};

/*
功能:判断是否字符串全是数字
参数:
  参数1--字符串
返回:SUCC---数字字符串,FAIL---非数字字符串
*/
static int _isdigitstr(const char* str)
{
while (*str != '\0')
{
  if (!isdigit(*str++))
    return FAIL;
}

return SUCC;
}

/*
功能:判断是否是闰年
参数:
  参数1--年份
返回:SUCC---闰年,1---非闰年
*/
static int _isleapyear(int year)
{
  int c1,c2,c3;

/*   if((year<1900) || (year>9999)){
    return FAIL;
  } */
  c1=year%4;
  c2=year%100;
  c3=year%400;
  if((c1==0 && c2>0) || (c1==0 && c2==0 && c3==0)){
    return SUCC;
  }
  else{
    return 1;
  }
}

/* -------------------以下是日期相关函数--------------------------- */

/*
功能:检查日期是否合法
参数:
  参数1---日期结构体
返回: SUCC---合法，FAIL---非法
*/
static int _splitdate(DatePtr date)
{
if(date->year < 1900 || date->year > 9999)
  return FAIL;

if ((date->day < 1) || (date->day > 31))
  return FAIL;

switch(date->month){
  case 2:
    if(_isleapyear(date->year)>0){
      if(date->day>28){
        return FAIL;
      }
    }
    else{
      if(date->day>29){
        return FAIL;
      }
    }
    break;
  case 4:case 6:case 9:case 11:
    if(date->day>30){
      return FAIL;
    }
    break;
  case 1:case 3:case 5:case 7:case 8:case 10:case 12:
      break;
  default:
    return FAIL;
    break;
}

return SUCC;
}

/*
功能: 获取当前时间与GMT的偏移
返回: 偏移小时(负数代表东区,正数代表西区)
*/
static int _getOffsetTimeOfZone(void)
{

tzset();

return (int)timezone/3600;
}
/*
功能:将日期结构体转换成时间戳
参数:
  参数1--日期结构体
返回: 正确---时间戳,FAIL---日期格式错误
*/
static time_t _DateStructToTS(DatePtr date)
{
#ifdef LINUX
struct tm tm={0};

tm.tm_mday = date->day;
tm.tm_mon = date->month - 1;
tm.tm_year = date->year - 1900;
tm.tm_hour = 0 - _getOffsetTimeOfZone();
tm.tm_min = 0;
tm.tm_sec = 0;

return mktime(&tm);
/*
#elif WIN32 
SYSTEMTIME st = {0};
FILETIME ft={0};
ULARGE_INTEGER uli;

st.wYear = date->year;
st.wMonth = date->month;
st.wDay = date->day;

SystemTimeToFileTime(&st, &ft);
uli.LowPart = ft.dwLowDateTime;
uli.HighPart = ft.dwHighDateTime;
   
const uint64_t DIFF_BETWEEN_EPOCHS = 11644473600ULL;

return (uli.QuadPart/10000000ULL) - DIFF_BETWEEN_EPOCHS;
*/
#endif
}

/*
功能: 将时间戳放到日期和时间结构体
参数:
  参数1--时间戳
  参数2--日期结构体
  参数3--时间结构体
返回:  成功---日期和时间结构体, 其余---失败
*/
static int _TsToDateTimeStruct(time_t n,DatePtr date,TimePtr time)
{
struct tm result={0};

tzset();

if (localtime_r(&n,&result) == NULL)
  return FAIL;

if (result.tm_year >= 8099)
  return FAIL;

if (date != NULL){
    date->year = result.tm_year + 1900;
    date->month = result.tm_mon + 1;
    date->day = result.tm_mday;
}

if (time != NULL){
    time->hour = result.tm_hour;
    time->minute = result.tm_min;
    time->second = result.tm_sec;
}

return SUCC;
}

/*
将带有格式的日期转成结构体并校验,如果是2位的年份,默认20xx
  参数1---日期字符串
  参数2---日期格式
  参数3---日期结构体
返回: SUCC---合法，FAIL---非法
*/
static int _DateStrToStruct(const char* datestr,const char* format,DatePtr date)
{

for(int i = 0; i < sizeof(parseDate)/sizeof(parseDate[0]); i++) {
  if (
     (!strcasecmp(format, parseDate[i].dateTimeFormatStr)) &&
     (strlen(format) == strlen(parseDate[i].dateTimeFormatStr))
     )
    {
      if (parseDate[i].parseFunc(datestr, parseDate[i].dateTimeInputFormat,date) == FAIL)
        return FAIL;
      else
        return _splitdate(date);
    }
}

return FAIL;
}

/*
将日期结构体转成对应日期格式
  参数1--日期格式(YYYYMMDD,YYYY/MM/DD,YYYY-MM-DD,DD/MM/YYYY,MM/DD/YYYY,YYYY.MM.DD)
  参数2--日期字符串
  参数3--日期字符串的长度
  参数4--日期结构体
返回: 成功---日期字符串, NULL---长度不足
*/
static char* _StructToDateStr(const char* format,char* datestr,size_t len,DatePtr date)
{
char* p = datestr;

for(int i = 0; i < sizeof(parseDate)/sizeof(parseDate[0]); i++) {
  if (
      (!strcasecmp(format,parseDate[i].dateTimeFormatStr)) &&
      (strlen(format) == strlen(parseDate[i].dateTimeFormatStr)) &&
      (len > strlen(parseDate[i].dateTimeFormatStr))
     )
     {
        parseDate[i].ConvertFunc(parseDate[i].dateTimeInputFormat,datestr,len,date);
        return p;
     }
}

return NULL;
}

/*
功能: 自动检测日期格式
参数:
   参数1--日期格式字符串
   参数2--日期格式
   参数3--日期格式长度
返回: 成功---日期格式, NULL---无法判断日期格式或日期格式不对
*/
static char* _checkDateFormat(const char* datestr,char* result,size_t len)
{
char* p = result;
char temp1[2+1]={0};
char temp2[2+1]={0};
size_t datelen = strlen(datestr);

if ((datelen != 8) && (datelen != 10))
  return NULL;

/* YYYYMMDD */
if ((_isdigitstr(datestr) == SUCC) && (datelen == 8))
{
  snprintf(result,len,"YYYYMMDD");
  return p;
}

/* YYYY-MM-DD YYYY.MM.DD,YYYY/MM/DD */
if ((datestr[4] == '-') && (datestr[7] == '-'))
{
  snprintf(result,len,"YYYY-MM-DD");
  return p;
}

if ((datestr[4] == '.') && (datestr[7] == '.'))
{
  snprintf(result,len,"YYYY.MM.DD");
  return p;
}

if ((datestr[4] == '/') && (datestr[7] == '/')){
  snprintf(result,len,"YYYY/MM/DD");
  return p;
}

/* MM/DD/YYYY,DD/MM/YYYY */
if ((datestr[2] == '/') && (datestr[5] == '/'))
{
strncpy(temp1,datestr,2);
strncpy(temp2,datestr+3,2);

  if ((atoi(temp1) > 12) && (atoi(temp2) <= 12)) /* 假定前面超过12, 中间未超过12, DD/MM/YYYY */
  {
    if ( datelen == 8) 
    {
      snprintf(result,len,"DD/MM/YY");
      return p;
    }
    else if(datelen == 10)
    {
      snprintf(result,len,"DD/MM/YYYY");
      return p;
    }
  }
  else if ((atoi(temp1) <= 12) && (atoi(temp2) > 12))  /* 假定前面未超过12, 中间超过12, MM/DD/YYYY */
  {
    if ( datelen == 8) 
    {
      snprintf(result,len,"MM/DD/YY");
      return p;
    }
    else if(datelen == 10)
    {
      snprintf(result,len,"MM/DD/YYYY");
      return p;
    }
  }
  else    /* 无法判断MM/DD/YYYY,DD/MM/YYYY */ 
    return NULL;
}

return NULL;
}

/*
功能: 校验字符串是否是NULL,有没有结束\0
参数:
   参数1--字符串
   参数2--最大值
返回: SUCC---有\0, FAIL---NULL或在maxvalue范围内无\0
*/
static int _checkInputStr(const char* inputstr,int maxvalue)
{

if (inputstr == NULL)
  return FAIL;

for(int i=0;i<maxvalue;i++){
  if (inputstr[i] == '\0')
    return SUCC;
}

return FAIL;
}

int timeutil_initDateFormat(const char* dateformat)
{

if ((_checkInputStr(dateformat,DATELEN) == FAIL) || ((strlen(dateformat) != 8) && (strlen(dateformat) != 10)))
  return FAIL;

for(int i=0;i<sizeof(parseDate)/sizeof(parseDate[0]);i++)
{
  if (
       (!strcasecmp(dateformat,parseDate[i].dateTimeFormatStr)) && 
       (strlen(dateformat) == strlen(parseDate[i].dateTimeFormatStr))
     )
     {
      snprintf(TIMEUTIL_DATEFORMAT,sizeof(TIMEUTIL_DATEFORMAT),parseDate[i].dateTimeFormatStr);
      return SUCC;
     }
}

return FAIL;
}

char* timeutil_getDateFormat(char* format,size_t len)
{
char* p = format;

if (len <= strlen(TIMEUTIL_DATEFORMAT))
  return NULL;

snprintf(format,len,TIMEUTIL_DATEFORMAT);

return p;
}

int timeutil_checkDate(const char *datestr)
{
Date date={0};
char dateformat[DATELEN]={0};

if (_checkInputStr(datestr,DATELEN) == FAIL)
  return FAIL;

if (_checkDateFormat(datestr,dateformat,sizeof(dateformat)) == NULL)
    return _DateStrToStruct(datestr,TIMEUTIL_DATEFORMAT,&date);
else
    return _DateStrToStruct(datestr,dateformat,&date);
}

char* timeutil_getDate(char* str,size_t len)
{

return timeutil_convertTimestampToDate(timeutil_getTimestamp(),str,len);
}

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
char* timeutil_transDate(const char* datestr,const char* srcformat,char* dst,size_t len,const char* dstformat)
{
Date date={0};

if (
    (_checkInputStr(datestr,DATELEN) == FAIL) ||
    (_checkInputStr(srcformat,DATELEN) == FAIL) ||
    (_checkInputStr(dstformat,DATELEN) == FAIL) ||
    (len <= strlen(dstformat))
   )
  return NULL;

if (
    (!strcasecmp(srcformat,dstformat)) &&
    (strlen(srcformat) == strlen(dstformat))
    )
    {
        snprintf(dst,len,datestr);
        return dst;
    }

if (_DateStrToStruct(datestr,srcformat,&date) == FAIL)
  return NULL;

return _StructToDateStr(dstformat,dst,len,&date);
}

char* timeutil_convertTimestampToDate(time_t n,char* str,size_t len)
{
Date date={0};

if (len <= strlen(TIMEUTIL_DATEFORMAT))
  return NULL;

if (_TsToDateTimeStruct(n,&date,NULL) ==  FAIL)
  return NULL;

return _StructToDateStr(TIMEUTIL_DATEFORMAT,str,len,&date);
}

/*
功能:将日期转换成时间戳
参数:
  参数1--日期时间字符串
返回: 正确---时间戳, INVALID_DATE_VALUE---日期格式错误
*/
time_t timeutil_convertDateToTimestamp(const char* datestr)
{
Date date={0};
char dateformat[DATELEN]={0};

if (_checkInputStr(datestr,DATELEN) == FAIL)
  return INVALID_DATE_VALUE;

if (_checkDateFormat(datestr,dateformat,sizeof(dateformat)) == NULL)
    snprintf(dateformat,sizeof(dateformat),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(datestr,dateformat,&date) != SUCC)
  return INVALID_DATE_VALUE;

return _DateStructToTS(&date);
}

/*
功能:日期相加
参数:
  参数1--日期字符串
  参数2--天数(如果是负数，则日期相减)
  参数3--相加后的日期
  参数4--参数3的长度
返回:成功---相加后的日期,NULL---日期非法
*/
char* timeutil_addDate(const char *datestr,int len,char* dst,size_t dlen)
{
Date date={0};
char* p = dst;
char dateformat[DATELEN]={0};

if (_checkInputStr(datestr,DATELEN) == FAIL)
  return NULL;

if (dlen <= strlen(dateformat))
  return NULL;

if (len < 0)
  return timeutil_subDate(datestr,0-len,dst,dlen);

if (len == 0){
  snprintf(dst,dlen,"%s",datestr);
  return p;
}

if (_checkDateFormat(datestr,dateformat,sizeof(dateformat)) == NULL)
  snprintf(dateformat,sizeof(dateformat),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(datestr,dateformat,&date) != SUCC)
  return NULL;

if (_TsToDateTimeStruct(_DateStructToTS(&date) + len * TIMESTPOFDAY,&date,NULL) == FAIL)
  return NULL;

return _StructToDateStr(dateformat,dst,dlen,&date);
}

/*
功能:日期减法
参数:
  参数1--日期字符串
  参数2--天数(如果是负数，则日期相加)
  参数3--减去天数后的日期
  参数4--参数3的长度
返回:成功---减去天数的日期,NULL---日期非法
*/
char* timeutil_subDate(const char *datestr,int len,char* dst,size_t dlen)
{
Date date={0};
char* p = dst;
char dateformat[DATELEN]={0};

if (_checkInputStr(datestr,DATELEN) == FAIL)
  return NULL;

if (dlen <= strlen(dateformat))
  return NULL;

if (len < 0)
  return timeutil_addDate(datestr,0-len,dst,dlen);

if (len == 0){
  snprintf(dst,dlen,"%s",datestr);
  return p;
}

if (_checkDateFormat(datestr,dateformat,sizeof(dateformat)) == NULL)
  snprintf(dateformat,sizeof(dateformat),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(datestr,dateformat,&date) != SUCC)
  return NULL;

_TsToDateTimeStruct(_DateStructToTS(&date) - len * TIMESTPOFDAY,&date,NULL);

return _StructToDateStr(dateformat,dst,dlen,&date);
}

/*
功能:两个日期相减后的天数
参数:
  参数1--日期字符串1
  参数2--日期字符串2
返回: INVALID_DATE---日期非法,其他---相减后的天数
*/
int timeutil_calcDateDiff(const char* srcdate1,const char* srcdate2)
{
Date date1={0},date2={0};
char dateformat1[DATELEN]={0};
char dateformat2[DATELEN]={0};

if ((_checkInputStr(srcdate1,DATELEN) == FAIL) || (_checkInputStr(srcdate2,DATELEN) == FAIL))
  return INVALID_DATE;

if (_checkDateFormat(srcdate1,dateformat1,sizeof(dateformat1)) == NULL)
  snprintf(dateformat1,sizeof(dateformat1),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(srcdate1,dateformat1,&date1) != SUCC)
  return INVALID_DATE;

if (_checkDateFormat(srcdate2,dateformat2,sizeof(dateformat2)) == NULL)
  snprintf(dateformat2,sizeof(dateformat2),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(srcdate2,dateformat2,&date2) != SUCC)
  return INVALID_DATE;

return (_DateStructToTS(&date1) - _DateStructToTS(&date2))/TIMESTPOFDAY;
}

/*
功能:将日期字符串转换成长整型
参数:
  参数1--日期字符串
返回:成功---长整型的日期,FAIL---日期非法
*/
long timeutil_convertDateStrToLong(const char* datestr)
{
Date date={0};
char dateformat[DATELEN]={0};

if (_checkInputStr(datestr,DATELEN) == FAIL)
  return FAIL;

if (_checkDateFormat(datestr,dateformat,sizeof(dateformat)) == NULL)
  snprintf(dateformat,sizeof(dateformat),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(datestr,dateformat,&date) == FAIL)
  return FAIL;

return date.year * 10000L + date.month * 100 + date.day;
}

/*
功能:将长整形日期转换成字符串
参数:
  参数1--长整型日期
  参数2--日期字符串
  参数3--日期字符串长度
返回:成功---字符串日期, NULL---日期非法或长度不足
*/
char* timeutil_convertLongDateToStr(long longdate,char* datestr,size_t len)
{
Date date={0};

if ((longdate <= 10000000) || (longdate >= 99999999))
  return NULL;

if (len <= strlen(TIMEUTIL_DATEFORMAT))
  return NULL;

date.year = longdate / 10000;
date.month = (longdate / 100) % 100; 
date.day = longdate % 100;

return _StructToDateStr(TIMEUTIL_DATEFORMAT,datestr,len, &date);
}
/* -------------------以下是时间相关函数--------------------------- */

/*
功能:检查时间是否合法
参数:
  参数1--时间结构体
返回:SUCC---合法，FAIL---非法
*/
static int _splittime(TimePtr time)
{
  if ((time->minute > 59) || (time->second > 59) || (time->hour > 23))
    return FAIL;

return SUCC;
}

/*
功能:获取当前时间戳
参数:
  参数1---无
返回:当前时间戳
*/
time_t timeutil_getTimestamp(void)
{
time_t nsec;

return time(&nsec);
}

/*
功能: 自动检测时间格式
参数:
   参数1--时间格式字符串
   参数2--时间格式
   参数3--日期格式长度
返回: 成功---时间格式, NULL---时间格式不对
*/
static char* _checkTimeFormat(const char* timestr,char* result,size_t len)
{
char *p = result;
size_t timelen = strlen(timestr);

if ((timelen < 6) || (timelen > 12))
  return NULL;

if (_isdigitstr(timestr) == SUCC)
{
  if (timelen == strlen("HHMMSS"))
  {
    snprintf(result,len,"HHMMSS");
    return p;
  }
  else if (timelen == strlen("HHMMSSBBB"))
  {
    snprintf(result,len,"HHMMSSBBB");
    return p;
  }
}
else if ((timestr[2] == ':') && (timestr[5] == ':'))
{
  if (timelen == strlen("HH:MM:SS"))
  {
    snprintf(result,len,"HH:MM:SS");
    return p;
  }
  else if (timelen == strlen("HH:MM:SS:BBB"))
  {
    snprintf(result,len,"HH:MM:SS:BBB");
    return p;
  }
}

return NULL;
}

/*
将时间结构体转成对应时间格式
  参数1--时间格式
  参数2--时间字符串
  参数3--时间字符串的长度
  参数4--时间结构体
返回: 成功---时间字符串, NULL---长度不足
*/
static char* _StructToTimeStr(const char* format,char* timestr,size_t len,TimePtr time)
{
char* p = timestr;

for(int i=0;i<(sizeof(parseTime)/sizeof(parseTime[0]));i++){
  if (
      (!strcasecmp(format,parseTime[i].dateTimeFormatStr)) &&
      (strlen(format) == strlen(parseTime[i].dateTimeFormatStr)) &&
      (len > strlen(parseTime[i].dateTimeFormatStr))
    )
    {
      parseTime[i].ConvertFunc(parseTime[i].dateTimeInputFormat,timestr,len,time);
      return p;
    }
}

return NULL;
}

/*
将带有格式的时间转成结构体并校验
  参数1---时间字符串
  参数2---时间格式
  参数3---时间结构体
返回: SUCC---合法，FAIL---非法
*/
static int _TimeStrToStruct(const char* timestr,const char* format,TimePtr time)
{

  for(int i=0;i<sizeof(parseTime)/sizeof(parseTime[0]);i++){
      if (!strcmp(format, parseTime[i].dateTimeFormatStr) &&
         (strlen(format) == strlen(parseTime[i].dateTimeFormatStr))
         )
      {
        if (parseTime[i].parseFunc(timestr, parseTime[i].dateTimeInputFormat,time) >= 3)
          return _splittime(time);
      }
  }

return FAIL;
}

/*
功能: 初始化时间格式
参数:
  参数1--时间格式
返回: SUCC---成功, FAIL---日期格式非法
*/
int timeutil_initTimeFormat(const char* timeformat)
{

if (_checkInputStr(timeformat,TIMELEN) == FAIL)
  return FAIL;

for (int i=0;i<sizeof(parseTime)/sizeof(parseTime[0]);i++){
    if (!strncasecmp(timeformat,parseTime[i].dateTimeFormatStr,strlen(timeformat))){
    snprintf(TIMEUTIL_TIMEFORMAT,sizeof(TIMEUTIL_TIMEFORMAT),parseTime[i].dateTimeFormatStr);
    return SUCC;
  } 
}

return FAIL;
}

/*
功能: 返回当前时间格式
参数: 
    参数1--时间格式
    参数2--格式长度
返回: 成功---当前默认的时间格式, NULL---长度不足
*/
char* timeutil_getTimeFormat(char* timeformat,size_t len)
{
char* p = timeformat;

if (len <= strlen(TIMEUTIL_TIMEFORMAT))
  return NULL;

snprintf(timeformat,len,"%s",TIMEUTIL_TIMEFORMAT);

return p;
}

/*
功能:校验时间否合法
参数:
  参数1-时间字符串
返回:SUCC---时间合法,FAIL---时间非法
*/
int timeutil_checkTime(const char *timestr)
{
Time time;
char timeformat[TIMELEN]={0};

if (_checkInputStr(timestr,TIMELEN) == FAIL)
  return FAIL;

if (_checkTimeFormat(timestr,timeformat,sizeof(timeformat)) == NULL)
  snprintf(timeformat,sizeof(timeformat),"%s",TIMEUTIL_TIMEFORMAT);

return _TimeStrToStruct(timestr,timeformat,&time);
}

/*
功能:获取时间
参数:
  参数1--时间字符串
  参数2--时间长度
返回:正确---时间字符串,NULL---错误
*/
#ifdef LINUX
char* timeutil_getTime(char* timestr,size_t len)
{
struct timespec time1 = {0, 0};
char temp[30]={0};
char* p = timestr;

if (_checkInputStr(timestr,TIMELEN) == FAIL)
  return NULL;

if (len <= TIMELEN)
  return NULL;

clock_gettime(CLOCK_REALTIME, &time1);
if (timeutil_convertTimestampToTime(time1.tv_sec,temp,sizeof(temp)) == NULL)
  return NULL;

if ( (!strcasecmp(TIMEUTIL_TIMEFORMAT,"HHMMSSBBB")) && (len > strlen("HHMMSSBBB")) )
  snprintf(timestr,len,"%s%03ld",temp,time1.tv_nsec/1000000);
else if ((!strcasecmp(TIMEUTIL_TIMEFORMAT,"HH:MM:SS:BBB")) && (len > strlen("HH:MM:SS:BBB")) )
  snprintf(timestr,len,"%s:%03ld",temp,time1.tv_nsec/1000000);
else
  snprintf(timestr,len,"%s",temp);

return p;
}
#elif WIN32
char* timeutil_getTime(char* timestr,size_t len)
{
Time time={0};

if (_checkInputStr(timestr,TIMELEN) == FAIL)
  return NULL;

if (len <= TIMELEN)
  return NULL;

SYSTEMTIME sys_time;
GetLocalTime(&sys_time);

time.hour = sys_time.wHour;
time.minute = sys_time.wMinute;
time.second = sys_time.wSecond;
time.millisecond = sys_time.wMilliseconds;

return _StructToTimeStr(TIMEUTIL_TIMEFORMAT,timestr,len,&time);
}
#endif

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
char* timeutil_transTime(const char* srctime,const char* srcformat,char* dsttime,size_t len,const char* dstformat)
{
Time time={0};

if (
     (_checkInputStr(srctime,TIMELEN) == FAIL) ||
     (_checkInputStr(srcformat,TIMELEN) == FAIL) ||
     (_checkInputStr(dstformat,TIMELEN) == FAIL) ||
     (len <= strlen(dstformat))
    )
  return NULL;

if (_TimeStrToStruct(srctime,srcformat,&time) == FAIL)
  return NULL;

return _StructToTimeStr(dstformat,dsttime,len,&time);
}

/*
功能:将时间戳转换成时间
参数:
  参数1--时间戳
  参数2--时间字符串
  参数3--参数2长度
返回: 正确---时间字符串,NULL---错误
*/
char* timeutil_convertTimestampToTime(time_t n,char* str,size_t len)
{
Time time={0};

if (_checkInputStr(str,TIMELEN) == FAIL)
  return NULL;

if (len <= strlen(TIMEUTIL_TIMEFORMAT))
  return NULL;

_TsToDateTimeStruct(n,NULL,&time);

return _StructToTimeStr(TIMEUTIL_TIMEFORMAT,str,len,&time);
}

/* -------------------以下是日期时间相关函数--------------------------- */
/*
功能:返回毫秒级日期时间
参数:
  参数1--日期时间字符串
  参数2--参数1长度
返回:正确---毫秒级的日期时间(YYYY-MM-DD HH24:MM:SS:BBB),NULL---错误
*/
#ifdef LINUX
char* timeutil_getDateTime(char* datetimestr,size_t len)
{
struct timespec time1 = {0, 0};
char datestr[DATELEN+1] = {0};
char timestr[TIMELEN+1] = {0};
char* p = datetimestr;
Date date={0};
Time time={0};

if (_checkInputStr(datetimestr,DATELEN + TIMELEN) == FAIL)
  return NULL;

if (len <= strlen("YYYY-MM-DD HH24:MM:SS:BBB"))
  return NULL;

clock_gettime(CLOCK_REALTIME, &time1);
_TsToDateTimeStruct(time1.tv_sec,&date,&time);
time.millisecond = time1.tv_nsec/1000000;

_StructToDateStr("YYYY-MM-DD",datestr,sizeof(datestr),&date);
_StructToTimeStr("HH:MM:SS:BBB",timestr,sizeof(timestr),&time);

snprintf(datetimestr,len,"%s %s",datestr,timestr);
return p;
}
#elif WIN32
char* timeutil_getDateTime(char* datetimestr,size_t len)
{
char* p = datetimestr;
char datestr[DATELEN+1] = {0};
char timestr[TIMELEN+1] = {0};
Date date={0};
Time time={0};

if (_checkInputStr(datetimestr,DATELEN + TIMELEN) == FAIL)
  return NULL;

if (len <= strlen("YYYY-MM-DD HH24:MM:SS:BBB"))
  return NULL;

SYSTEMTIME sys_time;
GetLocalTime(&sys_time);

date.year = sys_time.wYear;
date.month = sys_time.wMonth;
date.day = sys_time.wDay;

time.hour = sys_time.wHour;
time.minute = sys_time.wMinute;
time.second = sys_time.wSecond;
time.millisecond = sys_time.wMilliseconds;

_StructToDateStr("YYYY-MM-DD",datestr,sizeof(datestr),&date);
_StructToTimeStr("HH:MM:SS:BBB",timestr,sizeof(timestr),&time);

snprintf(datetimestr,len,"%s %s",datestr,timestr);

return p;
}
#endif

/*
功能:将时间戳转换成日期时间
参数:
  参数1--时间戳
  参数2--日期字符串
  参数3--参数2长度
返回:正确---日期时间字符串,NULL---错误
*/
char* timeutil_convertTimestampToDateTime(time_t n,char* str,size_t len)
{
char *p = str;
char datestr[DATELEN]={0};
char timestr[TIMELEN]={0};
Date date={0};
Time time={0};

if (_checkInputStr(str,DATELEN + TIMELEN) == FAIL)
  return NULL;

if (len <= strlen(TIMEUTIL_DATEFORMAT) + strlen(TIMEUTIL_TIMEFORMAT) + 1)
  return NULL;

_TsToDateTimeStruct(n,&date,&time);

_StructToDateStr(TIMEUTIL_DATEFORMAT,datestr,sizeof(datestr),&date);
_StructToTimeStr(TIMEUTIL_TIMEFORMAT,timestr,sizeof(timestr),&time);

snprintf(str,len,"%s %s",datestr,timestr);

return p;
}

/* -------------------以下是其它相关函数--------------------------- */

/* 
功能: 根据日期字符串获取星期
参数:
  参数1-日期时间字符串
返回: 成功---星期(0-星期天),FAIL---日期格式错误
*/
int timeutil_getWeek(const char* datestr)
{
Date date={0};
char dateformat[DATELEN]={0};
int weekday=0;

if (_checkInputStr(datestr,DATELEN) == FAIL)
  return FAIL;

if (_isdigitstr(datestr) == FAIL)
  return FAIL;

if (_checkDateFormat(datestr,dateformat,sizeof(dateformat)) == NULL)
  snprintf(dateformat,sizeof(dateformat),"%s",TIMEUTIL_DATEFORMAT);

if (_DateStrToStruct(datestr,dateformat,&date) == FAIL)
  return FAIL;

if (date.month == 1 || date.month == 2)
{
  date.month+=12;
  date.year--;
}

weekday = (date.day+2*date.month+3*(date.month+1)/5+date.year+date.year/4-date.year/100+date.year/400)%7;

if (weekday == 6) 
  weekday = 0;
else 
  weekday++;

return weekday;
}

/*
功能: 获取当前年份
参数:
返回:成功---当前年份,FAIL---错误
*/
int timeutil_getCurrYear(void)
{
Date date={0};

if (_TsToDateTimeStruct(timeutil_getTimestamp(),&date,NULL) == FAIL)
  return FAIL;

return date.year;
}

/*
功能: 获取当前月份
参数:
返回:成功---当前月份,FAIL---错误
*/
int timeutil_getCurrMonth(void)
{
Date date={0};

if (_TsToDateTimeStruct(timeutil_getTimestamp(),&date,NULL) == FAIL)
  return FAIL;

return date.month;
}

/*
功能: 获取当前日期
参数:
返回:成功---当前日期,FAIL---错误
*/
int timeutil_getCurrDay(void)
{
Date date={0};

if (_TsToDateTimeStruct(timeutil_getTimestamp(),&date,NULL) == FAIL)
  return FAIL;

return date.day;
}
