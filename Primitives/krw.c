/*
 * Lara4 - Kernel Read/Write Primitives Implementation
 */

#include "krw.h"
#include "../Exploits/Kernel/kfd_adapter.h"
#include <stdio.h>
#include <string.h>

static krw_primitives_t *g_krw = NULL;
static uint64_t g_kslide = 0;
static uint64_t g_kernbase = 0;

int krw_init(void) {
    g_krw = lara4_kfd_get_primitives();
    if (!g_krw || !g_krw->initialized) {
        printf("[-] KRW: No kernel primitives available\n");
        return -1;
    }
    printf("[+] KRW subsystem initialized\n");
    return 0;
}

uint64_t kread64(uint64_t addr) {
    if (!g_krw) return 0;
    return g_krw->kread64(addr);
}

uint32_t kread32(uint64_t addr) {
    if (!g_krw) return 0;
    return g_krw->kread32(addr);
}

uint16_t kread16(uint64_t addr) {
    if (!g_krw) return 0;
    return (uint16_t)g_krw->kread32(addr);
}

uint8_t kread8(uint64_t addr) {
    if (!g_krw) return 0;
    return (uint8_t)g_krw->kread32(addr);
}

void kwrite64(uint64_t addr, uint64_t val) {
    if (!g_krw) return;
    g_krw->kwrite64(addr, val);
}

void kwrite32(uint64_t addr, uint32_t val) {
    if (!g_krw) return;
    g_krw->kwrite32(addr, val);
}

void kwrite16(uint64_t addr, uint16_t val) {
    if (!g_krw) return;
    uint32_t old = g_krw->kread32(addr);
    old = (old & ~0xFFFF) | val;
    g_krw->kwrite32(addr, old);
}

void kwrite8(uint64_t addr, uint8_t val) {
    if (!g_krw) return;
    uint32_t old = g_krw->kread32(addr);
    old = (old & ~0xFF) | val;
    g_krw->kwrite32(addr, old);
}

void kreadbuf(uint64_t addr, void *buf, size_t len) {
    uint8_t *b = (uint8_t*)buf;
    for (size_t i = 0; i < len; i++) {
        b[i] = kread8(addr + i);
    }
}

void kwritebuf(uint64_t addr, const void *buf, size_t len) {
    const uint8_t *b = (const uint8_t*)buf;
    for (size_t i = 0; i < len; i++) {
        kwrite8(addr + i, b[i]);
    }
}

uint64_t kalloc(size_t size) {
    if (!g_krw || !g_krw->kalloc) return 0;
    return g_krw->kalloc(size);
}

void kfree(uint64_t addr, size_t size) {
    if (!g_krw || !g_krw->kfree) return;
    g_krw->kfree(addr, size);
}

uint64_t kalloc_permanent(size_t size, uint64_t tag) {
    (void)tag;
    return kalloc(size);
}

uint64_t kmap_physical(uint64_t paddr, size_t size) {
    (void)size;
    if (!g_krw || !g_krw->physmap) return 0;
    return g_krw->physmap(paddr);
}

void kunmap_physical(uint64_t vaddr, size_t size) {
    (void)vaddr;
    (void)size;
    // No-op for now
}

uint64_t kslide(void) {
    return g_kslide;
}

uint64_t kernbase(void) {
    return g_kernbase;
}
