
#ifdef __SASC
# define CONST_STRPTR const STRPTR
# define CONST const
#endif

#include <clib/debug_protos.h>

static const char KPutStr_str[]  = " 1: Testing KPutStr\n";
static const char kputs_str[]    = " 2: Testing kputs\n";
static const char KPutS_str[]    = " 3: Testing KPutS\n";
static const char KPutChar_str[] = " 4: Testing KPutChar\n";
static const char kputc_str[]    = " 5: Testing kputc\n";
static const char kputchar_str[] = " 6: Testing kputchar\n";
static const char kputch_str[]   = " 7: Testing kputch\n";
static const char KPutCh_str[]   = " 8: Testing KPutCh\n";
static const char KPutFmt_str[]  = " 9: Testing KPutFmt: %ld-1 is 0x%lx\n";
static const char KVPrintF_str[] = "10: Testing KVPrintF: %ld-1 is 0x%lx\n";
static const char KPrintF_str[]  = "11: Testing KPrintF & KCmpStr: %ld %ld %ld\n";
static const char kprintf_str[]  = "12: Testing kprintf & cmpstrexec: %ld %ld %ld\n";
static const char KDoFmt_str[]   = "13: Testing KDoFmt+KPutChar: %ld-1 is 0x%lx\n";
static const char KDoFmt2_str[]  = "14: Testing KDoFmt+custom: %ld-1 is 0x%lx\n";
static const char KGetChar_str[] = "15: Testing KGetChar: ";
static const char kgetc_str[]    = "16: Testing kgetc: ";
static const char kgetchar_str[] = "17: Testing kgetchar: ";
static const char kgetch_str[]   = "18: Testing kgetch: ";
static const char KGetCh_str[]   = "19: Testing KGetCh: ";


static void puts(const char* str, void (*func)(long c)) {
  while (TRUE) {
    if (*str == 0) {
      break;
    }

    func(*str++);
  }
}

static void putChProc(LONG c, APTR data) {
  if (data == (APTR) 0x01234567) {
    KPutCh(c);
  }
  else {
    KPutCh('?');
  }
}

static void test_getc(CONST_STRPTR str, LONG (*func)(VOID)) {
  LONG c;
  
  kputs(str);
  c = func();
  kprintf("Got char %ld\n", c);
}

int main(void) {
  ULONG args[] = { 262145, 262144 };

  KPutStr(KPutStr_str);
  kputs(kputs_str);
  KPutS(KPutS_str);

  puts(KPutChar_str, KPutChar);
  puts(kputc_str,    kputc);
  puts(kputchar_str, kputchar);
  puts(kputch_str,   kputch);
  puts(KPutCh_str,   KPutCh);

  KPutFmt(KPutFmt_str, args);
  KVPrintF(KVPrintF_str, args);

  KPrintF(KPrintF_str, KCmpStr("A","B1"), KCmpStr("B1","A"), KCmpStr("A1","A1"));
  KPrintF(kprintf_str, cmpstrexec("aaa", "aa"), cmpstrexec("aa", "aaa"), cmpstrexec("aaa", "aaa"));

  KDoFmt(KDoFmt_str, args, KPutChar, NULL);
  KDoFmt(KDoFmt2_str, args, putChProc, (APTR) 0x01234567);


  kputs("\nTesting input\n");

  test_getc(KGetChar_str, KGetChar);
  test_getc(kgetc_str, kgetc);
  test_getc(kgetchar_str, kgetchar);
  test_getc(kgetch_str, kgetch);
  test_getc(KGetCh_str, KGetCh);

  return 0;
}
