/*
   Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

#ifndef INNODB_PRIV_INCLUDED
#define INNODB_PRIV_INCLUDED

/** @file Declaring server-internal functions that are used by InnoDB. */

#include <sql_priv.h>

class THD;

int get_quote_char_for_identifier(THD *thd, const char *name, uint length);
bool schema_table_store_record(THD *thd, TABLE *table);
void localtime_to_TIME(MYSQL_TIME *to, struct tm *from);
bool check_global_access(THD *thd, ulong want_access);
uint strconvert(CHARSET_INFO *from_cs, const char *from,
                CHARSET_INFO *to_cs, char *to, uint to_length,
                uint *errors);
void sql_print_error(const char *format, ...);



#endif /* INNODB_PRIV_INCLUDED */
