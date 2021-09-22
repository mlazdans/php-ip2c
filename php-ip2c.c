#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ip2c.h"
#include "ip2c.h"

//
// Building PHP extensions
// http://www.phpinternalsbook.com/php7/build_system/building_extensions.html
//
// https://github.com/Microsoft/php-sdk-binary-tools
// https://ourcodeworld.com/articles/read/804/how-to-compile-a-php-extension-dll-file-in-windows-with-visual-studio
// https://wiki.php.net/internals/windows/stepbystepbuild_sdk_2
//
//
// Upgrading PHP extensions from PHP5 to NG
// https://wiki.php.net/phpng-upgrading
//
// Porting Extension to PHP7
// https://github.com/beberlei/beberlei.de/blob/master/drafts/porting_extension_to_php7.rst
//

ZEND_DECLARE_MODULE_GLOBALS(ip2c)
static PHP_GINIT_FUNCTION(ip2c)
{
#if defined(COMPILE_DL_IP2C) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	ip2c_globals->ipdb = NULL;

	snprintf(ip2c_globals->db_version, sizeof(ip2c_globals->db_version), "-not loaded-");
	snprintf(ip2c_globals->db_ip_count, sizeof(ip2c_globals->db_ip_count), "-not loaded-");
	snprintf(ip2c_globals->db_rec_count, sizeof(ip2c_globals->db_rec_count), "-not loaded-");
	snprintf(ip2c_globals->lib_version, sizeof(ip2c_globals->lib_version), "%d.%d", IP2C_DB_VERS_HI, IP2C_DB_VERS_LO);
}

//ZEND_BEGIN_ARG_INFO_EX(name, _unused, return_reference, required_num_args)
ZEND_BEGIN_ARG_INFO_EX(arginfo_ip2c_getcountry, 0, 0, 1)
	ZEND_ARG_INFO(0, ip_list)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ip2c_db_get_version, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ip2c_db_get_rec_count, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ip2c_db_get_ip_count, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ip2c_db_load_file, 0, 0, 1)
	ZEND_ARG_INFO(0, file_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ip2c_lib_get_version, 0)
ZEND_END_ARG_INFO()

PHP_INI_BEGIN()
	//PHP_INI_ENTRY("ip2c.database", IP2C_DB_PATH, PHP_INI_ALL, NULL)
	PHP_INI_ENTRY("ip2c.database", "", PHP_INI_ALL, NULL)
PHP_INI_END()

zend_function_entry ip2c_functions[] = {
	PHP_FE(ip2c_getcountry,       arginfo_ip2c_getcountry)
	PHP_FE(ip2c_db_get_version,   arginfo_ip2c_db_get_version)
	PHP_FE(ip2c_db_get_rec_count, arginfo_ip2c_db_get_rec_count)
	PHP_FE(ip2c_db_get_ip_count,  arginfo_ip2c_db_get_ip_count)
	PHP_FE(ip2c_db_load_file,     arginfo_ip2c_db_load_file)
	PHP_FE(ip2c_lib_get_version,  arginfo_ip2c_lib_get_version)
	PHP_FE_END
};

zend_module_entry ip2c_module_entry = {
	STANDARD_MODULE_HEADER,
	"ip2c",
	ip2c_functions,
	PHP_MINIT(ip2c),
	PHP_MSHUTDOWN(ip2c),
	NULL, //PHP_RINIT(ip2c),
	NULL, //PHP_RSHUTDOWN(ip2c),
	PHP_MINFO(ip2c),
	"0.4", /* Replace with version number for your extension */
	PHP_MODULE_GLOBALS(ip2c),
	PHP_GINIT(ip2c),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_IP2C
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ip2c)
#endif

//#ifdef COMPILE_DL_IP2C
//ZEND_GET_MODULE(ip2c)
//#endif

int php_ip2c_db_load_file(const char *file_name) {
	if (IP2CG(ipdb)) {
		ip2c_db_free(IP2CG(ipdb));
		IP2CG(ipdb) = NULL;
	}

	if (IP2CG(ipdb) == NULL) {
		//startTime = (float)clock() / CLOCKS_PER_SEC;
		IP2CG(ipdb) = (IPDB*)ip2c_db_load_file(file_name);
		if (IP2CG(ipdb)) {
			snprintf(IP2CG(db_version), sizeof(IP2CG(db_version)), "%d.%d", IP2CG(ipdb)->vers_hi, IP2CG(ipdb)->vers_lo);
			snprintf(IP2CG(db_ip_count), sizeof(IP2CG(db_ip_count)), "%u", IP2CG(ipdb)->ip_count);
			snprintf(IP2CG(db_rec_count), sizeof(IP2CG(db_rec_count)), "%u", IP2CG(ipdb)->rec_count);
		} else {
			return 0;
		}
	}

	return 1;
}

