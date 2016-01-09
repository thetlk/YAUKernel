#include <driver/pic.h>
#include <sys/asm.h>
#include <driver/video.h>

void pic_init()
{
    video_print("Init PIC ... ");

    /*
    L'initialisation de fait en plusieurs étapes :
        - on balance un INIT sur le command 
        - le PIC attend alors sur data : ICW2, ICW3 puis ICW4 (si présent)
    */

    /* Initialisation de ICW1 */
    outb(PIC_MASTER_COMMAND, ICW1_INIT + ICW1_ICW4); /* debut de la séquence d'init (cascade mode avec ICW4) */
    io_wait();
    outb(PIC_SLAVE_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();

    /* Initialisation de ICW2 */
    /* setup idt offsets */
    outb(PIC_MASTER_DATA, MASTER_IDT_OFFSET);
    io_wait();
    outb(PIC_SLAVE_DATA, SLAVE_IDT_OFFSET);
    io_wait();

    /* Initialisation de ICW3 */
    outb(PIC_MASTER_DATA, 1<<2); /* slave on wire 2 - 0000 0100 */
    io_wait();
    outb(PIC_SLAVE_DATA, 2); /* wire number - 0000 0010 */
    io_wait();

    /* Initialisation de ICW4 */
    outb(PIC_MASTER_DATA, ICW4_8086);
    io_wait();
    outb(PIC_SLAVE_DATA, ICW4_8086);
    io_wait();

    /* masquage des interruptions */
    outb(PIC_MASTER_DATA, 0x0);
    io_wait();
    outb(PIC_SLAVE_DATA, 0x0);
    io_wait();

    video_print_ok();
}

void pic_send_eoi(unsigned char irq)
{
    if(irq >= 8)
    {
        outb(PIC_SLAVE_COMMAND, PIC_EOI);
    }

    outb(PIC_MASTER_COMMAND, PIC_EOI);
}
