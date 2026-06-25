# KPM Plan

The reliable path for DuckDetector's dirty sepolicy rule is kernel-side:

- Duck's app_zygote carrier writes queries to `/sys/fs/selinux/access`.
- A normal module daemon cannot alter another app process' read/write result.
- Zygisk can alter user-space results, but it depends on process injection and has proven risky in the current SuKiSU/GKI environment.
- KPM can hook the SELinux kernel query path directly and avoid target-process injection.

## Implementation Split

- `kpm/smoke`: no-op load test.
- `kpm/dirtyduck`: SELinux filter forked from `Admirepowered/selinux_hook`.
- `module-kpm`: ordinary module used only as a safe loader wrapper.

## First Functional Goal

Make the KPM return clean-policy results for app UID SELinux probes while leaving root/policy-manager UIDs on the live policy path.

Important hooks inherited from the reference implementation:

- `/sys/fs/selinux/access` write path
- `/sys/fs/selinux/context` write path
- `/sys/fs/selinux/status` read/mmap path
- `/proc/self/attr/current` through `security_setprocattr`
- `security_read_policy`
- `context_struct_compute_av`

## Non-Goals

- No global Zygisk injection.
- No boot autoload until manual KPM load is validated.
- No persistent GKI/module flashing from scripts in this repository.
