#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/sched.h>

unsigned long jiff;

/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");

	printk(KERN_INFO "HZ = %d\n",HZ);
	printk(KERN_INFO "jiffies = %lu\n",jiffies);
	jiff = jiffies;
	
	printk(KERN_INFO "pid: %d\n", init_task.pid); // pid_t is a signed int
	printk(KERN_INFO "state: %ld\n", init_task.__state);
	printk(KERN_INFO "flags: %u\n", init_task.flags);
	printk(KERN_INFO "rt_priority: %ld\n", init_task.rt_priority);
	printk(KERN_INFO "policy: %u\n", init_task.policy);
	printk(KERN_INFO "tgid: %d\n", init_task.tgid);

	printk(KERN_INFO "pid: %d\n", current->pid);
	printk(KERN_INFO "state: %ld\n", current->__state);
	printk(KERN_INFO "flags: %u\n", current->flags);
	printk(KERN_INFO "rt_priority: %ld\n", current->rt_priority);
	printk(KERN_INFO "policy: %u\n", current->policy);
	printk(KERN_INFO "tgid: %d\n", current->tgid);

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

        printk(KERN_INFO "%lu\n", gcd(3300,24));
        
        printk(KERN_INFO "jiffies = %lu\n",jiffies);
        //printk(KERN_INFO "jiffies = %lu\n",jiff);
        printk(KERN_INFO "time elapsed= %lu\n",((jiffies-jiff)/1000)); // time between loading and removing this kernel module, as calculated by the computer timer interrupts & refresh rate
        // Note: the above is reported as an integer number of seconds, as printk cannot handle float
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

