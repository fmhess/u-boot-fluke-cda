/*
 *  Copyright (C) 2012 Altera Corporation <www.altera.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef	_RESET_MANAGER_H_
#define	_RESET_MANAGER_H_

void reset_cpu(ulong addr);
void reset_deassert_peripherals_handoff(void);
void reset_dma_peripheral_requests(int assert);

void socfpga_bridges_reset(int enable);

void socfpga_per_reset(u32 reset, int set);
void socfpga_per_reset_all(void);

struct socfpga_reset_manager {
	u32	status;
	u32	ctrl;
	u32	counts;
	u32	padding1;
	u32	mpu_mod_reset;
	u32	per_mod_reset;
	u32	per2_mod_reset;
	u32	brg_mod_reset;
	u32	misc_mod_reset;
	u32	padding2[12];
	u32	tstscratch;
};

#if defined(CONFIG_SOCFPGA_VIRTUAL_TARGET)
#define RSTMGR_CTRL_SWWARMRSTREQ_LSB 2
#else
#define RSTMGR_CTRL_SWWARMRSTREQ_LSB 1
#endif

#define RSTMGR_CTRL_SDRSELFREFEN_MASK 0x00000010
#define RSTMGR_CTRL_FPGAHSEN_MASK 0x00010000
#define RSTMGR_CTRL_ETRSTALLEN_MASK 0x00100000

/*
 * Define a reset identifier, from which a permodrst bank ID
 * and reset ID can be extracted using the subsequent macros
 * RSTMGR_RESET() and RSTMGR_BANK().
 */
#define RSTMGR_BANK_OFFSET	8
#define RSTMGR_BANK_MASK	0x7
#define RSTMGR_RESET_OFFSET	0
#define RSTMGR_RESET_MASK	0x1f
#define RSTMGR_DEFINE(_bank, _offset)		\
	((_bank) << RSTMGR_BANK_OFFSET) | ((_offset) << RSTMGR_RESET_OFFSET)

/* Extract reset ID from the reset identifier. */
#define RSTMGR_RESET(_reset)			\
	(((_reset) >> RSTMGR_RESET_OFFSET) & RSTMGR_RESET_MASK)

/* Extract bank ID from the reset identifier. */
#define RSTMGR_BANK(_reset)			\
	(((_reset) >> RSTMGR_BANK_OFFSET) & RSTMGR_BANK_MASK)

/*
 * SocFPGA Cyclone V/Arria V reset IDs, bank mapping is as follows:
 * 0 ... mpumodrst
 * 1 ... permodrst
 * 2 ... per2modrst
 * 3 ... brgmodrst
 * 4 ... miscmodrst
 */
#define RSTMGR_EMAC0		RSTMGR_DEFINE(1, 0)
#define RSTMGR_EMAC1		RSTMGR_DEFINE(1, 1)
#define RSTMGR_USB0	RSTMGR_DEFINE(1, 2)
#define RSTMGR_USB1	RSTMGR_DEFINE(1, 3)
#define RSTMGR_NAND		RSTMGR_DEFINE(1, 4)
#define RSTMGR_QSPI		RSTMGR_DEFINE(1, 5)
#define RSTMGR_L4WD0		RSTMGR_DEFINE(1, 6)
#define RSTMGR_L4WD1	RSTMGR_DEFINE(1, 7)
#define RSTMGR_OSC1TIMER0	RSTMGR_DEFINE(1, 8)
#define RSTMGR_OSC1TIMER1	RSTMGR_DEFINE(1, 9)
#define RSTMGR_SPTIMER0	RSTMGR_DEFINE(1, 10)
#define RSTMGR_SPTIMER1	RSTMGR_DEFINE(1, 11)
#define RSTMGR_I2C0	RSTMGR_DEFINE(1, 12)
#define RSTMGR_I2C1	RSTMGR_DEFINE(1, 13)
#define RSTMGR_I2C2	RSTMGR_DEFINE(1, 14)
#define RSTMGR_I2C3	RSTMGR_DEFINE(1, 15)
#define RSTMGR_UART0		RSTMGR_DEFINE(1, 16)
#define RSTMGR_UART1	RSTMGR_DEFINE(1, 17)
#define RSTMGR_SPIM0		RSTMGR_DEFINE(1, 18)
#define RSTMGR_SPIM1		RSTMGR_DEFINE(1, 19)
#define RSTMGR_SPIS0	RSTMGR_DEFINE(1, 20)
#define RSTMGR_SPIS1	RSTMGR_DEFINE(1, 21)
#define RSTMGR_SDMMC		RSTMGR_DEFINE(1, 22)
#define RSTMGR_CAN0	RSTMGR_DEFINE(1, 23)
#define RSTMGR_CAN1	RSTMGR_DEFINE(1, 24)
#define RSTMGR_GPIO0	RSTMGR_DEFINE(1, 25)
#define RSTMGR_GPIO1	RSTMGR_DEFINE(1, 26)
#define RSTMGR_GPIO2	RSTMGR_DEFINE(1, 27)
#define RSTMGR_DMA		RSTMGR_DEFINE(1, 28)
#define RSTMGR_SDR		RSTMGR_DEFINE(1, 29)

#define RSTMGR_DMAIF(n)	RSTMGR_DEFINE(2, (n))

/* Create a human-readable reference to SoCFPGA reset. */
#define SOCFPGA_RESET(_name)	RSTMGR_##_name

#endif /* _RESET_MANAGER_H_ */
