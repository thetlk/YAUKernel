#ifndef PIC_H
#define PIC_H

void init_pic();
void pic_send_eoi(unsigned char irq);

#define MASTER_IDT_OFFSET 0x20
#define SLAVE_IDT_OFFSET  0x70

#define PIC_MASTER			0x20		/* IO base address for master PIC */
#define PIC_SLAVE			0xA0		/* IO base address for slave PIC */
#define PIC_MASTER_COMMAND	PIC_MASTER
#define PIC_MASTER_DATA		(PIC_MASTER+1)
#define PIC_SLAVE_COMMAND	PIC_SLAVE
#define PIC_SLAVE_DATA		(PIC_SLAVE+1)

#define PIC_EOI 0x20 /* end of interrupt */

/* ICW1 values */
#define ICW1_ICW4		0x01	/* with ICW4 (0 = without) */
#define ICW1_SINGLE 	0x02 	/* single mode (0 = cascade) */
#define ICW1_INTERVAL4 	0x04 	/* call address interval 4 (0 = 8 interval) */
#define ICW1_LEVEL 		0x08 	/* level triggered (0 = front edge) */
#define ICW1_INIT 		0x10	/* initialization */

/* ICW4 values */
#define ICW4_8086		0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO		0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM		0x10		/* Special fully nested (not) */

#endif