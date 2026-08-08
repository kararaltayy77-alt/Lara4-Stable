/*
 * Lara4 - PPL Read/Write Primitives
 * For accessing PPL-protected memory after Titan bypass
 */

#ifndef PPLRW_H
#define PPLRW_H

#include <stdint.h>

int pplrw_init(void);

uint64_t ppl_read64(uint64_t addr);
uint32_t ppl_read32(uint64_t addr);
void ppl_write64(uint64_t addr, uint64_t val);
void ppl_write32(uint64_t addr, uint32_t val);

void ppl_readbuf(uint64_t addr, void *buf, size_t len);
void ppl_writebuf(uint64_t addr, const void *buf, size_t len);

#endif /* PPLRW_H */
