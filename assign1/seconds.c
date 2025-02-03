#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "seconds"
#define MESSAGE "Hello World\n"


// Function prototypes, variables, and the like

//ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static const struct proc_ops my_proc_ops = {
        //.proc_read = proc_read,
};


unsigned long my_jiffies;
unsigned long my_HZ;


/* This function is called when the module is loaded. */
int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");

    printk(KERN_INFO "jiffies = %lu\n",jiffies);
	printk(KERN_INFO "HZ = %d\n",HZ);
	
	my_jiffies = jiffies;
    my_HZ = HZ;


// **********************************

    proc_create(PROC_NAME, 0, NULL, &my_proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;



} 
// Loads kernel module, queries for (and saves in global variables) the system's HZ value and the jiffies value at module startup
// Creates a /proc file named /proc/seconds when the kernel module is loaded.

// Write a function that reports when cat command is called.

/*{

    printk(KERN_INFO "jiffies = %lu\n",jiffies);
    //printk(KERN_INFO "jiffies = %lu\n",jiff);
    printk(KERN_INFO "time elapsed= %lu\n",((jiffies-my_jiffies)/my_HZ)); // time between loading and removing this kernel module, as calculated by the computer timer interrupts & refresh rate
    // Note: the above is reported as an integer number of seconds, as printk cannot handle float

}*/



/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

    printk(KERN_INFO "%lu\n", gcd(3300,24));
    
    //printk(KERN_INFO "jiffies = %lu\n",jiffies);
    //printk(KERN_INFO "jiffies = %lu\n",jiff);
    //printk(KERN_INFO "time elapsed= %lu\n",((jiffies-my_jiffies)/my_HZ)); // time between loading and removing this kernel module, as calculated by the computer timer interrupts & refresh rate
    // Note: the above is reported as an integer number of seconds, as printk cannot handle float

// *********************************

    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);

}
// Also removes /proc/seconds



module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("Wilson Liu & Andy Liang");
