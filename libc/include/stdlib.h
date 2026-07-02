#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Terminate the process abnormally.
 * This function never returns.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Noreturn void abort(void);
#elif defined(__GNUC__)
void abort(void) __attribute__((__noreturn__));
#else
void abort(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _STDLIB_H */
