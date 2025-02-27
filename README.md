# 这是关于日期和时间的函数  
 
- 支持日期格式: YYYYMMDD(默认),YYYY-MM-DD,YYYY.MM.DD,YYYY/MM/DD,MM/DD/YY,DD/MM/YY,MM/DD/YYYY,DD/MM/YYYY  
- 支持时间格式: HHMMSS(默认),HHMMSSBBB,HH:MM:SS,HH:MM:SS:BBB  

1. 如果入参若包含日期或时间,首先会自动检测, 如果不合法,则取默认值,如果仍然不合法则报错  
2. 如果出参是日期或时间, 则取默认值  
3. 如果函数是日期或时间格式转换,则入参的源和目标都需要带格式说明  
4. 如果日期和月份均小于12,MM/DD/YY,DD/MM/YY,MM/DD/YYYY,DD/MM/YYYY自动检测都会报错  
5. 如果年份格式是YY, 默认是20YY  
6. timeutil_getDateTime 函数返回固定格式  


# There are function about date and time  
- Support Date Format: YYYYMMDD(default)
                       YYYY-MM-DD
                       YYYY.MM.DD
                       YYYY/MM/DD
                       MM/DD/YY
                       DD/MM/YY
                       MM/DD/YYYY
                       DD/MM/YYYY
- Supoort Time Format: HHMMSS(default)
                       HHMMSSBBB
                       HH:MM:SS
                       HH:MM:SS:BBB  

1. if the function's parameters include a date or time, they will first be automatically checked. If they are invalid, the default values will be used. If they are still invalid, an error will be return  
2. if the return value is a date or time, the default value will be used  
3. if the function is for date or time format conversion, both the source and target parameters must include format specifications  
4. if both the day and month are less than 12, automatic detection for MM/DD/YY, DD/MM/YY, MM/DD/YYYY, and DD/MM/YYYY formats will result in an error  
5. if the year format is 'YY', it defaults to '20YY'  
6. timeutil_getDateTime function returns a fixed format  