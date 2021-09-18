dnl $Id$
dnl config.m4 for extension ip2c

PHP_ARG_WITH(ip2c, for ip2c support, [  --with-ip2c[=DIR]            Include ip2c support])

if test "$PHP_IP2C" != "no"; then

	if test -r $PHP_IP2C/ip2c.h; then
		IP2C_DIR=$PHP_IP2C
	else
		AC_MSG_CHECKING(for ip2c in default path)

		for i in /usr/local /usr ; do
			if test -r $i/include/ip2c.h; then
				IP2C_DIR=$i
				AC_MSG_RESULT(found in $i)
				break
			fi
		done
	fi

	if test -z "$IP2C_DIR"; then
		AC_MSG_RESULT([ip2c library not found])
		AC_MSG_ERROR([Please reinstall the ip2c distribution])
	fi

	PHP_ADD_INCLUDE($IP2C_DIR)
	PHP_ADD_LIBRARY_WITH_PATH(ip2c, $IP2C_DIR, IP2C_SHARED_LIBADD)
	PHP_NEW_EXTENSION(ip2c, ip2c.c, $ext_shared)
	PHP_SUBST(IP2C_SHARED_LIBADD)
fi
