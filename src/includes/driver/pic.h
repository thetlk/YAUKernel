#ifndef PIC_H
#define PIC_H

void init_pic();
void pic_send_eoi(unsigned char irq);

#define PIC_MASTER			0x20		/* IO base address for master PIC */
#define PIC_SLAVE			0xA0		/* IO base address for slave PIC */
#define PIC_MASTER_COMMAND	PIC_MASTER
#define PIC_MASTER_DATA		(PIC_MASTER+1)
#define PIC_SLAVE_COMMAND	PIC_SLAVE
#define PIC_SLAVE_DATA		(PIC_SLAVE+1)

#define PIC_EOI 0x20 /* end of interrupt */

#endif