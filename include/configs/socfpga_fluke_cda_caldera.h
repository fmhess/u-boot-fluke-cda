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

#ifndef __CONFIG_SOCFPGA_FLUKE_CDA_CALDERA_H__
#define __CONFIG_SOCFPGA_FLUKE_CDA_CALDERA_H__

#include <asm/arch/base_addr_ac5.h>
/*
 * Console setup
 */
/* Monitor Command Prompt */
/*
#define CONFIG_SYS_PROMPT		"Fluke Caldera A9 Rev001 # " 
*/

/* Define machine type for Cyclone 5 */
#define CONFIG_MACH_TYPE 4251

/*
 *Define this variable to reservvee a region of protected ram for the frame buffer rotaator 
 */
#define CONFIG_PRAM 8192

/* U-Boot Commands */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_DOS_PARTITION
#define CONFIG_FAT_WRITE
/* Do NOT set CONFIG_HW_WATCHDOG, it will result in the hardware reset for the watchdog 
 * never being asserted (see the code for arch_cpu_init() in arch/arm/mach-socfpga/misc.c
 * if you don't believe me).  This results in odd behavior, like Linux spontaneously 
 * rebooting a while after being shut down.
 */
/* #define CONFIG_HW_WATCHDOG 
 */

/* Memory configurations */
#define PHYS_SDRAM_1_SIZE		0x40000000

/* Booting Linux */
#define CONFIG_BOOTFILE         "zImage"
#define CONFIG_BOOTARGS         "console=ttyS0," __stringify(CONFIG_BAUDRATE)
#ifdef CONFIG_SOCFPGA_VIRTUAL_TARGET
#define CONFIG_BOOTCOMMAND      "run ramboot"
#else

#define CONFIG_BOOTCOMMAND      "run qspifpga; bridge enable; run rotate_init; run qspiload; run qspiboot"

#endif
#define CONFIG_LOADADDR         0x1000000
#define CONFIG_SYS_LOAD_ADDR    CONFIG_LOADADDR

/* Ethernet on SoC (EMAC) */
#if defined(CONFIG_CMD_NET)
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9021
#endif

/* enable date env command */
#define CONFIG_CMD_DATE
#define CONFIG_RTC_PCF8563
#define CONFIG_SYS_RTC_BUS_NUM 1
#define CONFIG_SYS_I2C_RTC_ADDR 0X51

/* environment setting for MMC */
#define CONFIG_ENV_IS_IN_SPI_FLASH

/* Extra Environment */
#define CONFIG_EXTRA_ENV_SETTINGS \
        "verify=n\0" \
        "loadaddr=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
        "ramboot=setenv bootargs " CONFIG_BOOTARGS ";" \
                "bootm ${loadaddr} - ${fdtaddr}\0" \
        "bootimage=zImage\0" \
        "bootimagesize=0x600000\0" \
        "fdtaddr=100\0" \
        "fdtimage=Caldera_soc.dtb\0" \
        "fdtimagesize=0xFFFF\0" \
        "bootm ${loadaddr} - ${fdtaddr}\0" \
        "mmcroot=/dev/mmcblk0p2\0" \
        "mmcboot=setenv bootargs " CONFIG_BOOTARGS \
                " root=${mmcroot} rw rootwait;" \
                "bootz ${loadaddr} - ${fdtaddr}\0" \
        "mmcload=mmc rescan;" \
                "load mmc 0:1 ${loadaddr} ${bootimage};" \
                "load mmc 0:1 ${fdtaddr} ${fdtimage}\0" \
        "qspiloadcs=0\0" \
        "qspibootimageaddr=0x200000\0" \
        "qspifdtaddr=0x110000\0" \
        "qspiload=sf probe ${qspiloadcs};" \
                "sf read ${loadaddr} ${qspibootimageaddr} ${bootimagesize};" \
                "sf read ${fdtaddr} ${qspifdtaddr} ${fdtimagesize};\0" \
        "qspirootpartition=ubi.mtd=4\0" \
        "qspiroot=ubi0:root-fs\0" \
        "qspirootfstype=ubifs\0" \
        "qspifpgaaddr=0x800000\0" \
        "qspifpga=sf probe ${qspiloadcs}; sf read ${fpgadata} ${qspifpgaaddr} ${fpgadatasize}; fpga load ${fpga} ${fpgadata} ${fpgadatasize}\0" \
        "qspiboot=setenv bootargs " CONFIG_BOOTARGS \
                " ${qspirootpartition} root=${qspiroot} rw rootfstype=${qspirootfstype} vt.global_cursor_default=0 vt.cur_default=1 coherent_pool=256K mem=${mem} isolcpus=1; "\
                "bootz ${loadaddr} - ${fdtaddr}\0" \
        "fpga=0\0" \
        "fpgadata=0x2000000\0" \
        "fpgadatasize=0x600000\0" \
        "rotate_ctrl=0xff24A700\0" \
        "rotate_init=mw ${rotate_ctrl} 0x0; mw ${rotate_ctrl} 0x5\0" \
        "ubiload=ubi part UBI && ubifsmount ubi0 && " \
                "ubifsload ${loadaddr} /boot/${bootimage} && " \
                "ubifsload ${fdtaddr} /boot/${fdtimage}\0"

#include "socfpga_common.h"

#endif  /* __CONFIG_SOCFPGA_FLUKE_CDA_CALDERA_H__ */
