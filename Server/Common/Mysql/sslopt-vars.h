#ifndef SSLOPT_VARS_INCLUDED
#define SSLOPT_VARS_INCLUDED

/*
   Copyright (c) 2000, 2011, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

#if defined(HAVE_OPENSSL) && !defined(EMBEDDED_LIBRARY)
#ifdef SSL_VARS_NOT_STATIC
#define SSL_STATIC
#else
#define SSL_STATIC static
#endif
SSL_STATIC my_bool opt_use_ssl   = 0;
SSL_STATIC char *opt_ssl_ca      = 0;
SSL_STATIC char *opt_ssl_capath  = 0;
SSL_STATIC char *opt_ssl_cert    = 0;
SSL_STATIC char *opt_ssl_cipher  = 0;
SSL_STATIC char *opt_ssl_key     = 0;
SSL_STATIC char *opt_ssl_crl     = 0;
SSL_STATIC char *opt_ssl_crlpath = 0;
#ifdef MYSQL_CLIENT
SSL_STATIC my_bool opt_ssl_verify_server_cert= 0;
#endif
#endif
#endif /* SSLOPT_VARS_INCLUDED */
