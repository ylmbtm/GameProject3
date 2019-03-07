#ifndef __CHECK_MACRO_DEFINE_H__
#define __CHECK_MACRO_DEFINE_H__

#include <assert.h>

#define ASSERT_FAIELD (assert(false))
#undef ASSERT
#define ASSERT(x)     (assert(x))


////////////////////////////////////////////


#endif //__CHECK_MACRO_DEFINE_H__
