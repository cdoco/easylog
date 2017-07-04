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
```
### easylog.conf

```conf
* GLOBAL:
    ENABLED                 =   true
    TO_FILE                 =   true
    TO_STANDARD_OUTPUT      =   true
    FORMAT                  =   "[%level | %datetime] | %msg"
    FILENAME                =   "/data/logs/php/log_%datetime{%Y%M%d}.log"
    MILLISECONDS_WIDTH      =   3
    PERFORMANCE_TRACKING    =   false
    MAX_LOG_FILE_SIZE       =   1048576
    LOG_FLUSH_THRESHOLD     =   0

* TRACE:
    FILENAME                =   "/data/logs/php/trace_%datetime{%Y%M%d}.log"

* DEBUG:
    FILENAME                =   "/data/logs/php/debug_%datetime{%Y%M%d}.log"

* ERROR:
    FILENAME                =   "/data/logs/php/error_%datetime{%Y%M%d}.log"

* WARNING:
    FILENAME                =   "/data/logs/php/warning_%datetime{%Y%M%d}.log"

* INFO:
    FILENAME                =   "/data/logs/php/info_%datetime{%Y%M%d}.log"
```

### easylog.php

```php
<?php
    $easylog = new Easylog();

    $easylog->info("info");
    $easylog->warning("warning");
    $easylog->error("error");
    $easylog->debug("debug");
    $easylog->trace("trace");
```
