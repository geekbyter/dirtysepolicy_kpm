# DirtySepolicy KPM

Standalone KernelPatch/SuKiSU KPM source tree for the DirtySepolicy and
DuckDetector SELinux query bypass path.

Author: geekbyte

This directory is split out from `DirtySepolicy_Bypass`. The Zygisk module stays
in `../DirtySepolicy_Bypass`; this directory contains only the kernel-side KPM
implementation and related KPM documentation.

## Layout

- `dirtyduck/`: main SELinux access/status/context/procattr filter KPM.
- `smoke/`: no-op KPM used to validate that the device can load KPM safely.
- `docs/kpm-plan.md`: implementation notes and non-goals.
- `docs/recovery.md`: recovery and emergency disable notes.
- `.github/workflows/build-kpm.yml`: GitHub Actions cloud build workflow.

## What The KPM Handles

`dirtyduck_selinux` targets kernel-side SELinux query surfaces used by
DirtySepolicy and DuckDetector:

- `/sys/fs/selinux/access`
- `/sys/fs/selinux/context`
- `/sys/fs/selinux/status` read and mmap paths
- `/proc/self/attr/current` through procattr hooks
- clean-policy reads through `security_read_policy`
- AV decisions through `context_struct_compute_av`

The KPM keeps policy-manager/root maintenance paths on the live policy route
and returns clean-policy results for probe-style app queries.

## GitHub Cloud Build

Use this directory as a repository root for the simplest setup. If you keep it
as `dirtysepolicy_kpm` inside a larger repository, copy
`.github/workflows/build-kpm.yml` to the repository root `.github/workflows/`
directory; GitHub only discovers workflows from the repository root. The YAML
itself supports both project layouts.

1. Push the directory to GitHub.
2. Open **Actions**.
3. Run **Build DirtySepolicy KPM** manually, or push a change to source files.
4. Download the `dirtysepolicy-kpm` artifact.

The artifact contains:

- `dirtyduck_smoke_0.1.0.kpm`
- `dirtyduck_selinux_0.1.4.kpm`

The workflow downloads KernelPatch commit
`0ceeeb968bef86b48af68640af8b135215dc3399`, which matches the newer
`hotpatch_nosync` API used by the original working DirtySepolicy KPM path. It
then installs Android NDK `29.0.14206865` and runs the root `Makefile`.

If you intentionally build against older KernelPatch `0.11.2`, pass
`DIRTYDUCK_HOTPATCH_KP0112=1`; otherwise the produced KPM may compile but fail
to load on devices whose KernelPatch export table expects `hotpatch_nosync`.

## Local Build

```sh
cd dirtysepolicy_kpm

# Option A: place the matching KernelPatch SDK beside this README.
curl -L https://github.com/bmax121/KernelPatch/archive/0ceeeb968bef86b48af68640af8b135215dc3399.tar.gz -o KernelPatch.tar.gz
mkdir -p KernelPatch
tar xzf KernelPatch.tar.gz --strip-components=1 -C KernelPatch

# Point this to your local NDK.
export ANDROID_NDK_HOME=/path/to/android-ndk

make
```

Outputs are copied to the project root:

```text
dirtyduck_smoke_0.1.0.kpm
dirtyduck_selinux_0.1.4.kpm
```

## Install And Test Order

Do not enable boot autoload for a new KPM first.

1. Load `dirtyduck_smoke_0.1.0.kpm` manually from the KPM manager.
2. Check kernel logs for `[dirtyduck_smoke] init`.
3. Reboot once and confirm the device still boots normally.
4. Load `dirtyduck_selinux_0.1.4.kpm` manually.
5. Use the KPM manager control command:

```sh
kpm ctl dirtysepolicy_duck_kpm status
kpm ctl dirtysepolicy_duck_kpm warm
```

6. Run DirtySepolicy or DuckDetector checks.

## Emergency Disable

If you use a separate loader later, keep an emergency disable flag available:

```sh
su -c 'touch /data/adb/dirtysepolicy_kpm/disable; rm -f /data/adb/dirtysepolicy_kpm/enable-autoload'
```

See `docs/recovery.md` for the longer recovery checklist.
