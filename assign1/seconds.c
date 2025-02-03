#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/sched.h>

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

    return 0;
} // Loads kernel module, queries for (and saves in global variables) the system's HZ value and the jiffies value at module startup


// Write a function that reports when cat command is called.
/*
{

    printk(KERN_INFO "jiffies = %lu\n",jiffies);
    //printk(KERN_INFO "jiffies = %lu\n",jiff);
    printk(KERN_INFO "time elapsed= %lu\n",((jiffies-my_jiffies)/my_HZ)); // time between loading and removing this kernel module, as calculated by the computer timer interrupts & refresh rate
    // Note: the above is reported as an integer number of seconds, as printk cannot handle float

}
*/


/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

    // printk(KERN_INFO "%lu\n", gcd(3300,24));
    
    printk(KERN_INFO "jiffies = %lu\n",jiffies);
    //printk(KERN_INFO "jiffies = %lu\n",jiff);
    printk(KERN_INFO "time elapsed= %lu\n",((jiffies-my_jiffies)/my_HZ)); // time between loading and removing this kernel module, as calculated by the computer timer interrupts & refresh rate
    // Note: the above is reported as an integer number of seconds, as printk cannot handle float
}



module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("Wilson Liu & Andy Liang");
