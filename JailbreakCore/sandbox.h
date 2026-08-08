/*
 * Lara4 - Sandbox Escape Engine
 */

#ifndef SANDBOX_H
#define SANDBOX_H

#include <stdint.h>

int sandbox_escape(void);
int sandbox_patch_container(void);
int sandbox_grant_entitlements(const char **ents, int count);

#endif /* SANDBOX_H */
