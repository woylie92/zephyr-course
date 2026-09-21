#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int scratch_init(void)
{
	printk("Board Initialized\n");
	return 0;
}

SYS_INIT(scratch_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);