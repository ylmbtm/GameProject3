/*
   Copyright (c) 2000, 2011, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

/*
** Ask for a password from tty
** This is an own file to avoid conflicts with curses
*/

#ifndef MYSQL_GET_PASSWORD_H_INCLUDED
#define MYSQL_GET_PASSWORD_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef char *(* strdup_handler_t)(const char *, int);
char *get_tty_password_ext(const char *opt_message,
                           strdup_handler_t strdup_function);

#ifdef __cplusplus
}
#endif

#endif /* ! MYSQL_GET_PASSWORD_H_INCLUDED */
