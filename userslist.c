#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccc");

static int __init logindetect_init(void)
{
    struct task_struct *task;

    printk(KERN_INFO "List of logged-in users:\n");

    for_each_process(task) {
        if (task->signal->tty) {
            printk(KERN_INkoO "%s (pid: %d, tty: %s)\n", task->comm, task->pid, task->signal->tty->name);
        }
    }

    return 0;
}

static void __exit logindetect_exit(void)
{
    printk(KERN_INFO "Logged-in users detection module removed.\n");
}

module_init(logindetect_init);
module_exit(logindetect_exit);
/*
make
insmod userslist.c
*/
