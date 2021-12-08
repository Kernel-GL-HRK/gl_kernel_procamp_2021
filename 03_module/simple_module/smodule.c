#include <linux/module.h>

static int a = 1;
static int b = 2;

module_param(a, int, 0660);
module_param(b, int, 0660);

static int smodule_init(void)
{
    pr_info("SM init: a = %d, b = %d. a + b = %d", a, b, a + b);
    return 0;
}

static void smodule_exit(void)
{
    pr_info("SM exit: a = %d, b = %d. a - b = %d", a, b, a - b);
}

module_init(smodule_init);
module_exit(smodule_exit);

MODULE_LICENSE("GPL v2");
