/*
 * Lara4 - Root Escalation Engine
 * Escalates from sandboxed app to UID 0 + unsandboxed
 */

#ifndef LARA4_ESCALATE_H
#define LARA4_ESCALATE_H

#include <stdint.h>
#include <stdbool.h>

// Process offsets (iOS 15-16, arm64e)
// These need to be updated per iOS version
struct proc_offsets {
    uint32_t off_p_list_le_prev;
    uint32_t off_p_pid;
    uint32_t off_p_ucred;
    uint32_t off_p_csflags;
    uint32_t off_p_sandbox;
    uint32_t off_p_textvp;
    uint32_t off_p_name;
};

// ucred offsets
struct ucred_offsets {
    uint32_t off_ucred_uid;
    uint32_t off_ucred_ruid;
    uint32_t off_ucred_svuid;
    uint32_t off_ucred_gid;
    uint32_t off_ucred_rgid;
    uint32_t off_ucred_svgid;
    uint32_t off_ucred_cr_label;
};

// Label offsets
struct label_offsets {
    uint32_t off_amfi_slot;
    uint32_t off_sandbox_slot;
};

// CS flags
#define CS_VALID        0x0000001
#define CS_ADHOC        0x0000002
#define CS_GET_TASK_ALLOW 0x0000004
#define CS_INSTALLER    0x0000008
#define CS_HARD         0x0000100
#define CS_KILL         0x0000200
#define CS_RESTRICT     0x0000800
#define CS_ENFORCEMENT  0x0001000
#define CS_REQUIRE_LV   0x0002000
#define CS_RUNTIME      0x0008000
#define CS_PLATFORM_BINARY 0x0400000
#define CS_DEBUGGED     0x10000000

int lara4_escalate_init(void);
int lara4_get_root(void);
int lara4_unsandbox(void);
int lara4_set_platform_binary(void);
int lara4_patch_csflags(void);
bool lara4_is_root(void);
bool lara4_is_sandboxed(void);

// Offset management
void lara4_set_offsets(struct proc_offsets *poff, struct ucred_offsets *uoff, struct label_offsets *loff);

#endif /* LARA4_ESCALATE_H */
