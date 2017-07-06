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

	$easylog = new Easylog("zy");

	$easylog->setFormat(Easylog::GLOBAL, "[%datetime{%Y-%M-%d %H:%m:%s}] %logger.%level | %msg");

	$easylog->info("info");
	$easylog->warning("warning");
	$easylog->error("error");
	$easylog->debug("debug");
