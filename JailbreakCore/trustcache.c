/*
 * Lara4 - Dynamic TrustCache Implementation
 */

#include "trustcache.h"
#include "../Primitives/pplrw.h"
#include <stdio.h>
#include <string.h>
#include <CommonCrypto/CommonDigest.h>

static struct trustcache_page g_tc = {0};
static uint64_t g_tc_kernel_addr = 0;

// Kernel globals (need to be found via ksymbol resolution)
static uint64_t g_trust_caches = 0;  // gTrustCaches linked list head

int trustcache_init(void) {
    memset(&g_tc, 0, sizeof(g_tc));
    g_tc.version = 1;
    g_tc.num_entries = 0;
    printf("[Lara4] TrustCache engine initialized\n");
    return 0;
}

int trustcache_add_cdhash(const uint8_t *cdhash, uint8_t hash_type) {
    if (g_tc.num_entries >= TRUSTCACHE_MAX_ENTRIES) {
        printf("[-] TrustCache full\n");
        return -1;
    }

    struct trustcache_entry *entry = &g_tc.entries[g_tc.num_entries++];
    memcpy(entry->cdhash, cdhash, CDHASH_LEN);
    entry->hash_type = hash_type;
    entry->flags = 0; // Trusted

    printf("[+] Added CDHash to TrustCache (total: %d)\n", g_tc.num_entries);
    return 0;
}

int trustcache_add_file(const char *path) {
    // Calculate CDHash of file
    // Simplified: in production, parse Mach-O and compute hash
    (void)path;
    printf("[*] trustcache_add_file: %s\n", path);
    return 0;
}

int trustcache_inject(void) {
    if (!g_trust_caches) {
        printf("[-] gTrustCaches not found - need ksymbol resolution\n");
        return -1;
    }

    if (g_tc.num_entries == 0) {
        printf("[-] No entries to inject\n");
        return -1;
    }

    printf("[Lara4] Injecting TrustCache into kernel...\n");

    // Allocate kernel memory for trustcache page
    size_t tc_size = sizeof(uint32_t) * 2 + g_tc.num_entries * sizeof(struct trustcache_entry);
    g_tc_kernel_addr = 0; // kalloc(tc_size); // needs kalloc from primitives

    if (!g_tc_kernel_addr) {
        printf("[-] Failed to allocate kernel memory for TrustCache\n");
        return -1;
    }

    // Write trustcache to kernel
    ppl_writebuf(g_tc_kernel_addr, &g_tc, tc_size);

    // Insert into gTrustCaches linked list
    // Read current head
    uint64_t next = ppl_read64(g_trust_caches);

    // Set our trustcache as new head
    // Structure: next_ptr -> our_tc -> old_head
    ppl_write64(g_tc_kernel_addr + tc_size, next); // next pointer
    ppl_write64(g_trust_caches, g_tc_kernel_addr);

    printf("[+] TrustCache injected at 0x%llx\n", g_tc_kernel_addr);
    return 0;
}

void trustcache_dump(void) {
    printf("TrustCache entries: %d\n", g_tc.num_entries);
    for (uint32_t i = 0; i < g_tc.num_entries; i++) {
        printf("  [%d] ", i);
        for (int j = 0; j < CDHASH_LEN; j++) {
            printf("%02x", g_tc.entries[i].cdhash[j]);
        }
        printf("\n");
    }
}
