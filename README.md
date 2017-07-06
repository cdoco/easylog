# Easylog
Logging system based on [Easylogging++](https://github.com/muflihun/easyloggingpp)

## Installation

```shell
phpize && ./configure && make && make install
```

## Basic Usage

### php.ini

```ini
extension=easylog.so

[easylog]
easylog.config_file = "/data/easylog.conf"

// Level: global trace debug fatal error warning verbose info unknown
easylog.log_level = "info"
```

### Quick start

```php
<?php
    //设置日志级别
    Easylog::setLevel(Easylog::ERROR);

    //设置 logger ID
    //这个 id 必须在配置文件中存在
    $easylog = new Easylog("easylog");

    $easylog->info("info");
    $easylog->warning("warning");
    $easylog->error("error");
    $easylog->debug("debug");
    $easylog->trace("trace");
    $easylog->fatal("fatal");

    $zy = new Easylog("zy");

    //Determines format of logging corresponding level and logger.
    $zy->setFormat(Easylog::GLOBAL, "[%datetime{%Y-%M-%d %H:%m:%s}] %logger.%level | %msg");

    //Whether or not to write corresponding log to log file
    $zy->setToFile(Easylog::GLOBAL, "true");

    //Determines log file (full path) to write logs to for correponding level and logger
    $zy->setFileName(Easylog::GLOBAL, "/data/logs/php/zy.log");

    //If file size of corresponding log file (for corresponding level) is >= specified size, log file will be truncated and re-initiated.
    $zy->setMaxLogFileSize(Easylog::GLOBAL, "2097152");

    //Whether or not to write corresponding level and logger log to standard output
    $zy->setToStandardOutput(Easylog::GLOBAL, "true");

    //This does not depend on logger or level. Performance tracking always uses 'performance' logger
    $zy->setPerformanceTracking(Easylog::GLOBAL, "false");

    //Specifies number of log entries to hold until we flush pending log data
    $zy->setLogFlushThreshold(Easylog::GLOBAL, "1");

    //Alias of SubsecondPrecision (for backward compatibility)
    $zy->setMillisecondsWidth(Easylog::GLOBAL, "3");

    $zy->info("info");
    $zy->warning("warning");
```

### Configuring file

```conf
-- default // logger id
    * GLOBAL:
        ENABLED                 =   true
        TO_FILE                 =   true
        TO_STANDARD_OUTPUT      =   true
        FORMAT                  =   "[%datetime{%Y-%M-%d %H:%m:%s}] %logger.%level | %msg"
        FILENAME                =   "/data/logs/php/log_%datetime{%Y%M%d}.log"
        MILLISECONDS_WIDTH      =   3
        PERFORMANCE_TRACKING    =   false
        MAX_LOG_FILE_SIZE       =   1048576
        LOG_FLUSH_THRESHOLD     =   0

    * TRACE:
        FILENAME                =   "/data/logs/php/trace_%datetime{%Y%M%d}.log"

    * DEBUG:
        FILENAME                =   "/data/logs/php/debug_%datetime{%Y%M%d}.log"

    * FATAL:
        ENABLED                 =   false 

    * ERROR:
        FILENAME                =   "/data/logs/php/error_%datetime{%Y%M%d}.log"

    * WARNING:
        FILENAME                =   "/data/logs/php/warning_%datetime{%Y%M%d}.log"

    * INFO:
        FILENAME                =   "/data/logs/php/info_%datetime{%Y%M%d}.log"

    * VERBOSE:  
        ENABLED                 =   false

-- easylog // logger id
    * GLOBAL:
        ENABLED                 =   true
        TO_FILE                 =   true
        TO_STANDARD_OUTPUT      =   true
        FORMAT                  =   "[%datetime{%Y-%M-%d %H:%m:%s}] %logger.%level | %msg"
        FILENAME                =   "/data/logs/php/easylog/log_%datetime{%Y%M%d}.log"
        MILLISECONDS_WIDTH      =   3
        PERFORMANCE_TRACKING    =   false
        MAX_LOG_FILE_SIZE       =   1048576
        LOG_FLUSH_THRESHOLD     =   0

    * INFO:
        FILENAME                =   "/data/logs/php/easylog/info_%datetime{%Y%M%d}.log"

    * ERROR:
        FILENAME                =   "/data/logs/php/easylog/error_%datetime{%Y%M%d}.log"
```

#### Configuration Options

|   Configuration Name  |   Type   |                 Description                                                                                                                                                 |
|-----------------------|----------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `Enabled`               |   bool   | Determines whether or not corresponding level for logger is enabled. You may disable all logs by using |
| `To_File`               |   bool   | Whether or not to write corresponding log to log file  |
| `To_Standard_Output`    |   bool   | Whether or not to write logs to standard output e.g, terminal or command prompt  |
| `Format`                |   char*  | Determines format/pattern of logging for corresponding level and logger.         |
| `Filename`              |   char*  | Determines log file (full path) to write logs to for corresponding level and logger   |
| `Subsecond_Precision`   |   uint   | Specifies subsecond precision (previously called 'milliseconds width'). Width can be within range (1-6)  |
| `Performance_Tracking`  |   bool   | Determines whether or not performance tracking is enabled. This does not depend on logger or level. Performance tracking always uses 'performance' logger unless specified|
| `Max_Log_File_Size`     |   size_t | If log file size of corresponding level is >= specified size, log file will be truncated.    |
| `Log_Flush_Threshold`   |  size_t  | Specifies number of log entries to hold until we flush pending log data.    |                               

#### Date/Time Format Specifiers
You can customize date/time format using following specifiers

|    Specifier    |                 Replaced By                                                                                      |
|-----------------|------------------------------------------------------------------------------------------------------------------|
| `%d`            | Day of month (zero-padded)                                                                                       |
| `%a`            | Day of the week - short (Mon, Tue, Wed, Thu, Fri, Sat, Sun)                                                      |
| `%A`            | Day of the week - long (Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday)                          |
| `%M`            | Month (zero-padded)                                                                                              |
| `%b`            | Month - short (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec)                                       |
| `%B`            | Month - Long (January, February, March, April, May, June, July, August, September, October, November, December)  |
| `%y`            | Year - Two digit (13, 14 etc)                                                                                    |
| `%Y`            | Year - Four digit (2013, 2014 etc)                                                                               |
| `%h`            | Hour (12-hour format)                                                                                            |
| `%H`            | Hour (24-hour format)                                                                                            |
| `%m`            | Minute (zero-padded)                                                                                             |
| `%s`            | Second (zero-padded)                                                                                             |
| `%g`            | Subsecond part (precision is configured by ConfigurationType::SubsecondPrecision)                               |
| `%F`            | AM/PM designation                                                                                                |
| `%`             | Escape character                                                                                                 |
