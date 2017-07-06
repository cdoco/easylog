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

	$easylog2 = new Easylog("zy");

	$easylog2->setFormat(Easylog::GLOBAL, "[%datetime{%Y-%M-%d %H:%m:%s}] %logger.%level | %msg");

	$easylog2->info("info");
	$easylog2->warning("warning");
	$easylog2->error("error");
	$easylog2->debug("debug");
