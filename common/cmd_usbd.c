/*
 * common/cmd_usbd.c
 *
 *  $Id: cmd_usbd.c,v 1.2 2009/01/28 00:11:42 dark0351 Exp $
 *
 * (C) Copyright 2007
 * Byungjae Lee, Samsung Erectronics, bjlee@samsung.com.
 *	- support for S3C2412, S3C2443 and S3C6400
 *
 * (C) Copyright SAMSUNG Electronics
 *      SW.LEE  <hitchcar@samsung.com>
 *      - add USB device fo S3C2440A, S3C24A0A
 *
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

/*
 * Memory Functions
 *
 * Copied from FADS ROM, Dan Malek (dmalek@jlc.net)
 */

#include <common.h>
#include <command.h>

#ifdef CONFIG_S3C_USBD

#include <asm/arch/cpu.h>
#include <asm/arch/usb2.h>

#if defined(CONFIG_EXYNOS4X12) || defined(CONFIG_ARCH_EXYNOS)
#include "../drivers/usb/gadget/usbd-otg-hs.h"
#else
#error "* CFG_ERROR : you have to setup right Samsung CPU configuration"
#endif

#include <dnw.h>

int s3c_usbctl_init(void);
#undef	CMD_USBD_DEBUG
#ifdef	CMD_USBD_DEBUG
#define	PRINTF(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTF(fmt,args...)
#endif

extern int g_dnw_version;
extern unsigned int exynos_boot_da;
static const char pszMe[] = "usbd: ";

#ifdef CONFIG_DNW_VERSION
int usdbd_dnw_start_usb(void){
	s3c_usbctl_init();

	if (!s3c_usb_wait_cable_insert()) {
		printf("%s, usb conneciton ok\n", __func__);
	}
	mdelay(10);
	printf("[dnw] after delay \n");

	/* when operation is done, usbd must be stopped */
	s3c_usb_stop();
	mdelay(500); // for slow computer
}

int usbd_dnw_v10( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int ret = 1;
	uint64_t timeout_endtime = 0;
	uint64_t timeout_ticks = 0;
	int check_timeout = 0;
	int continue_from_disconnect = 0;
	char fboot_cmd[20];

	printf("[dnw] Starting dnw \n");
	setenv("bootcmd", CONFIG_BOOTCOMMAND);	// for bootcmd recovery

	if ((argc > 1) && (0 == strcmp(argv[1], "v10"))){ // dnw v.1.0
		g_dnw_version = DNW_v10;
	}else if ((argc > 1) && (0 == strcmp(argv[1], "v05"))){ // dnw v.0.5
		sprintf(fboot_cmd, "fastboot dnw");
		g_dnw_version = DNW_v05;
		return run_command(fboot_cmd, 0);
	}


	do{

		/* Initialize the board specific support */
		if (0 == dnw_init())
		{
			int poll_status;

			/* If we got this far, we are a success */
			ret = 0;

			timeout_endtime = get_ticks();
			timeout_endtime += timeout_ticks;

#ifdef CONFIG_USE_LCD
			LCD_turnon();
#endif

			while (1)
			{
				uint64_t current_time = 0;
				poll_status = dnw_poll();

				if (1 == check_timeout)
					current_time = get_ticks();

				if ( (poll_status != DNW_OK) && (ctrlc()))
				{
					printf("DNW ended by user\n");
					continue_from_disconnect = 0;
					break;
				}

				if (serial_tstc()) {
					serial_getc();
					break;
				}
			}
		}
		/* Reset the board specific support */
		dnw_shutdown();

		/* restart the loop if a disconnect was detected */
	} while (continue_from_disconnect);

}
#endif /* CONFIG_DNW_VERSION */
int do_usbd_dnw ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#ifdef CONFIG_DNW_VERSION
	if  (((argc > 1) && (0 == strcmp(argv[1], "v10")) ) // dnw v.1.0
		 || ((argc > 1) && (0 == strcmp(argv[1], "v05"))) ){ // dnw v.0.5
		return usbd_dnw_v10(cmdtp, flag, argc, argv);
	}
#endif /* CONFIG_DNW_VERSION */

	if (argv[0][0] == 'u') {
		DNW = 0;
	}
	else {
		DNW = 1;
		s3c_got_header = 0;
	}

	switch (argc) {
	case 1 :
		s3c_usbd_dn_addr = USBD_DOWN_ADDR;	/* Default Address */
		break;
	case 2 :
		s3c_usbd_dn_addr = simple_strtoul(argv[1], NULL, 16);
		break;
	default:
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	s3c_receive_done = 0;

	s3c_usbctl_init();
	s3c_usbc_activate();

	PRINTF("Download address 0x%08x\n", s3c_usbd_dn_addr);
	printf("Now, Waiting for DNW to transmit data\n");

	while (1) {
		if (S3C_USBD_DETECT_IRQ()) {
			s3c_udc_int_hndlr();
			S3C_USBD_CLEAR_IRQ();
		}

		if (s3c_receive_done)
			break;

		if (serial_tstc()) {
			serial_getc();
			break;
		}
	}

	/* when operation is done, usbd must be stopped */
	s3c_usb_stop();

	return 0;
}

#if 0 /* ud command not support yet */
U_BOOT_CMD(
	ud, 3, 0, do_usbd_dnw,
	"ud      - initialize USB device and ready to receive for LINUX server (specific)\n",
	"[download address]\n"
);
#endif

U_BOOT_CMD(
	dnw, 3, 0, do_usbd_dnw,
	"dnw     - initialize USB device and ready to receive for Windows server (specific)\n",
	"[download address]\n"
);

#endif	/* CONFIG_S3C_USBD */

