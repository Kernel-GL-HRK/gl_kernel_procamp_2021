#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h>
#include <linux/interrupt.h>

#define INTERR 17  // default interrupt

static int irq = INTERR;
static int dev_id = 0;
static int irq_counter = 0;

module_param(irq, int, S_IRUGO);

static irqreturn_t my_interrupt(int irq, void* devid)
{
    irq_counter++;
    pr_info("In the ISR: counter = %d\n", irq_counter);
    pr_info("in irq(): %s\n", in_irq()? "Y" : "N");
    return IRQ_NONE;
}

static int hello_init(void)
{
	int res = 0;
    if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interr", &dev_id)) {
        res = -1;
        pr_info("mymodule: failed to load\n");
    } else {
        pr_info("mymodule: module loaded, irq handled on IRQ = %d\n", irq);
        pr_info("TASK: in irq(): %s\n", in_irq()? "Y" : "N");
    }
	
	return res;
}

static void hello_exit(void)
{
    synchronize_irq(irq);
    free_irq(irq, &dev_id);
	pr_info("module exited, counter: %d\n", irq_counter);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Oleksandr Posukhov <alex.posukhov@gmail.com>");
MODULE_DESCRIPTION("IRQ example module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");