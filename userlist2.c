#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/mutex.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ski");

static int __init logindetect_init(void)
{
    struct task_struct *task;
    struct tty_struct *tty;
    struct mutex *lock;
    char *name;

    printk(KERN_INFO "List of logged-in users:\n");

    for_each_process(task) {
        if (task->signal->tty) {
            tty = task->signal->tty;
            lock = &tty->termios_mutex;
            mutex_lock(lock);

            if (strncmp(tty->name, "pts/", 4) == 0) {
                name = tty->name + 4;
                printk(KERN_INFO "%s (pid: %d, tty: %s, login: %s)\n", task->comm, task->pid, tty->name, name);
            }

            mutex_unlock(lock);
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
