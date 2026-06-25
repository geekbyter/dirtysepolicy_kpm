#include <linux/kernel.h>
#include <linux/printk.h>
#include <kpmodule.h>

#ifndef SMOKE_VERSION
#define SMOKE_VERSION "0.1.0"
#endif

KPM_NAME("dirtyduck_smoke");
KPM_VERSION(SMOKE_VERSION);
KPM_LICENSE("GPLv3");
KPM_AUTHOR("geekbyte");
KPM_DESCRIPTION("No-op KPM smoke test for SuKiSU/KernelPatch loading");

static long init(const char *args, const char *event, void *__user reserved)
{
    (void)args;
    (void)reserved;
    pr_info("[dirtyduck_smoke] init event=%s version=%s\n",
            event ?: "(null)", SMOKE_VERSION);
    return 0;
}

static long control(const char *args, char *__user out_msg, int outlen)
{
    (void)args;
    (void)out_msg;
    (void)outlen;
    pr_info("[dirtyduck_smoke] control\n");
    return 0;
}

static long exit_(void *__user reserved)
{
    (void)reserved;
    pr_info("[dirtyduck_smoke] exit\n");
    return 0;
}

KPM_INIT(init);
KPM_CTL0(control);
KPM_EXIT(exit_);
