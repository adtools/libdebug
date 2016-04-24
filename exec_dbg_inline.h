/* Automatically generated header (sfdc 1.4)! Do not edit! */

#ifndef _INLINE_EXEC_DBG_H
#define _INLINE_EXEC_DBG_H

#ifndef _SFDC_VARARG_DEFINED
#define _SFDC_VARARG_DEFINED
#ifdef __HAVE_IPTR_ATTR__
typedef APTR _sfdc_vararg __attribute__((iptr));
#else
typedef ULONG _sfdc_vararg;
#endif /* __HAVE_IPTR_ATTR__ */
#endif /* _SFDC_VARARG_DEFINED */

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif /* !__INLINE_MACROS_H */

#ifndef EXEC_DBG_BASE_NAME
#define EXEC_DBG_BASE_NAME SysBase
#endif /* !EXEC_DBG_BASE_NAME */

#define RawIOInit() \
	LP0NR(0x1f8, RawIOInit ,\
	, EXEC_DBG_BASE_NAME)

#define RawMayGetChar() \
	LP0(0x1fe, BYTE, RawMayGetChar ,\
	, EXEC_DBG_BASE_NAME)

#define RawPutChar(___c) \
	LP1NR(0x204, RawPutChar , UBYTE, ___c, d0,\
	, EXEC_DBG_BASE_NAME)

#endif /* !_INLINE_EXEC_DBG_H */
