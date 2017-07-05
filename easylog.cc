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
	ZEND_ARG_INFO(0, slog)
ZEND_END_ARG_INFO()

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("easylog.config_file", "", PHP_INI_SYSTEM, OnUpdateString, config_file, zend_easylog_globals, easylog_globals)
PHP_INI_END()

PHP_GINIT_FUNCTION(easylog){
	easylog_globals->config_file = "";
}

//write log
static void write_log(el::Level level, INTERNAL_FUNCTION_PARAMETERS) {

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
static el::Level level_conversion(long level) {
	el::Level elevel;
	switch(level) {
		case EASYLOG_GLOBAL_LEVEL:
			elevel = el::Level::Global;
			break;
		case EASYLOG_TRACE_LEVEL:
			elevel = el::Level::Trace;
			break;
		case EASYLOG_DEBUG_LEVEL:
			elevel = el::Level::Debug;
			break;
		case EASYLOG_FATAL_LEVEL:
			elevel = el::Level::Fatal;
			break;
		case EASYLOG_ERROR_LEVEL:
			elevel = el::Level::Error;
			break;
		case EASYLOG_WARNING_LEVEL:
			elevel = el::Level::Warning;
			break;
		case EASYLOG_VERBOSE_LEVEL:
			elevel = el::Level::Verbose;
			break;
		case EASYLOG_INFO_LEVEL:
			elevel = el::Level::Info;
			break;
		case EASYLOG_UNKNOWN_LEVEL:
			elevel = el::Level::Unknown;
			break;
		default:
			elevel = el::Level::Global;
			break;
	}

	return elevel;
}

PHP_METHOD(easylog, __construct) {

	char *logger_id = NULL;
	zval *self 	= getThis();
	size_t len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|s", &logger_id, &len) == FAILURE) {
		return;
	}

	if (logger_id != NULL) {
		zend_update_property_string(easylog_ce, self, ZEND_STRL(EASYLOG_PROPERTY_LOGGER_ID), logger_id TSRMLS_DC);
	}

	RETURN_TRUE;
}

PHP_METHOD(easylog, setLevel) {

	long level = NULL;
	el::Level elevel;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &level) == FAILURE) {
		return;
	}

	//转换级别
	elevel = level_conversion(level);

	// 设置级别门阀值，修改参数可以控制日志输出
    el::Loggers::setLoggingLevel(elevel);
}

PHP_METHOD(easylog, info) {
	write_log(el::Level::Info, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, warning) {
	write_log(el::Level::Warning, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, error) {
	write_log(el::Level::Error, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, debug) {
	write_log(el::Level::Debug, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, trace) {
	write_log(el::Level::Trace, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, fatal) {
	write_log(el::Level::Fatal, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(easylog, verbose) {
	write_log(el::Level::Verbose, INTERNAL_FUNCTION_PARAM_PASSTHRU);
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
	PHP_ME(easylog, setLevel, arginfo_level, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in easylog_functions[] */
};

PHP_MINIT_FUNCTION(easylog)
{
	REGISTER_INI_ENTRIES();

	zend_class_entry ce;
   	INIT_CLASS_ENTRY(ce, "Easylog", easylog_methods);

	easylog_ce = zend_register_internal_class(&ce TSRMLS_CC);

	if (EASYLOG_G(config_file) != "") {
		el::Loggers::configureFromGlobal(EASYLOG_G(config_file));
	}

	zend_declare_property_string(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_LOGGER_ID), EASYLOG_DEFAULT_LOGGER_ID, ZEND_ACC_PUBLIC TSRMLS_CC);

	//注册常量
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_GLOBAL_LEVEL), EASYLOG_GLOBAL_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_TRACE_LEVEL), EASYLOG_TRACE_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_DEBUG_LEVEL), EASYLOG_DEBUG_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_FATAL_LEVEL), EASYLOG_FATAL_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_ERROR_LEVEL), EASYLOG_ERROR_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_WARNING_LEVEL), EASYLOG_WARNING_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_VERBOSE_LEVEL), EASYLOG_VERBOSE_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_INFO_LEVEL), EASYLOG_INFO_LEVEL TSRMLS_CC);
	zend_declare_class_constant_long(easylog_ce, ZEND_STRL(EASYLOG_PROPERTY_UNKNOWN_LEVEL), EASYLOG_UNKNOWN_LEVEL TSRMLS_CC);

	// 防止Fatal级别日志中断程序
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    // 选择划分级别的日志
    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);

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
