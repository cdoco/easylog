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

	$zy = new Easylog("zy");

	$zy->setFormat(Easylog::GLOBAL, "[%datetime{%Y-%M-%d %H:%m:%s}] %logger.%level | %msg");
	$zy->setToFile(Easylog::GLOBAL, "true");
	$zy->setFileName(Easylog::GLOBAL, "/data/logs/php/zy.log");
	$zy->setMaxLogFileSize(Easylog::GLOBAL, "2097152");
	$zy->setToStandardOutput(Easylog::GLOBAL, "true");
	$zy->setPerformanceTracking(Easylog::GLOBAL, "false");
	$zy->setLogFlushThreshold(Easylog::GLOBAL, "1");
	$zy->setMillisecondsWidth(Easylog::GLOBAL, "3");

	$zy->info("info");
	$zy->warning("warning");
	$zy->error("error");
	$zy->debug("debug");
