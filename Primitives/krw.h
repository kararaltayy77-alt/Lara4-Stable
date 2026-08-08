/*
 * Lara4 - Kernel Read/Write Primitives
 * Unified interface for kernel memory access
 */

#ifndef KRW_H
#define KRW_H

#include <stdint.h>
#include <stddef.h>

// Initialize KRW subsystem
int krw_init(void);

// Basic kernel read/write
uint64_t kread64(uint64_t addr);
uint32_t kread32(uint64_t addr);
uint16_t kread16(uint64_t addr);
uint8_t  kread8(uint64_t addr);

void kwrite64(uint64_t addr, uint64_t val);
void kwrite32(uint64_t addr, uint32_t val);
void kwrite16(uint64_t addr, uint16_t val);
void kwrite8(uint64_t addr, uint8_t val);

// Bulk reads
void kreadbuf(uint64_t addr, void *buf, size_t len);
void kwritebuf(uint64_t addr, const void *buf, size_t len);

// Kernel allocation
uint64_t kalloc(size_t size);
void kfree(uint64_t addr, size_t size);
uint64_t kalloc_permanent(size_t size, uint64_t tag);

// Physical memory mapping
uint64_t kmap_physical(uint64_t paddr, size_t size);
void kunmap_physical(uint64_t vaddr, size_t size);

// Utility
uint64_t kslide(void);  // Get kernel slide
uint64_t kernbase(void); // Get kernel base

#endif /* KRW_H */
