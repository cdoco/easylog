/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  ZiHang Gao <ocdoco@gmail.com>                               |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_easylog.h"
}

#include "easyloggingpp/easylogging++.h"

using namespace el;

INITIALIZE_EASYLOGGINGPP

static int le_easylog;
zend_class_entry *easylog_ce;

ZEND_DECLARE_MODULE_GLOBALS(easylog)

ZEND_BEGIN_ARG_INFO_EX(arginfo__construct, 0, 0, 1)
	ZEND_ARG_INFO(0, conf)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_info, 0, 0, 1)
	ZEND_ARG_INFO(0, slog)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_level, 0, 0, 1)
	ZEND_ARG_INFO(0, level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_config, 0, 0, 2)
	ZEND_ARG_INFO(0, level)
	ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()


PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("easylog.config_file", "", PHP_INI_SYSTEM, OnUpdateString, config_file, zend_easylog_globals, easylog_globals)
	STD_PHP_INI_ENTRY("easylog.log_level", "global", PHP_INI_SYSTEM, OnUpdateString, log_level, zend_easylog_globals, easylog_globals)
PHP_INI_END()

PHP_GINIT_FUNCTION(easylog){
	easylog_globals->config_file = "";
	easylog_globals->log_level = "global";
}

//write log
static void write_log(Level level, INTERNAL_FUNCTION_PARAMETERS) {

	char *slog = NULL;
	size_t slog_len;
	zval *self 	= getThis();
	zval *logger_id = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &slog, &slog_len) == FAILURE) {
		return;
	}

	logger_id = zend_read_property(easylog_ce, self, ZEND_STRL(EASYLOG_PROPERTY_LOGGER_ID), 1, NULL TSRMLS_DC);

	switch(level) {
		case el::Level::Info:
			CLOG(INFO, Z_STRVAL_P(logger_id)) << slog;
			break;
		case el::Level::Debug:
			CLOG(DEBUG, Z_STRVAL_P(logger_id)) << slog;
			break;
		case el::Level::Warning:
			CLOG(WARNING, Z_STRVAL_P(logger_id)) << slog;
			break;
		case el::Level::Error:
			CLOG(ERROR, Z_STRVAL_P(logger_id)) << slog;
			break;
		case el::Level::Trace:
			CLOG(TRACE, Z_STRVAL_P(logger_id)) << slog;
			break;
		case el::Level::Fatal:
			CLOG(FATAL, Z_STRVAL_P(logger_id)) << slog;
			break;
		case el::Level::Verbose:
			VLOG(0) << slog;
			break;
		default:
			break;
	}

	RETURN_TRUE;
}

//转换成 easylogging++ 的级别
static Level level_conversion(char *level) {

	// global
	if (!strcmp(level, EASYLOG_LEVEL_GLOBAL)) {
		return Level::Global;
	}

	// trace
	if (!strcmp(level, EASYLOG_LEVEL_TRACE)) {
		return Level::Trace;
	}

	// debug
	if (!strcmp(level, EASYLOG_LEVEL_DEBUG)) {
		return Level::Debug;
	}

	// fatal
	if (!strcmp(level, EASYLOG_LEVEL_FATAL)) {
		return Level::Fatal;
	}

	// error
	if (!strcmp(level, EASYLOG_LEVEL_ERROR)) {
		return Level::Error;
	}

	// warning
	if (!strcmp(level, EASYLOG_LEVEL_WARNING)) {
		return Level::Warning;
	}

	// verbose
	if (!strcmp(level, EASYLOG_LEVEL_VERBOSE)) {
		return Level::Verbose;
	}

	// info
	if (!strcmp(level, EASYLOG_LEVEL_INFO)) {
		return Level::Info;
	}

	// unknown
	if (!strcmp(level, EASYLOG_LEVEL_UNKNOWN)) {
		return Level::Unknown;
	}

	return Level::Global;
}

PHP_METHOD(easylog, __construct) {

	char *logger_id = NULL;
	zval *self 	= getThis();
	size_t len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|s", &logger_id, &len) == FAILURE) {
		return;
	}

	if (logger_id != NULL) {
		//修复配置文件没有要新建记录器
		Loggers::getLogger(logger_id);
		zend_update_property_string(easylog_ce, self, ZEND_STRL(EASYLOG_PROPERTY_LOGGER_ID), logger_id TSRMLS_DC);
	}

	RETURN_TRUE;
}

PHP_METHOD(easylog, setLevel) {

	char *level = NULL;
	size_t len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &level, &len) == FAILURE) {
		return;
	}

	if (level != NULL) {
		// 设置级别门阀值，修改参数可以控制日志输出
    	Loggers::setLoggingLevel(level_conversion(level));
	}
}

