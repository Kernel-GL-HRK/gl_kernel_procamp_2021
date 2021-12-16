#include <linux/module.h>
#include <linux/slab.h>

static int works = 2;
module_param(works, int, S_IRUGO);

static struct workqueue_struct *my_wk;

typedef struct {
    struct work_struct my_work;
    int id;
    unsigned int i;
    cycles_t cycles;
} my_work_t;

static void wq_func(struct work_struct *work) {
    unsigned int j = jiffies;
    cycles_t cycles = get_cycles();
    my_work_t *wrk = (my_work_t*) work;
    printk( KERN_INFO"worker: %010llu [%05u] id <%d>\n",
        (unsigned long long) (cycles - wrk->cycles), (unsigned int) j - wrk->i,
        wrk->id);
    kfree(wrk);
    wrk = NULL;
}

static int hello_init(void)
{
	int n, res = 0;
    unsigned int i = jiffies;
    cycles_t cycles = get_cycles();

    if ( my_wk = create_workqueue("my_queue")){
        for (n = 0; n < works; n++) {
            my_work_t *work = (my_work_t*) kmalloc(sizeof(my_work_t), GFP_KERNEL);
            if (work) {
                INIT_WORK((struct work_struct*) work, wq_func);
                work->id = n;
                work->i = i;
                work->cycles = cycles;
                res = queue_work(my_wk, (struct work_struct*) work);
                if (!res) return -EPERM;
            } else {
                return -ENOMEM;
            }
        }
    } else {
        return -EINVAL;
    }

    pr_info("init : %010llu [%05u] \n",
        (unsigned long long) cycles, i);
	return 0;
}

static void hello_exit(void)
{
    flush_workqueue(my_wk);
    destroy_workqueue(my_wk);
	pr_info("module exited\n");
    return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Oleksandr Posukhov <alex.posukhov@gmail.com>");
MODULE_DESCRIPTION("Tasklet example module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");