/*
 * Lara4 - Dopamine 3.0 Jailbreak Orchestrator
 * Main entry point for full jailbreak chain
 * Version: 6.0-Dopamine3
 */

#ifndef LARA4_JAILBREAK_H
#define LARA4_JAILBREAK_H

#include <stdint.h>
#include <stdbool.h>

// Jailbreak stages
typedef enum {
    JB_STAGE_IDLE = 0,
    JB_STAGE_KERNEL_EXPLOIT,
    JB_STAGE_PPL_BYPASS,
    JB_STAGE_PAC_BYPASS,
    JB_STAGE_ROOT_ESCALATION,
    JB_STAGE_CODESIGN_BYPASS,
    JB_STAGE_BOOTSTRAP,
    JB_STAGE_COMPLETE,
    JB_STAGE_FAILED
} jb_stage_t;

// Jailbreak configuration
typedef struct {
    bool verbose;
    bool auto_reboot_userspace;
    bool install_sileo;
    bool install_zebra;
    bool enable_tweak_injection;
    bool custom_boot_logo;
    char boot_logo_path[256];
} jb_config_t;

// Main API
int lara4_jailbreak_init(jb_config_t *config);
int lara4_jailbreak_run(void);
jb_stage_t lara4_jailbreak_get_stage(void);
const char* lara4_jailbreak_stage_name(jb_stage_t stage);
bool lara4_jailbreak_is_complete(void);
void lara4_jailbreak_cleanup(void);

// Individual stage runners (for debugging)
int lara4_stage_kernel_exploit(void);
int lara4_stage_ppl_bypass(void);
int lara4_stage_pac_bypass(void);
int lara4_stage_root_escalation(void);
int lara4_stage_codesign_bypass(void);
int lara4_stage_bootstrap(void);

#endif /* LARA4_JAILBREAK_H */
