
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>

/* Define the size of the stack for each thread */
#define STACKSIZE 1024

/* Define thread priority (lower numbers indicate higher priority) */
#define PRIORITY1 7
#define PRIORITY2 5

/* Thread stack memory allocation */
K_THREAD_STACK_DEFINE(thread1_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACKSIZE);

/* Define the thread control blocks */
struct k_thread thread1_data;
struct k_thread thread2_data;

/* Thread entry point function */
void thread_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        printk("%s is running .. \n", k_thread_name_get(k_current_get()));
        k_msleep(2000);
    }
}


int main()
{
    #if CONFIG_SMP
        printk("Number of CPUs available: %d\n", arch_num_cpus());
    #endif

    /* Create Thread 1 */
    k_thread_create(&thread1_data, thread1_stack,
                    K_THREAD_STACK_SIZEOF(thread1_stack),
                    thread_entry,
                    NULL, NULL, NULL,
                    PRIORITY1, 0, K_NO_WAIT);

    /* Create Thread 2 */
    k_thread_create(&thread2_data, thread2_stack,
                    K_THREAD_STACK_SIZEOF(thread2_stack),
                    thread_entry,
                    NULL, NULL, NULL,
                    PRIORITY2, 0, K_NO_WAIT);

    k_thread_name_set(&thread1_data, "Thread_1");
    k_thread_name_set(&thread2_data, "Thread_2");

    
    return 0;
}
