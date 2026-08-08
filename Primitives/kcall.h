/*
 * Lara4 - Kernel Function Calling Primitives
 */

#ifndef KCALL_H
#define KCALL_H

#include <stdint.h>

// Initialize kernel function calling
int kcall_init(void);

// Call kernel function with up to 8 arguments
uint64_t kcall(uint64_t func, uint64_t *args, int nargs);

// Convenience wrappers
uint64_t kcall0(uint64_t func);
uint64_t kcall1(uint64_t func, uint64_t a1);
uint64_t kcall2(uint64_t func, uint64_t a1, uint64_t a2);
uint64_t kcall3(uint64_t func, uint64_t a1, uint64_t a2, uint64_t a3);
uint64_t kcall4(uint64_t func, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4);

// Find kernel symbol
uint64_t ksym(const char *name);

#endif /* KCALL_H */