PHP_MINIT_FUNCTION(ip2c)
{
#if defined(ZTS) && defined(COMPILE_DL_IP2C)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	//ZEND_INIT_MODULE_GLOBALS(ip2c, php_ip2c_init_globals, NULL);
	REGISTER_INI_ENTRIES();

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(ip2c)
{
#if defined(COMPILE_DL_IP2C) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	UNREGISTER_INI_ENTRIES();

	ip2c_db_free(IP2CG(ipdb));

	return SUCCESS;
}

PHP_MINFO_FUNCTION(ip2c)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "ip2c support",     "enabled");
	php_info_print_table_row(2, "libip2c version",  IP2CG(lib_version));
	php_info_print_table_row(2, "Database version", IP2CG(db_version));
	php_info_print_table_row(2, "Record count",     IP2CG(db_rec_count));
	php_info_print_table_row(2, "IP count",         IP2CG(db_ip_count));
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

PHP_FUNCTION(ip2c_getcountry)
{
	zval *ip_list, *element;
	HashTable *ht;
	ip2c_ip *ip_array;
	ip2c_iso *iso_codes;
	unsigned long ip_count = 0, r, count, ip_long;

	if (!IP2CG(ipdb)) {
		if (strlen(INI_STR("ip2c.database"))) {
			if (!php_ip2c_db_load_file(INI_STR("ip2c.database"))) {
				php_error_docref(NULL, E_WARNING, "Can't load ip2c database: %s", INI_STR("ip2c.database"));
				return;
			}
		} else {
			php_error_docref(NULL, E_WARNING, "ip2c database not loaded");
			return;
		}
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(ip_list)
	ZEND_PARSE_PARAMETERS_END();

	ht = Z_ARRVAL_P(ip_list);

	// zend_hash_num_elements() � returns number of elements in HashTable.
	// zend_array_count(zend_array * arr) � returns number of elements in array.
	count = zend_hash_num_elements(ht);

	ip_array = ecalloc(count, sizeof(ip2c_ip));

	ZEND_HASH_FOREACH_VAL(ht, element) {
		switch (Z_TYPE_P(element)) {
			case IS_STRING:
				ip_long = ip2c_ip2long(Z_STRVAL_P(element));
				break;
			case IS_LONG:
				ip_long = (unsigned long)Z_LVAL_P(element);
				break;
			case IS_DOUBLE:
				ip_long = (unsigned long)Z_DVAL_P(element);
				break;
			case IS_ARRAY:
			case IS_OBJECT:
			default:
				ip_long = 0;
				break;
				//php_error_docref(NULL, E_NOTICE, "Unknown datatype: %s", zend_zval_type_name(element));
				//ZVAL_DUP(&val7n, val7);
				//convert_scalar_to_number(&val7n TSRMLS_CC);
				//convert_to_long(&val7n);
				//iplong = zval_get_long(&val7n);
		}

		ip_array[ip_count++] = ip_long;
	} ZEND_HASH_FOREACH_END();

	iso_codes = ecalloc(count, sizeof(ip2c_iso));

	//startTime = (float)clock()/CLOCKS_PER_SEC;
	ip2c_getcountry(IP2CG(ipdb), ip_array, ip_count, iso_codes);

	array_init_size(return_value, ip_count);
	for(r = 0; r < ip_count; r++)
		if(strlen(iso_codes[r]))
			add_next_index_string(return_value, iso_codes[r]);
		else
			add_next_index_bool(return_value, 0);

	efree(ip_array);
	efree(iso_codes);
}

PHP_FUNCTION(ip2c_db_get_version)
{
	RETURN_STRING(IP2CG(db_version));
}

PHP_FUNCTION(ip2c_db_get_rec_count)
{
	RETURN_STRING(IP2CG(db_rec_count));
}

PHP_FUNCTION(ip2c_db_get_ip_count)
{
	RETURN_STRING(IP2CG(db_ip_count));
}

PHP_FUNCTION(ip2c_db_load_file)
{
	zend_string *file_name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(file_name)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_BOOL(php_ip2c_db_load_file(file_name->val));
}

PHP_FUNCTION(ip2c_lib_get_version)
{
	RETURN_STRING(IP2CG(lib_version));
}
