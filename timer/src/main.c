#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Timer handler function */
void my_timer_expiry_function(struct k_timer *timer)
{
    printk("Timer expired!\n");
}

/* Define and initialize the timer */
K_TIMER_DEFINE(my_timer, my_timer_expiry_function, NULL);

void main(void)
{
    printk("Starting the timer example...\n");

    /* Start the timer with a 1-second timeout (1000 ms) */
    k_timer_start(&my_timer, K_SECONDS(1), K_NO_WAIT);

    /* Do other work while the timer is running */
    printk("Waiting for the timer to expire...\n");

    /* Wait for the timer to complete */
    k_sleep(K_SECONDS(2));

    printk("Timer example finished.\n");
}
