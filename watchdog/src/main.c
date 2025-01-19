
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/task_wdt/task_wdt.h>
#include <zephyr/sys/printk.h>

static void task_wdt_callback(int channel_id, void *user_data)
{
	printk("Task watchdog channel %d callback, thread: %s\n",
		channel_id, k_thread_name_get((k_tid_t)user_data));

	printk("Resetting device...\n");

	sys_reboot(SYS_REBOOT_COLD);
}

void control_thread(void)
{
	int task_wdt_id;
	int count = 0;

	printk("Control thread started.\n");

	task_wdt_id = task_wdt_add(100U, task_wdt_callback,
		(void *)k_current_get());

	while (true) {
		if (count == 50) {
			printk("Control thread getting stuck...\n");
			k_sleep(K_FOREVER);
		}

		task_wdt_feed(task_wdt_id);
		k_sleep(K_MSEC(50));
		count++;
	}
}


int main(void)
{
	int ret;
	const struct device *const hw_wdt_dev = NULL;

	printk("Task watchdog sample application.\n");

	if (!device_is_ready(hw_wdt_dev)) {
		printk("Hardware watchdog not ready; ignoring it.\n");
	}

	ret = task_wdt_init(hw_wdt_dev);
	if (ret != 0) {
		printk("task wdt init failure: %d\n", ret);
		return 0;
	}


	/* passing NULL instead of callback to trigger system reset */
	int task_wdt_id = task_wdt_add(1100U, NULL, NULL);

	while (true) {
		printk("Main thread still alive...\n");
		task_wdt_feed(task_wdt_id);
		k_sleep(K_MSEC(1000));
	}
	return 0;
}
K_THREAD_DEFINE(control, 1024, control_thread, NULL, NULL, NULL, -1, 0, 1000);