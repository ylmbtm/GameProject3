/*
   Copyright 漏 2012, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

#ifndef MYSQL_PLUGIN_VALIDATE_PASSWORD_INCLUDED
#define MYSQL_PLUGIN_VALIDATE_PASSWORD_INCLUDED

/* API for validate_password plugin. (MYSQL_VALIDATE_PASSWORD_PLUGIN) */

#include <mysql/plugin.h>
#define MYSQL_VALIDATE_PASSWORD_INTERFACE_VERSION 0x0100

/*  
  The descriptor structure for the plugin, that is referred from
  st_mysql_plugin.
*/

typedef void* mysql_string_handle;

struct st_mysql_validate_password
{
  int interface_version;
  /*  
    This function retuns TRUE for passwords which satisfy the password
    policy (as choosen by plugin variable) and FALSE for all other
    password
  */
  int (*validate_password)(mysql_string_handle password);
  /*  
    This function returns the password strength (0-100) depending
    upon the policies
  */
  int (*get_password_strength)(mysql_string_handle password);
};
#endif
