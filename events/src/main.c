#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
// Define and initialize the event object
K_EVENT_DEFINE(my_event);

// Producer thread stack size and priority
#define PRODUCER_STACK_SIZE 512
#define PRODUCER_PRIORITY 1

// Consumer thread stack size and priority
#define CONSUMER_STACK_SIZE 512
#define CONSUMER_PRIORITY 2

// Event mask
#define EVENT_1 0x01
#define EVENT_2 0x02

// Producer thread function
void producer_thread(void)
{
    while (1) {
        // Set Event 1
        printk("Producer: Setting EVENT_1\n");
        k_event_set(&my_event, EVENT_1);

        // Wait for a second
        k_sleep(K_SECONDS(1));

        // Post Event 2
        printk("Producer: Posting EVENT_2\n");
        k_event_post(&my_event, EVENT_2);

        // Wait for another second
        k_sleep(K_SECONDS(1));
    }
}

// Consumer thread function
void consumer_thread(void)
{
    uint32_t events;

    while (1) {
        // Wait for either EVENT_1 or EVENT_2
        events = k_event_wait(&my_event, EVENT_1 | EVENT_2, false, K_FOREVER);

        if (events & EVENT_1) {
            printk("Consumer: EVENT_1 received\n");
        }
        if (events & EVENT_2) {
            printk("Consumer: EVENT_2 received\n");
        }

        // Optional: Clear the event object for reuse
        k_event_set(&my_event, 0);
    }
}

// Define thread stacks
K_THREAD_STACK_DEFINE(producer_stack, PRODUCER_STACK_SIZE);
K_THREAD_STACK_DEFINE(consumer_stack, CONSUMER_STACK_SIZE);

// Define thread data
struct k_thread producer_thread_data;
struct k_thread consumer_thread_data;

// Main function
void main(void)
{
    printk("Starting Event Object Demo\n");

    // Create producer thread
    k_thread_create(&producer_thread_data, producer_stack, PRODUCER_STACK_SIZE,
                    (k_thread_entry_t)producer_thread, NULL, NULL, NULL,
                    PRODUCER_PRIORITY, 0, K_NO_WAIT);

    // Create consumer thread
    k_thread_create(&consumer_thread_data, consumer_stack, CONSUMER_STACK_SIZE,
                    (k_thread_entry_t)consumer_thread, NULL, NULL, NULL,
                    CONSUMER_PRIORITY, 0, K_NO_WAIT);
}
