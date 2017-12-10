/*
 *  Copyright Altera Corporation (C) 2012-2013. All rights reserved
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms and conditions of the GNU General Public License,
 *  version 2, as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/reset_manager.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Print Board information
 */
int checkboard(void)
{
#ifdef CONFIG_SOCFPGA_VIRTUAL_TARGET
	puts("BOARD : Altera VTDEV5XS1 Virtual Board\n");
#else
	puts("BOARD : Fluke Nighthawk A9 Board Rev002\n");
#endif
	return 0;
}

#ifndef CONFIG_BOARD_LATE_INIT
#error "CONFIG_BOARD_LATE_INIT must be set for this board to deassert peripheral dma request resets"
#endif
#define RSTMGR_DMAIF0 RSTMGR_DEFINE(2, 0)
#define RSTMGR_DMAIF1 RSTMGR_DEFINE(2, 1)
#define RSTMGR_DMAIF2 RSTMGR_DEFINE(2, 2)
#define RSTMGR_DMAIF3 RSTMGR_DEFINE(2, 3)
#define RSTMGR_DMAIF4 RSTMGR_DEFINE(2, 4)
#define RSTMGR_DMAIF5 RSTMGR_DEFINE(2, 5)
#define RSTMGR_DMAIF6 RSTMGR_DEFINE(2, 6)
#define RSTMGR_DMAIF7 RSTMGR_DEFINE(2, 7)
int board_late_init()
{
	socfpga_per_reset(RSTMGR_DMAIF0, 0);	
}

