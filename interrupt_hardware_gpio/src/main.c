#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
#include <zephyr/irq.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/arch/cpu.h>

#define SLEEP_TIME_MS   2000


/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define GPIO1_NODE DT_NODELABEL(gpio0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec gpio2 = GPIO_DT_SPEC_GET(GPIO1_NODE, gpios);


void gpio_isr_handler(){
    printk("Interrupt triggered");
    int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

    int i = 0;
	while (i<=2) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
        i++;
	}
}


int main(){
    if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&gpio2) ) {
		return 0;
	}

    int ret1 = gpio_pin_configure_dt(&gpio2,GPIO_INPUT);

    if (ret1 != 0) {
		return 0;
	}

    printk("Starting GPIO interrupt simulation...\n");

    while (1) {

        if(gpio_pin_get_dt(&gpio2) == 1){
            gpio_isr_handler();
        }
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}