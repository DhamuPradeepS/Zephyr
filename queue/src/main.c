#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

/* Define message structure */
struct data_item_type {
    uint32_t value;
};

/* Define and initialize the message queue */
#define MSGQ_SIZE 10
K_MSGQ_DEFINE(my_msgq, sizeof(struct data_item_type), MSGQ_SIZE, 4);

/* Producer thread function */
void producer_thread(void)
{
    struct data_item_type data;
    uint32_t count = 0;

    while (1) {
        /* Generate data */
        data.value = count++;
        printk("Producer: Sending data %d\n", data.value);

        /* Try to add data to the queue */
        while (k_msgq_put(&my_msgq, &data, K_NO_WAIT) != 0) {
            /* If the queue is full, purge old messages */
            printk("Producer: Queue full, purging...\n");
            k_msgq_purge(&my_msgq);
        }

        /* Simulate some delay */
        k_sleep(K_MSEC(5000));
    }
}

/* Consumer thread function */
void consumer_thread(void)
{
    struct data_item_type data;

    while (1) {
        /* Wait for data to be available in the queue */
        k_msgq_get(&my_msgq, &data, K_FOREVER);

        /* Process the data */
        printk("Consumer: Received data %d\n", data.value);

        /* Simulate some processing delay */
        k_sleep(K_MSEC(1000));
    }
}

/* Thread definitions */
K_THREAD_DEFINE(producer_tid, 1024, producer_thread, NULL, NULL, NULL,
                1, 0, 0);
K_THREAD_DEFINE(consumer_tid, 1024, consumer_thread, NULL, NULL, NULL,
                1, 0, 0);