//设置单个日志的配置文件
static void set_config(ConfigurationType type, INTERNAL_FUNCTION_PARAMETERS) {

	zval *self = getThis();
	zval *logger_id = NULL;
	char *level = NULL, *config = NULL;
	size_t level_len = 0, config_len = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &level, &level_len, &config, &config_len) == FAILURE) {
		return;
	}

	//获取 loggerid
	logger_id = zend_read_property(easylog_ce, self, ZEND_STRL(EASYLOG_PROPERTY_LOGGER_ID), 1, NULL TSRMLS_DC);

	//设置单个日志的配置
	Configurations c;
    c.set(level_conversion(level), type, config);
	Loggers::reconfigureLogger(Z_STRVAL_P(logger_id), c);

	RETURN_TRUE;
}

PHP_METHOD(easylog, setFileName) {
	set_config(ConfigurationType::Filename, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setFormat) {
	set_config(ConfigurationType::Format, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setToFile) {
	set_config(ConfigurationType::ToFile, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setMaxLogFileSize) {
	set_config(ConfigurationType::MaxLogFileSize, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setToStandardOutput) {
	set_config(ConfigurationType::ToStandardOutput, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setPerformanceTracking) {
	set_config(ConfigurationType::PerformanceTracking, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setLogFlushThreshold) {
	set_config(ConfigurationType::LogFlushThreshold, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, setMillisecondsWidth) {
	set_config(ConfigurationType::MillisecondsWidth, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, info) {
	write_log(Level::Info, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, warning) {
	write_log(Level::Warning, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, error) {
	write_log(Level::Error, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, debug) {
	write_log(Level::Debug, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, trace) {
	write_log(Level::Trace, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, fatal) {
	write_log(Level::Fatal, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, verbose) {
	write_log(Level::Verbose, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

static const zend_function_entry easylog_methods[] = {
	PHP_ME(easylog, __construct, arginfo__construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, info, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, warning, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, error, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, debug, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, trace, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, fatal, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, verbose, arginfo_info, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setLevel, arginfo_level, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setFormat, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setFileName, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setToFile, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setMaxLogFileSize, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setToStandardOutput, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setPerformanceTracking, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setLogFlushThreshold, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_ME(easylog, setMillisecondsWidth, arginfo_config, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in easylog_functions[] */
};

PHP_MINIT_FUNCTION(easylog)
{
	REGISTER_INI_ENTRIES();

	zend_class_entry ce;
   	INIT_CLASS_ENTRY(ce, "Easylog", easylog_methods);

	easylog_ce = zend_register_internal_class(&ce TSRMLS_CC);

	if (EASYLOG_G(config_file) != "") {
		Loggers::configureFromGlobal(EASYLOG_G(config_file));
	}

	zend_declare_property_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LOGGER_ID), EASYLOG_DEFAULT_LOGGER_ID, ZEND_ACC_PUBLIC TSRMLS_CC);

	//注册常量
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_GLOBAL), EASYLOG_LEVEL_GLOBAL TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_TRACE), EASYLOG_LEVEL_TRACE TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_DEBUG), EASYLOG_LEVEL_DEBUG TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_FATAL), EASYLOG_LEVEL_FATAL TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_ERROR), EASYLOG_LEVEL_ERROR TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_WARNING), EASYLOG_LEVEL_WARNING TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_VERBOSE), EASYLOG_LEVEL_VERBOSE TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_INFO), EASYLOG_LEVEL_INFO TSRMLS_CC);
	zend_declare_class_constant_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LEVEL_UNKNOWN), EASYLOG_LEVEL_UNKNOWN TSRMLS_CC);

	// 防止Fatal级别日志中断程序
    Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    // 选择划分级别的日志
    Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);

	if (EASYLOG_G(log_level)) {
		// 设置级别门阀值，修改参数可以控制日志输出
    	Loggers::setLoggingLevel(level_conversion(EASYLOG_G(log_level)));
	}

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(easylog)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(easylog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "easylog support", "enabled");
	php_info_print_table_row(2, "Version", PHP_EASYLOG_VERSION);
	php_info_print_table_row(2, "Log Level", "global trace debug fatal error warning verbose info unknown");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

zend_module_entry easylog_module_entry = {
	STANDARD_MODULE_HEADER,
	"easylog",
	easylog_methods,
	PHP_MINIT(easylog),
	PHP_MSHUTDOWN(easylog),
	NULL,		/* Replace with NULL if there's nothing to do at request start */
	NULL,	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(easylog),
	PHP_EASYLOG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EASYLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(easylog)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
