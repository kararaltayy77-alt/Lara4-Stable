/*
 * Lara4 - Dynamic TrustCache Engine
 * Bypasses code signing by injecting CDHashes into kernel trust cache
 */

#ifndef TRUSTCACHE_H
#define TRUSTCACHE_H

#include <stdint.h>
#include <stddef.h>

#define CDHASH_LEN 20
#define TRUSTCACHE_MAX_ENTRIES 256

struct trustcache_entry {
    uint8_t cdhash[CDHASH_LEN];
    uint8_t hash_type;
    uint8_t flags;
} __attribute__((packed));

struct trustcache_page {
    uint32_t version;
    uint32_t num_entries;
    struct trustcache_entry entries[TRUSTCACHE_MAX_ENTRIES];
} __attribute__((packed));

int trustcache_init(void);
int trustcache_add_cdhash(const uint8_t *cdhash, uint8_t hash_type);
int trustcache_add_file(const char *path);
int trustcache_inject(void);
void trustcache_dump(void);

#endif /* TRUSTCACHE_H */
