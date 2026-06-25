# Recovery Notes

Do not test a new KPM by enabling boot autoload first.

## Disable The KPM Loader

From a working root shell:

```sh
su -c 'touch /data/adb/dirtysepolicy_kpm/disable; rm -f /data/adb/dirtysepolicy_kpm/enable-autoload'
```

Disable the ordinary module entirely:

```sh
su -c 'touch /data/adb/modules/dirtysepbypass_kpm/disable; rm -rf /data/adb/modules_update/dirtysepbypass_kpm'
```

## Disable All Root Modules

Use this only when the phone can reach recovery or a root shell:

```sh
for d in /data/adb/modules/*; do
  touch "$d/disable"
done
rm -rf /data/adb/modules_update/*
```

## Safe Test Order

1. Load smoke KPM manually.
2. Reboot and confirm normal boot.
3. Load dirtyduck KPM manually.
4. Reboot and confirm normal boot.
5. Enable autoload only after the previous two steps are boring.
