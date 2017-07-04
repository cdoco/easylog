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

#ifndef PHP_EASYLOG_H
#define PHP_EASYLOG_H

extern zend_module_entry easylog_module_entry;
#define phpext_easylog_ptr &easylog_module_entry

#define PHP_EASYLOG_VERSION "0.1.0-dev" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EASYLOG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EASYLOG_API __attribute__ ((visibility("default")))
#else
#	define PHP_EASYLOG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define EASYLOG_G(v) TSRMG(easylog_globals_id, zend_easylog_globals *, v)
#else
#define EASYLOG_G(v) (easylog_globals.v)
#endif

ZEND_BEGIN_MODULE_GLOBALS(easylog)
	char *config_file;
ZEND_END_MODULE_GLOBALS(easylog)

#if defined(ZTS) && defined(COMPILE_DL_EASYLOG)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_EASYLOG_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
