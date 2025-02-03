/**
 * hello.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * Samkith jain September 6, 2024
 * Comp Sci 3SH3, Fall 2024
 * Reference:  hello.c Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/sched.h>

#define BUFFER_SIZE 128

#define PROC_NAME "hello"
#define MESSAGE "Hello World\n"

/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static const struct proc_ops my_proc_ops = {
        .proc_read = proc_read,
};

unsigned long my_jiffies;
unsigned long my_HZ;


/* This function is called when the module is loaded. */
int proc_init(void)
{

printk(KERN_INFO "Loading Module\n");

    printk(KERN_INFO "jiffies = %lu\n",jiffies);
	printk(KERN_INFO "HZ = %d\n",HZ);
	
	my_jiffies = jiffies;
    my_HZ = HZ;



        // creates the /proc/hello entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &my_proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

	printk(KERN_INFO "Removing Module\n");

    printk(KERN_INFO "%lu\n", gcd(3300,24));

        // removes the /proc/hello entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        unsigned long time_elapsed = 0; // If 0 reported, then you know it's not working (as long as you take one second to run the cat /proc/seconds command at least)

        if (completed) {
                completed = 0;
                return 0;
        }

	// printk(KERN_INFO "jiffies at read time = %lu\n",jiffies);
	// printk(KERN_INFO "time elapsed since kernel module loaded = %lu\n",((jiffies-my_jiffies)/my_HZ));
	
	time_elapsed = ((jiffies-my_jiffies)/my_HZ);
	
        completed = 1;

        rv = sprintf(buffer, "%lu\n", time_elapsed);

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;



    printk(KERN_INFO "jiffies at read time = %lu\n",jiffies);
    //printk(KERN_INFO "jiffies = %lu\n",jiff);
    printk(KERN_INFO "time elapsed= %lu\n",((jiffies-my_jiffies)/my_HZ)); // time between loading and removing this kernel module, as calculated by the computer timer interrupts & refresh rate
    // Note: the above is reported as an integer number of seconds, as printk cannot handle float


}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");

