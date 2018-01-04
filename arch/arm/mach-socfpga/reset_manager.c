/*
 *  Copyright (C) 2013 Altera Corporation <www.altera.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#include <common.h>
#include <asm/io.h>
#include <asm/arch/fpga_manager.h>
#include <asm/arch/reset_manager.h>
#include <asm/arch/system_manager.h>
#include <qts/reset_config.h>

DECLARE_GLOBAL_DATA_PTR;

static const struct socfpga_reset_manager *reset_manager_base =
		(void *)SOCFPGA_RSTMGR_ADDRESS;
static struct socfpga_system_manager *sysmgr_regs =
	(struct socfpga_system_manager *)SOCFPGA_SYSMGR_ADDRESS;

/* Assert or de-assert SoCFPGA reset manager reset. */
void socfpga_per_reset(u32 reset, int set)
{
	const void *reg;

	if (RSTMGR_BANK(reset) == 0)
		reg = &reset_manager_base->mpu_mod_reset;
	else if (RSTMGR_BANK(reset) == 1)
		reg = &reset_manager_base->per_mod_reset;
	else if (RSTMGR_BANK(reset) == 2)
		reg = &reset_manager_base->per2_mod_reset;
	else if (RSTMGR_BANK(reset) == 3)
		reg = &reset_manager_base->brg_mod_reset;
	else if (RSTMGR_BANK(reset) == 4)
		reg = &reset_manager_base->misc_mod_reset;
	else	/* Invalid reset register, do nothing */
		return;

	if (set)
		setbits_le32(reg, 1 << RSTMGR_RESET(reset));
	else
		clrbits_le32(reg, 1 << RSTMGR_RESET(reset));
}

/*
 * Assert reset on every peripheral but L4WD0.
 * Watchdog must be kept intact to prevent glitches
 * and/or hangs.
 */
void socfpga_per_reset_all(void)
{
	const u32 l4wd0 = 1 << RSTMGR_RESET(SOCFPGA_RESET(L4WD0));

	writel(~l4wd0, &reset_manager_base->per_mod_reset);
	writel(0xffffffff, &reset_manager_base->per2_mod_reset);
}

/*
 * Write the reset manager register to cause reset
 */
void reset_cpu(ulong addr)
{
	/* request a warm reset */
	writel((1 << RSTMGR_CTRL_SWWARMRSTREQ_LSB),
		&reset_manager_base->ctrl);
	/*
	 * infinite loop here as watchdog will trigger and reset
	 * the processor
	 */
	while (1)
		;
}

/*
 * Release peripherals from reset based on handoff
 */
void reset_deassert_peripherals_handoff(void)
{
	/* permodrst */
	socfpga_per_reset(RSTMGR_EMAC0, CONFIG_HPS_RESET_ASSERT_EMAC0);
	socfpga_per_reset(RSTMGR_EMAC1, CONFIG_HPS_RESET_ASSERT_EMAC1);
	socfpga_per_reset(RSTMGR_USB0, CONFIG_HPS_RESET_ASSERT_USB0);
	socfpga_per_reset(RSTMGR_USB1, CONFIG_HPS_RESET_ASSERT_USB1);
	socfpga_per_reset(RSTMGR_NAND, CONFIG_HPS_RESET_ASSERT_NAND);
	socfpga_per_reset(RSTMGR_QSPI, CONFIG_HPS_RESET_ASSERT_QSPI);
	socfpga_per_reset(RSTMGR_L4WD0, 0);
	socfpga_per_reset(RSTMGR_L4WD1, CONFIG_HPS_RESET_ASSERT_L4WD1);
	socfpga_per_reset(RSTMGR_QSPI, CONFIG_HPS_RESET_ASSERT_QSPI);
	socfpga_per_reset(RSTMGR_OSC1TIMER0, 0);
	socfpga_per_reset(RSTMGR_OSC1TIMER1, CONFIG_HPS_RESET_ASSERT_OSC1TIMER1);
	socfpga_per_reset(RSTMGR_SPTIMER0, CONFIG_HPS_RESET_ASSERT_SPTIMER0);
	socfpga_per_reset(RSTMGR_SPTIMER1, CONFIG_HPS_RESET_ASSERT_SPTIMER1);
	socfpga_per_reset(RSTMGR_I2C0, CONFIG_HPS_RESET_ASSERT_I2C0);
	socfpga_per_reset(RSTMGR_I2C1, CONFIG_HPS_RESET_ASSERT_I2C1);
	socfpga_per_reset(RSTMGR_I2C2, CONFIG_HPS_RESET_ASSERT_I2C2);
	socfpga_per_reset(RSTMGR_I2C3, CONFIG_HPS_RESET_ASSERT_I2C3);
	socfpga_per_reset(RSTMGR_UART0, CONFIG_HPS_RESET_ASSERT_UART0);
	socfpga_per_reset(RSTMGR_UART1, CONFIG_HPS_RESET_ASSERT_UART1);
	socfpga_per_reset(RSTMGR_SPIM0, CONFIG_HPS_RESET_ASSERT_SPIM0);
	socfpga_per_reset(RSTMGR_SPIM1, CONFIG_HPS_RESET_ASSERT_SPIM1);
	socfpga_per_reset(RSTMGR_SPIS0, CONFIG_HPS_RESET_ASSERT_SPIS0);
	socfpga_per_reset(RSTMGR_SPIS1, CONFIG_HPS_RESET_ASSERT_SPIS1);
	socfpga_per_reset(RSTMGR_SDMMC, CONFIG_HPS_RESET_ASSERT_SDMMC);
	socfpga_per_reset(RSTMGR_CAN0, CONFIG_HPS_RESET_ASSERT_CAN0);
	socfpga_per_reset(RSTMGR_CAN1, CONFIG_HPS_RESET_ASSERT_CAN1);
	socfpga_per_reset(RSTMGR_GPIO0, CONFIG_HPS_RESET_ASSERT_GPIO0);
	socfpga_per_reset(RSTMGR_GPIO1, CONFIG_HPS_RESET_ASSERT_GPIO1);
	socfpga_per_reset(RSTMGR_GPIO2, CONFIG_HPS_RESET_ASSERT_GPIO2);
	socfpga_per_reset(RSTMGR_DMA, CONFIG_HPS_RESET_ASSERT_DMA);
	socfpga_per_reset(RSTMGR_SDR, CONFIG_HPS_RESET_ASSERT_SDR);

	/* warm reset handshake support */
#if (CONFIG_HPS_RESET_WARMRST_HANDSHAKE_FPGA == 1)
	setbits_le32(&reset_manager_base->ctrl, RSTMGR_CTRL_FPGAHSEN_MASK);
#else
	clrbits_le32(&reset_manager_base->ctrl, RSTMGR_CTRL_FPGAHSEN_MASK);
#endif

#if (CONFIG_HPS_RESET_WARMRST_HANDSHAKE_ETR == 1)
	setbits_le32(&reset_manager_base->ctrl, RSTMGR_CTRL_ETRSTALLEN_MASK);
#else
	clrbits_le32(&reset_manager_base->ctrl, RSTMGR_CTRL_ETRSTALLEN_MASK);
#endif

#if (CONFIG_HPS_RESET_WARMRST_HANDSHAKE_SDRAM == 1)
	setbits_le32(&reset_manager_base->ctrl, RSTMGR_CTRL_SDRSELFREFEN_MASK);
#else
	clrbits_le32(&reset_manager_base->ctrl, RSTMGR_CTRL_SDRSELFREFEN_MASK);
#endif
}

