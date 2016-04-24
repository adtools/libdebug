
#include <exec/types.h>
#include <exec/execbase.h>
#include <proto/exec.h>

static struct ExecBase* SysBase = NULL;


/**** OS/C/ASM WRAPPERS ******************************************************/

struct CallPutChData {
    VOID (*PutChProc)(LONG c, APTR PutChDataPtr);
    APTR PutChDataPtr;
};

#if defined(__MORPHOS__)

#include <emul/emulinterface.h>

VOID CallPutChProcMOS(VOID) {
  struct CallPutChData* callputchdata = (struct CallPutChData*) REG_A3;

  return callputchdata->PutChProc(REG_D0, callputchdata->PutChDataPtr);
}

struct EmulLibEntry CallPutChProc68k = {
  TRAP_LIB, 0, CallPutChProcMOS
};

#elif defined(__amithlon__)

VOID CallPutChProc86(LONG c, struct CallPutChData* callputchdata) __attribute__((regparm(3)));

VOID CallPutChProc86(LONG c, struct CallPutChData* callputchdata) {
  return callputchdata->PutChProc(c, callputchdata->PutChDataPtr);
}

struct CallPutChProc68k {
    UWORD movel_a3_d1;
    UWORD jmp;
    APTR  addr;
} CallPutChProc68k = {
  0x220B,
  0x4EF9, ((char*) CallPutChProc86) + 1
};

#elif defined(__mc68000__)

// This function sets up d0/a3 with the classical values but also
// pushes Char and PutChData on the stack for C programs.

static const UWORD CallPutChProc68k[] = {
  0x2F0B,                 // MOVE.L      A3,-(A7)
  0x205B,                 // MOVEA.L     (A3)+,A0
  0x2653,                 // MOVEA.L     (A3),A3
  0x2F0B,                 // MOVE.L      A3,-(A7)
  0x2F00,                 // MOVE.L      D0,-(A7)
  0x4E90,                 // JSR         (A0)
  0x508F,                 // ADDQ.L      #8,A7
  0x205F,                 // MOVEA.L     (A7)+,A0
  0x214B, 0x0004,         // MOVE.L      A3,($0004,A0)
  0x2648,                 // MOVEA.L     A0,A3
  0x4E75,                 // RTS
};

#else
# error Unknown OS/CPU
#endif


#if defined(USE_SERIAL)

/**** SERIAL INPUT/OUTPUT ****************************************************/

#include <clib/debug_protos.h>

#if defined(__MORPHOS__)
#undef RawIOInit
#undef RawPutChar
#undef RawMayGetChar
#endif

#include "exec_dbg_inline.h"

#define f(name) k ## name
#define F(name) K ## name

VOID KPutChar(LONG c) {
  // Avoid all but one page faults by caching SysBase
  if (SysBase == NULL) {
    SysBase = *((struct ExecBase**) 4);
  }

  RawPutChar(c);
}

LONG KMayGetChar(VOID) {
  // Avoid all but one page faults by caching SysBase
  if (SysBase == NULL) {
    SysBase = *((struct ExecBase**) 4);
  }

  return RawMayGetChar();
}

VOID KDoFmt(CONST_STRPTR formatString, CONST APTR dataStream,
	    CONST APTR putChProc, APTR putChData) {
  struct CallPutChData callputchdata = {
    putChProc, putChData
  };
  
  // Avoid all but one page faults by caching SysBase
  if (SysBase == NULL) {
    SysBase = *((struct ExecBase**) 4);
  }

  RawDoFmt(formatString, dataStream,
	   (void (*)(void)) &CallPutChProc68k, &callputchdata);
}


#else

/**** PARALLEL INPUT/OUTPUT **************************************************/

#include <clib/ddebug_protos.h>

#define f(name) d ## name
#define F(name) D ## name

LONG PRawMayGetChar(VOID) {
  return -1;
}

#endif


/**** COMMON FUNCTIONS *******************************************************/

VOID F(PutStr)(CONST_STRPTR string) {
  UBYTE c;

  while((c = *string++) != 0) {
    F(PutChar)(c);
  }
}

LONG KGetChar(VOID) {
  LONG c;

  while((c = F(MayGetChar)()) < 0);
  return c;
}

VOID F(PutFmt)(CONST_STRPTR formatString, CONST APTR values) {
  KDoFmt(formatString, values, (void (*)(void)) &KPutChar, NULL);
}

#ifndef __MORPHOS__

VOID F(PrintF)(CONST_STRPTR formatString, ...) {
  KDoFmt(formatString, &formatString+1, (void (*)(void)) &KPutChar, NULL);
}

#else

#include <stdarg.h>

static APTR F(PutChPPC)(APTR data, UBYTE c) {
  F(PutChar)(c);

  return data;
}

VOID F(PrintF)(CONST_STRPTR formatString, ...) {
  va_list args;
  va_start (args, formatString);

  VNewRawDoFmt(formatString, F(PutChPPC), NULL, args);
}
#endif

LONG F(CmpStr)(CONST_STRPTR a, CONST_STRPTR b) {
  LONG cnt = 1;
  while(TRUE) {
    UBYTE c = *a++;

    if (c == 0) {
      if (*b++ == 0) {
	return 0;
      }
      else {
	return -cnt;
      }
    }

    if (c != *b++) {
      return cnt;
    }

    ++cnt;
  }
}

LONG F(GetNum)(VOID) {
  UBYTE c;
  LONG  num;
  BOOL  neg;
  ULONG cnt;

  while (TRUE) {
    num = 0;
    neg = FALSE;
    cnt = 0;

    c = KGetChar();

    if (c == '-') {
      neg = TRUE;
      ++cnt;
      KPutChar(c);
    }

    while (TRUE) {
      if (c == '\b') {
	if (cnt > 0) {
	  KPutChar('\b');
	  KPutChar(' ');
	  KPutChar('\b');
	  --cnt;
	}

	if (cnt == 0) {
	  break;
	}
	else {
	  num /= 10;
	}
      }
      else if( c >= '0' && c <= '9') {
	num = num * 10 + (c - '0');

	KPutChar(c);
	++cnt;
      }
      else if (c == '\r') {
	return neg ? -num : num;
      }

      c = KGetChar();
    }
  }
}


/**** ALIASES ****************************************************************/

#if defined(__ELF__)
# define ALIAS2(a,b) __asm(".set " #a "," #b "; .globl " #a  )
#else
# define ALIAS2(a,b) __asm(".stabs \"_" #a "\",11,0,0,0;.stabs \"_" #b "\",1,0,0,0")
#endif

#define ALIAS(new,old) ALIAS2(new,old)

ALIAS(f(putchar),F(PutChar));
ALIAS(f(putc),F(PutChar));
ALIAS(F(PutCh),F(PutChar));
ALIAS(f(putch),F(PutChar));

ALIAS(f(putstr),F(PutStr));
ALIAS(F(PutS),F(PutStr));
ALIAS(f(puts),F(PutStr));

ALIAS(F(MayGetCh),F(MayGetChar));
ALIAS(f(getc),F(GetChar));
ALIAS(f(getchar),F(GetChar));
ALIAS(f(getch),F(GetChar));
ALIAS(F(GetCh),F(GetChar));

ALIAS(F(VPrintF),F(PutFmt));
ALIAS(f(printf),F(PrintF));

ALIAS(cmpstrexec,F(CmpStr));
ALIAS(f(kgetnum),F(GetNum));
