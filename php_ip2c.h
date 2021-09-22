#ifndef PHP_IP2C_H
#define PHP_IP2C_H

extern zend_module_entry ip2c_module_entry;
#define phpext_ip2c_ptr &ip2c_module_entry

#include "ip2c.h"

//#ifdef PHP_WIN32
//#define PHP_IP2C_API __declspec(dllexport)
//#else
//#define PHP_IP2C_API
//#endif

//#ifdef PHP_WIN32
//#	ifdef PHP_IP2C_EXPORTS
//#		define PHP_IP2C_API __declspec(dllexport)
//#	elif defined(COMPILE_DL_IP2C)
//#		define PHP_IP2C_API __declspec(dllimport)
//#	else
//#		define PHP_IP2C_API /* nothing special */
//#	endif
//#elif defined(__GNUC__) && __GNUC__ >= 4
//#	define PHP_IP2C_API __attribute__ ((visibility("default")))
//#else
//#	define PHP_IP2C_API
//#endif


#ifdef PHP_WIN32
#define IP2C_DB_PATH "ip2c.db"
#else
#define IP2C_DB_PATH "/etc/ip2c.db"
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(ip2c);
PHP_MSHUTDOWN_FUNCTION(ip2c);
//PHP_RINIT_FUNCTION(ip2c);
//PHP_RSHUTDOWN_FUNCTION(ip2c);
PHP_MINFO_FUNCTION(ip2c);

PHP_FUNCTION(ip2c_getcountry);
PHP_FUNCTION(ip2c_db_get_version);
PHP_FUNCTION(ip2c_db_get_rec_count);
PHP_FUNCTION(ip2c_db_get_ip_count);
PHP_FUNCTION(ip2c_db_load_file);
PHP_FUNCTION(ip2c_lib_get_version);

/*
	Declare any global variables you may need between the BEGIN
	and END macros here:
*/
ZEND_BEGIN_MODULE_GLOBALS(ip2c)
	IPDB *ipdb;
	char lib_version[13];
	char db_version[13];
	char db_rec_count[13];
	char db_ip_count[13];
	//unsigned long rec_count;
	//unsigned long ip_count;
ZEND_END_MODULE_GLOBALS(ip2c)

/* In every utility function you add that needs to use variables
   in php_ip2c_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as IP2C_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

//# if defined(ZTS) && defined(COMPILE_DL_IP2C)
//ZEND_TSRMLS_CACHE_EXTERN()
//# endif
//
//#ifdef ZTS
//#define IP2C_G(v) TSRMG(ip2c_globals_id, zend_ip2c_globals *, v)
//#else
//#define IP2C_G(v) (ip2c_globals.v)
//#endif

#define IP2CG(v) ZEND_MODULE_GLOBALS_ACCESSOR(ip2c, v)

#endif	/* PHP_IP2C_H */
