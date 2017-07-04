dnl $Id$
dnl config.m4 for extension easylog

PHP_ARG_ENABLE(easylog, whether to enable easylog support,
[  --enable-easylog           Enable easylog support])

if test "$PHP_EASYLOG" != "no"; then
    CXXFLAGS="-std=c++11 -w ${CXXFLAGS}"
    PHP_REQUIRE_CXX()
    PHP_SUBST(EASYLOG_SHARED_LIBADD)
    PHP_ADD_INCLUDE(easyloggingpp)
    PHP_ADD_LIBRARY(stdc++, 1, EASYLOG_SHARED_LIBADD)
    PHP_NEW_EXTENSION(easylog, easyloggingpp/easylogging++.cc easylog.cc, $ext_shared)
fi
