<?php
	//设置 logger ID
	//这个 id 必须在配置文件中存在
	$easylog = new Easylog("easylog");

	//设置日志级别
	$easylog->setLevel(Easylog::ERROR);

	$easylog->info("info");
	$easylog->warning("warning");
	$easylog->error("error");
	$easylog->debug("debug");
