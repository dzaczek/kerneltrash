#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("A kernel module that changes keyboard input characters to X when the username is John or Adam");

// Function to change keyboard input characters to "X"
void change_keyboard_chars(struct input_dev *dev)
{
    int i;
    char *username = "John";

    // Check if the current username is "John" or "Adam"
    if (strcmp(current_username, username) == 0 || strcmp(current_username, "Adam") == 0)
    {
        // Loop through all keyboard input characters
        for (i = KEY_RESERVED; i < KEY_MAX; i++)
        {
            // Change the character to "X"
            set_bit(i, dev->keybit);
        }
    }
}

// Called when the module is loaded
static int __init change_keyboard_chars_init(void)
{
    // Register the function to be called when keyboard input events occur
    register_keyboard_chars_handler(change_keyboard_chars);

    return 0;
}

// Called when the module is unloaded
static void __exit change_keyboard_chars_exit(void)
{
    // Unregister the function that was called when keyboard input events occurred
    unregister_keyboard_chars_handler(change_keyboard_chars);
}

module_init(change_keyboard_chars_init);
module_exit(change_keyboard_chars_exit);
