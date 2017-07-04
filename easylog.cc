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

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("easylog.config_file", "", PHP_INI_SYSTEM, OnUpdateString, config_file, zend_easylog_globals, easylog_globals)
PHP_INI_END()

PHP_GINIT_FUNCTION(easylog){
	easylog_globals->config_file = "";
}

PHP_METHOD(easylog, __construct) {
	RETURN_TRUE;
}

PHP_METHOD(easylog, info) {

	char *slog = NULL;
	size_t slog_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &slog, &slog_len) == FAILURE) {
		return;
	}

	LOG(INFO) << slog;

	RETURN_TRUE;
}

PHP_METHOD(easylog, warning) {

	char *slog = NULL;
	size_t slog_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &slog, &slog_len) == FAILURE) {
		return;
	}

	LOG(WARNING) << slog;

	RETURN_TRUE;
}

PHP_METHOD(easylog, error) {

	char *slog = NULL;
	size_t slog_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &slog, &slog_len) == FAILURE) {
		return;
	}

	LOG(ERROR) << slog;

	RETURN_TRUE;
}

PHP_METHOD(easylog, debug) {

	char *slog = NULL;
	size_t slog_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &slog, &slog_len) == FAILURE) {
		return;
	}

	LOG(DEBUG) << slog;

	RETURN_TRUE;
}

PHP_METHOD(easylog, trace) {

	char *slog = NULL;
	size_t slog_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &slog, &slog_len) == FAILURE) {
		return;
	}

	LOG(TRACE) << slog;

	RETURN_TRUE;
}

static const zend_function_entry easylog_methods[] = {
	PHP_ME(easylog, __construct, arginfo__construct, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, info, arginfo_info, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, warning, arginfo_info, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, error, arginfo_info, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, debug, arginfo_info, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(easylog, trace, arginfo_info, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in easylog_functions[] */
};

PHP_MINIT_FUNCTION(easylog)
{
	REGISTER_INI_ENTRIES();

	zend_class_entry ce;
   	INIT_CLASS_ENTRY(ce, "Easylog", easylog_methods);

	easylog_ce = zend_register_internal_class(&ce TSRMLS_CC);

	if (EASYLOG_G(config_file) != "") {
		el::Configurations conf(EASYLOG_G(config_file));
    	el::Loggers::reconfigureAllLoggers(conf);
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
