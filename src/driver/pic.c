#include <driver/pic.h>
#include <sys/asm.h>
#include <driver/video.h>

void init_pic()
{
    video_print("Init PIC ... ");

    /* Initialisation de ICW1 */
    outb(PIC_MASTER_COMMAND, 0x11);
    outb(PIC_SLAVE_COMMAND, 0x11);

    /* Initialisation de ICW2 */
    outb(PIC_MASTER_DATA, 0x20);   /* vecteur de depart = 32 */
    outb(PIC_SLAVE_DATA, 0x70);   /* vecteur de depart = 96 */

    /* Initialisation de ICW3 */
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_SLAVE_DATA, 0x02);

    /* Initialisation de ICW4 */
    outb(PIC_MASTER_DATA, 0x01);
    outb(PIC_SLAVE_DATA, 0x01);

    /* masquage des interruptions */
    outb(PIC_MASTER_DATA, 0x0);
    outb(PIC_SLAVE_DATA, 0x0);

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
