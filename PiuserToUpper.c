#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init keyboard_module_init(void)
{
    // Initialization code goes here

    return 0;
}

static void __exit keyboard_module_exit(void)
{
    // Cleanup code goes here
}


static int keyboard_handler(struct notifier_block *nb, unsigned long action, void *data)
{
    // Keyboard event handling code goes here

    return NOTIFY_OK;
}

static struct notifier_block keyboard_nb = {
    .notifier_call = keyboard_handler
};


static int keyboard_handler(struct notifier_block *nb, unsigned long action, void *data)
{
    const char *username = getenv("USERNAME");
    if (username && strcmp(username, "pi") == 0)
    {
        struct keyboard_notifier_param *param = data;
        int i;
        for (i = 0; i < param->down; i++)
        {
            param->value[i] = toupper(param->value[i]);
        }
    }

    return NOTIFY_OK;
}

static int __init keyboard_module_init(void)
{
    register_keyboard_notifier(&keyboard_nb);

    return 0;
}

static void __exit keyboard_module_exit(void)
{
    unregister_keyboard_notifier(&keyboard_nb);
}
