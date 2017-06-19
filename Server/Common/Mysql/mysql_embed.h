#ifndef MYSQL_EMBED_INCLUDED
#define MYSQL_EMBED_INCLUDED

/*
   Copyright (c) 2000, 2011, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

/* Defines that are unique to the embedded version of MySQL */

#ifdef EMBEDDED_LIBRARY

/* Things we don't need in the embedded version of MySQL */
/* TODO HF add #undef HAVE_VIO if we don't want client in embedded library */

#undef HAVE_DLOPEN				/* No udf functions */
#undef HAVE_SMEM				/* No shared memory */

#endif /* EMBEDDED_LIBRARY */
#endif /* MYSQL_EMBED_INCLUDED */
