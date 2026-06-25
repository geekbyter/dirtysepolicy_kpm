/*
 * Copyright (C) 2024. All Rights Reserved.
 * SELinux Magisk Filter — Header
 *
 * Hooks security_sid_to_context() to sanitize Magisk context strings
 * returned via /sys/fs/selinux/context and /sys/fs/selinux/access.
 */

#ifndef __SELINUX_HOOK_H
#define __SELINUX_HOOK_H

#include <hook.h>
#include <ksyms.h>
#include <uapi/asm-generic/errno.h>

#define MAX_KEY_LEN 128

#define lookup_name(func)                                  \
    func = 0;                                              \
    func = (typeof(func))kallsyms_lookup_name(#func);      \
    pr_info("kernel function %s addr: %llx\n", #func, func); \
    if (!func) {                                           \
        return -21;                                        \
    }

#define hook_func(func, argv, before, after, udata)                        \
    if (!func) {                                                           \
        return -22;                                                        \
    }                                                                      \
    hook_err_t hook_err_##func = hook_wrap(func, argv, before, after, udata); \
    if (hook_err_##func) {                                                 \
        func = 0;                                                          \
        pr_err("hook %s error: %d\n", #func, hook_err_##func);             \
        return -23;                                                        \
    } else {                                                               \
        pr_info("hook %s success\n", #func);                               \
    }

#define unhook_func(func)           \
    if (func && !is_bad_address(func)) { \
        unhook(func);               \
        pr_info("unhook %s\n", #func); \
    }

#endif /* __SELINUX_HOOK_H */
