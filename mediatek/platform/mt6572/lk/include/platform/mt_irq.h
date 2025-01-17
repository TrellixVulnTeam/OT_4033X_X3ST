
#ifndef __MT_IRQ_H__
#define __MT_IRQ_H__

#define GIC_CPU_CTRL			0x00
#define GIC_CPU_PRIMASK			0x04
#define GIC_CPU_BINPOINT		0x08
#define GIC_CPU_INTACK			0x0c
#define GIC_CPU_EOI			0x10
#define GIC_CPU_RUNNINGPRI		0x14
#define GIC_CPU_HIGHPRI			0x18

#define GIC_DIST_CTRL			0x000
#define GIC_DIST_CTR			0x004
#define GIC_DIST_ENABLE_SET		0x100
#define GIC_DIST_ENABLE_CLEAR		0x180
#define GIC_DIST_PENDING_SET		0x200
#define GIC_DIST_PENDING_CLEAR		0x280
#define GIC_DIST_ACTIVE_BIT		0x300
#define GIC_DIST_PRI			0x400
#define GIC_DIST_TARGET			0x800
#define GIC_DIST_CONFIG			0xc00
#define GIC_DIST_SOFTINT		0xf00
#define GIC_DIST_ICDISR  		0x80
enum {IRQ_MASK_HEADER = 0xF1F1F1F1, IRQ_MASK_FOOTER = 0xF2F2F2F2};

struct mtk_irq_mask
{
    unsigned int header;   /* for error checking */
    unsigned int mask0;
    unsigned int mask1;
    unsigned int mask2;
    unsigned int mask3;
    unsigned int mask4;
    unsigned int mask5;
    unsigned int mask6;
    unsigned int mask7;
    unsigned int footer;   /* for error checking */
};




#define GIC_PRIVATE_SIGNALS (32)

#define GIC_PPI_OFFSET          (27)
#define GIC_PPI_GLOBAL_TIMER    (GIC_PPI_OFFSET + 0)
#define GIC_PPI_LEGACY_FIQ      (GIC_PPI_OFFSET + 1)
#define GIC_PPI_PRIVATE_TIMER   (GIC_PPI_OFFSET + 2)
#define GIC_PPI_WATCHDOG_TIMER  (GIC_PPI_OFFSET + 3)
#define GIC_PPI_LEGACY_IRQ      (GIC_PPI_OFFSET + 4)

#define MT_USB0_IRQ_ID                  (GIC_PRIVATE_SIGNALS + 20)
#define MT_GPT_IRQ_ID                   (GIC_PRIVATE_SIGNALS + 74)
#define MT_DSI_IRQ_ID                   (GIC_PRIVATE_SIGNALS + 104) //TBD
#define MT_MSDC0_IRQ_ID                 (GIC_PRIVATE_SIGNALS + 26)
#define MT_MSDC1_IRQ_ID                 (GIC_PRIVATE_SIGNALS + 27)
#define MT_NFIECC_IRQ_ID                (GIC_PRIVATE_SIGNALS + 55)
#define MT_NFI_IRQ_ID                   (GIC_PRIVATE_SIGNALS + 56)

#define MT_NR_PPI   (5)
#define MT_NR_SPI   (128)
#define NR_MT_IRQ_LINE  (GIC_PPI_OFFSET + MT_NR_PPI + MT_NR_SPI)    // 5 PPIs and 128 SPIs

#define MT65xx_EDGE_SENSITIVE 0
#define MT65xx_LEVEL_SENSITIVE 1

#define MT65xx_POLARITY_LOW   0
#define MT65xx_POLARITY_HIGH  1


int mt_irq_mask_all(struct mtk_irq_mask *mask); //(This is ONLY used for the sleep driver)
int mt_irq_mask_restore(struct mtk_irq_mask *mask); //(This is ONLY used for the sleep driver)
void mt_irq_set_sens(unsigned int irq, unsigned int sens);
void mt_irq_set_polarity(unsigned int irq, unsigned int polarity);
int mt_irq_mask_restore(struct mtk_irq_mask *mask);
void mt_irq_unmask(unsigned int irq);
void mt_irq_ack(unsigned int irq);

void platform_init_interrupts(void);

#endif  /* !__MT_IRQ_H__ */
