#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/irq.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/kernel.h>

#include <stdio.h>

#define MY_IRQ_LINE 1


#define MY_IRQ_PRIORITY 2

int count = 0;

void my_isr_handler(void *arg)
{
    printk("ISR executed! Argument passed : %p , Interrupt Called for %d times\n", arg, count++);
    k_sleep(K_SECONDS(5));
}


int main(void)
{
    printk("Starting Software Interrupt Example for %s Architecture...\n",CONFIG_BOARD);


    IRQ_CONNECT(MY_IRQ_LINE, MY_IRQ_PRIORITY, my_isr_handler, NULL, 0);


    irq_enable(MY_IRQ_LINE);

 
    while (1) {

        k_sleep(K_MSEC(2000));  


        printk("Triggering the software interrupt...\n");


        /* On real hardware, this would be triggered by a peripheral */
        
       NVIC_SetPendingIRQ(MY_IRQ_LINE);  // For ARM

        // __asm__ volatile("csrw mip, %0" : : "r"(1 << MY_IRQ_LINE)); // risc v        
    }
    return 0;
}