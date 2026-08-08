/*
 * Lara4 - PPL Read/Write Implementation
 */

#include "pplrw.h"
#include "../Exploits/PPL/titan_adapter.h"
#include <stdio.h>
#include <string.h>

int pplrw_init(void) {
    if (lara4_titan_ppl_active()) {
        printf("[-] PPL still active - run Titan bypass first\n");
        return -1;
    }
    printf("[+] PPL R/W subsystem initialized\n");
    return 0;
}

uint64_t ppl_read64(uint64_t addr) {
    return lara4_titan_ppl_read64(addr);
}

uint32_t ppl_read32(uint64_t addr) {
    uint64_t val = ppl_read64(addr);
    return (uint32_t)val;
}

void ppl_write64(uint64_t addr, uint64_t val) {
    lara4_titan_ppl_write64(addr, val);
}

void ppl_write32(uint64_t addr, uint32_t val) {
    lara4_titan_ppl_write32(addr, val);
}

void ppl_readbuf(uint64_t addr, void *buf, size_t len) {
    uint8_t *b = (uint8_t*)buf;
    for (size_t i = 0; i < len; i++) {
        b[i] = (uint8_t)ppl_read32(addr + i);
    }
}

void ppl_writebuf(uint64_t addr, const void *buf, size_t len) {
    const uint8_t *b = (const uint8_t*)buf;
    for (size_t i = 0; i < len; i++) {
        ppl_write32(addr + i, b[i]);
    }
}