void reset_dma_peripheral_requests(int assert)
{
	socfpga_per_reset(RSTMGR_DMAIF(0), CONFIG_HPS_RESET_ASSERT_FPGA_DMA0 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(1), CONFIG_HPS_RESET_ASSERT_FPGA_DMA1 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(2), CONFIG_HPS_RESET_ASSERT_FPGA_DMA2 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(3), CONFIG_HPS_RESET_ASSERT_FPGA_DMA3 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(4), CONFIG_HPS_RESET_ASSERT_FPGA_DMA4 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(5), CONFIG_HPS_RESET_ASSERT_FPGA_DMA5 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(6), CONFIG_HPS_RESET_ASSERT_FPGA_DMA6 || assert);
	socfpga_per_reset(RSTMGR_DMAIF(7), CONFIG_HPS_RESET_ASSERT_FPGA_DMA7 || assert);
}


#if defined(CONFIG_SOCFPGA_VIRTUAL_TARGET)
void socfpga_bridges_reset(int enable)
{
	/* For SoCFPGA-VT, this is NOP. */
}
#else

#define L3REGS_REMAP_LWHPS2FPGA_MASK	0x10
#define L3REGS_REMAP_HPS2FPGA_MASK	0x08
#define L3REGS_REMAP_OCRAM_MASK		0x01

void socfpga_bridges_reset(int enable)
{
	const uint32_t l3mask = L3REGS_REMAP_LWHPS2FPGA_MASK |
				L3REGS_REMAP_HPS2FPGA_MASK |
				L3REGS_REMAP_OCRAM_MASK;

	if (enable) {
		/* brdmodrst */
		writel(0xffffffff, &reset_manager_base->brg_mod_reset);
	} else {
		writel(0, &sysmgr_regs->iswgrp_handoff[0]);
		writel(l3mask, &sysmgr_regs->iswgrp_handoff[1]);

		/* Check signal from FPGA. */
		if (!fpgamgr_test_fpga_ready()) {
			/* FPGA not ready, do nothing. */
			printf("%s: FPGA not ready, aborting.\n", __func__);
			return;
		}

		/* brdmodrst */
		writel(0, &reset_manager_base->brg_mod_reset);

		/* Remap the bridges into memory map */
		writel(l3mask, SOCFPGA_L3REGS_ADDRESS);
	}
}
#endif

int resetmgr_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long bank;
	unsigned long offset;
	unsigned long assert;
	
	if (argc != 4)
		return CMD_RET_USAGE;

	bank = simple_strtoul(argv[1], NULL, 0);
	offset = simple_strtoul(argv[2], NULL, 0);
	assert = simple_strtoul(argv[3], NULL, 0);
	socfpga_per_reset(RSTMGR_DEFINE(bank, offset), assert);
	return 0;
}

U_BOOT_CMD(
	resetmgr, 4, 1, resetmgr_cmd,
	"SoCFPGA HPS reset manager control",
	"resetmgr bank offset assert\n"
	"    bank - Bank of reset to assert/deassert.\n"
	"        0 ... mpumodrst\n"
	"        1 ... permodrst\n"
	"        2 ... per2modrst\n"
	"        3 ... brgmodrst\n"
	"        4 ... miscmodrst\n"
	"    offset - Offset of reset to assert/deassert.\n"
	"    assert - 1 to assert reset, 0 to deassert.\n"
	""
);
