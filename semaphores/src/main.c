#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>

struct k_sem sem; 

#define STACKSIZE 1024

/* Thread stack memory allocation */
K_THREAD_STACK_DEFINE(thread1_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACKSIZE);

/* Define the thread control blocks */
struct k_thread thread1_data;
struct k_thread thread2_data;


void thread1(void) {
    while (1) {
        printk("Thread1: Waiting for semaphore...\n");
        k_sem_take(&sem, K_FOREVER); 
        printk("Thread1: Semaphore acquired!\n");
        k_msleep(1000);              
        k_sem_give(&sem);            
        printk("Thread1: Semaphore released.\n");
        k_msleep(2000);             
    }
}

void thread2(void) {
    while (1) {
        printk("Thread2: Waiting for semaphore...\n");
        k_sem_take(&sem, K_FOREVER);
        printk("Thread2: Semaphore acquired!\n");
        k_msleep(1000);             
        k_sem_give(&sem);            
        printk("Thread2: Semaphore released.\n");
        k_msleep(3000);              
    }
}

void main(void) {
    k_sem_init(&sem, 1, 1);  
    printk("Main thread: Semaphore initialized.\n");

    k_thread_create(&thread1_data, thread1_stack,
                    K_THREAD_STACK_SIZEOF(thread1_stack),
                    thread1,
                    NULL, NULL, NULL,
                    0, 0, K_NO_WAIT);

    /* Create Thread 2 */
    k_thread_create(&thread2_data, thread2_stack,
                    K_THREAD_STACK_SIZEOF(thread2_stack),
                    thread2,
                    NULL, NULL, NULL,
                    0, 0, K_NO_WAIT);
    return 0;
}