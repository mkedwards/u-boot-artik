/*
 * Copyright (c) 2012 Samsung Electronics.
 * Abhilash Kesavan <a.kesavan@samsung.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/sromc.h>

static void exynos4_uart_config(int peripheral)
{
	struct exynos4_gpio_part1 *gpio1 =
		(struct exynos4_gpio_part1 *) samsung_get_base_gpio_part1();
	struct s5p_gpio_bank *bank;
	int i, start, count;

	switch (peripheral) {
	case PERIPH_ID_UART0:
		bank = &gpio1->a0;
		start = 0;
		count = 4;
		break;
	case PERIPH_ID_UART1:
		bank = &gpio1->a0;
		start = 4;
		count = 4;
		break;
	case PERIPH_ID_UART2:
		bank = &gpio1->a1;
		start = 0;
		count = 4;
		break;
	case PERIPH_ID_UART3:
		bank = &gpio1->a1;
		start = 4;
		count = 2;
		break;
	}
	for (i = start; i < start + count; i++) {
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
	}
}

static void exynos5_uart_config(int peripheral)
{
	struct exynos5420_gpio_part4 *gpio4 =
		(struct exynos5420_gpio_part4 *) samsung_get_base_gpio_part4();
	struct exynos5_gpio_part1 *gpio1 =
		(struct exynos5_gpio_part1 *) samsung_get_base_gpio_part1();
	struct exynos5260_gpio_part3 *gpio3 =
		(struct exynos5260_gpio_part3 *) samsung_get_base_gpio_part3();
	struct s5p_gpio_bank *bank;
	int i, start, count;

	switch (peripheral) {
	case PERIPH_ID_UART0:
		if(s5p_cpu_id == 0x5420 || s5p_cpu_id == 0x5422) {
			bank = &gpio4->a0;
			start = 0;
			count = 4;
		} else {
			bank = &gpio1->a0;
			start = 0;
			count = 4;
		}
		break;
	case PERIPH_ID_UART1:
		if((s5p_cpu_id == 0x5250) && (s5p_get_cpu_rev() >= 0x1)) {
			bank = &gpio1->d0;
			start = 0;
			count = 4;
		} else if(s5p_cpu_id == 0x5260) {
			bank = &gpio1->a1;
			start = 0;
			count = 4;
		} else if(s5p_cpu_id == 0x5420 || s5p_cpu_id == 0x5422) {
			bank = &gpio4->a0;
			start = 4;
			count = 4;
		} else {
			bank = &gpio1->a0;
			start = 4;
			count = 4;
		}
		break;
	case PERIPH_ID_UART2:
		if(s5p_cpu_id == 0x5260) {
			bank = &gpio1->a1;
			start = 4;
			count = 2;
		} else if(s5p_cpu_id == 0x5420 || s5p_cpu_id == 0x5422) {
			bank = &gpio4->a1;
			start = 0;
			count = 4;
		} else {
			bank = &gpio1->a1;
			start = 0;
			count = 4;
		}
		break;
	case PERIPH_ID_UART3:
		if(s5p_cpu_id == 0x5260) {
			bank = &gpio3->z1;
			start = 0;
			count = 4;
		} else if(s5p_cpu_id == 0x5420 || s5p_cpu_id == 0x5422) {
			bank = &gpio4->a1;
			start = 4;
			count = 2;
		}
		break;
	}
	for (i = start; i < start + count; i++) {
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
	}
}

static void exynos5430_uart_config(int peripheral)
{
	struct exynos5430_gpio_peric *gpio_peric =
			(struct exynos5430_gpio_peric *)
			    exynos5430_get_base_gpio_peric();
	struct s5p_gpio_bank *bank;
	int i, start, count;

	switch (peripheral) {
	case PERIPH_ID_UART0:
		bank = &gpio_peric->d0;
		start = 0;
		count = 4;
		break;
	case PERIPH_ID_UART1:
		bank = &gpio_peric->d1;
		start = 0;
		count = 4;
		break;
	case PERIPH_ID_UART2:
		bank = &gpio_peric->d1;
		start = 4;
		count = 2;
		break;
	}
	for (i = start; i < start + count; i++) {
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
	}
}

static int exynos4_mmc_config(int peripheral, int flags)
{
	struct exynos4_gpio_part2 *gpio2 =
		(struct exynos4_gpio_part2 *) samsung_get_base_gpio_part2();
	struct s5p_gpio_bank *bank, *bank_ext;
	int i, bank_ext_bit;

	switch (peripheral) {
	case PERIPH_ID_SDMMC0:
		bank = &gpio2->k0;
		bank_ext = &gpio2->l0;
		bank_ext_bit = 0;
		break;
	case PERIPH_ID_SDMMC1:
		bank = &gpio2->k1;
		bank_ext = NULL;
		break;
	case PERIPH_ID_SDMMC2:
		bank = &gpio2->k2;
		bank_ext = &gpio2->k3;
		bank_ext_bit = 3;
		break;
	case PERIPH_ID_SDMMC3:
		bank = &gpio2->k3;
		bank_ext = NULL;
		break;
	}
	if ((flags & PINMUX_FLAG_8BIT_MODE) && !bank_ext) {
		debug("SDMMC device %d does not support 8bit mode",
				peripheral);
		return -1;
	}
	if (flags & PINMUX_FLAG_8BIT_MODE) {
		for (i = bank_ext_bit; i <= (bank_ext_bit + 3); i++) {
			if (peripheral == PERIPH_ID_SDMMC0)
				s5p_gpio_cfg_pin(bank_ext, i, GPIO_FUNC(0x2));
			else
				s5p_gpio_cfg_pin(bank_ext, i, GPIO_FUNC(0x3));

			s5p_gpio_set_pull(bank_ext, i, GPIO_PULL_UP);
			s5p_gpio_set_drv(bank_ext, i, GPIO_DRV_4X);
		}
	}
	for (i = 0; i < 2; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	for (i = 2; i <= 6; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_UP);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	return 0;
}

static int exynos5_mmc_config(int peripheral, int flags)
{
	struct exynos5_gpio_part1 *gpio1 =
		(struct exynos5_gpio_part1 *) samsung_get_base_gpio_part1();
	struct s5p_gpio_bank *bank, *bank_ext;
	int i, bank_ext_bit;

	switch (peripheral) {
	case PERIPH_ID_SDMMC0:
		bank = &gpio1->c0;
		bank_ext = &gpio1->c1;
		bank_ext_bit = 0;
		break;
	case PERIPH_ID_SDMMC1:
		bank = &gpio1->c2;
		bank_ext = NULL;
		break;
	case PERIPH_ID_SDMMC2:
		bank = &gpio1->c3;
		bank_ext = &gpio1->c4;
		bank_ext_bit = 3;
		break;
	case PERIPH_ID_SDMMC3:
		bank = &gpio1->c4;
		bank_ext = NULL;
		break;
	}
	if ((flags & PINMUX_FLAG_8BIT_MODE) && !bank_ext) {
		debug("SDMMC device %d does not support 8bit mode",
				peripheral);
		return -1;
	}
	if (flags & PINMUX_FLAG_8BIT_MODE) {
		for (i = bank_ext_bit; i <= (bank_ext_bit + 3); i++) {
			if (peripheral == PERIPH_ID_SDMMC0)
				s5p_gpio_cfg_pin(bank_ext, i, GPIO_FUNC(0x2));
			else
				s5p_gpio_cfg_pin(bank_ext, i, GPIO_FUNC(0x3));

			s5p_gpio_set_pull(bank_ext, i, GPIO_PULL_UP);
			s5p_gpio_set_drv(bank_ext, i, GPIO_DRV_4X);
		}
	}
	for (i = 0; i < 2; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	for (i = 2; i <= 6; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_UP);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	return 0;
}

static int exynos5420_mmc_config(int peripheral, int flags)
{
	struct exynos5420_gpio_part2 *gpio2 =
		(struct exynos5420_gpio_part2 *) samsung_get_base_gpio_part2();
	struct s5p_gpio_bank *bank, *bank_data, *bank_ext;
	int i, bank_bit, bank_data_bit, bank_ext_bit;

	switch (peripheral) {
	case PERIPH_ID_SDMMC0:
		bank = &gpio2->c0;
		bank_data = &gpio2->c0;
		bank_ext = &gpio2->c3;
		bank_bit = 0;
		bank_data_bit = 3;
		bank_ext_bit = 0;
		break;
	case PERIPH_ID_SDMMC1:
		bank = &gpio2->c1;
		bank_data = &gpio2->c1;
		bank_ext = &gpio2->d1;
		bank_bit = 0;
		bank_data_bit = 3;
		bank_ext_bit = 4;
		break;
	case PERIPH_ID_SDMMC2:
		bank = &gpio2->c2;
		bank_data = &gpio2->c2;
		bank_ext = NULL;
		bank_bit = 0;
		bank_data_bit = 3;
		bank_ext_bit = 0;
		break;
	default:
		debug("%s: invalid peripheral %d", __func__, peripheral);
		return -1;
	}
	if ((flags & PINMUX_FLAG_8BIT_MODE) && !bank_ext) {
		debug("SDMMC device %d does not support 8bit mode",
				peripheral);
		return -1;
	}

	s5p_gpio_cfg_pin(bank, 2, GPIO_FUNC(0x1));
	s5p_gpio_set_pull(bank, 2, GPIO_PULL_UP);
	s5p_gpio_set_drv(bank, 2, GPIO_DRV_4X);
	s5p_gpio_set_value(bank, 2, 1);

	for (i = bank_bit; i < bank_bit + 2; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	for (i = bank_data_bit; i < bank_data_bit + 4; i++) {
		s5p_gpio_cfg_pin(bank_data, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank_data, i, GPIO_PULL_UP);
		s5p_gpio_set_drv(bank_data, i, GPIO_DRV_4X);
	}
	if (flags & PINMUX_FLAG_8BIT_MODE) {
		for (i = bank_ext_bit; i < bank_ext_bit + 4; i++) {
			s5p_gpio_cfg_pin(bank_ext, i, GPIO_FUNC(0x2));
			s5p_gpio_set_pull(bank_ext, i, GPIO_PULL_UP);
			s5p_gpio_set_drv(bank_ext, i, GPIO_DRV_4X);
		}
	}
	return 0;
}

static int exynos5260_mmc_config(int peripheral, int flags)
{
	struct exynos5260_gpio_part2 *gpio2 =
		(struct exynos5260_gpio_part2 *) samsung_get_base_gpio_part2();
	struct s5p_gpio_bank *bank, *bank_ext;
	int i, bank_ext_bit;

	switch (peripheral) {
	case PERIPH_ID_SDMMC0:
		bank = &gpio2->c0;
		bank_ext = &gpio2->c3;
		bank_ext_bit = 0;
		break;
	case PERIPH_ID_SDMMC1:
		bank = &gpio2->c1;
		bank_ext = &gpio2->c4;
		bank_ext_bit = 0;
		break;
	case PERIPH_ID_SDMMC2:
		bank = &gpio2->c2;
		bank_ext = NULL;
		break;
	default:
		debug("%s: invalid peripheral %d", __func__, peripheral);
		return -1;
	}
	if ((flags & PINMUX_FLAG_8BIT_MODE) && !bank_ext) {
		debug("SDMMC device %d does not support 8bit mode",
				peripheral);
		return -1;
	}
	if (flags & PINMUX_FLAG_8BIT_MODE) {
		for (i = bank_ext_bit; i <= (bank_ext_bit + 3); i++) {
			s5p_gpio_cfg_pin(bank_ext, i, GPIO_FUNC(0x2));
			s5p_gpio_set_pull(bank_ext, i, GPIO_PULL_UP);
			s5p_gpio_set_drv(bank_ext, i, GPIO_DRV_4X);
		}
	}
	for (i = 0; i < 2; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	for (i = 2; i <= 6; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_UP);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}

	return 0;
}

static int exynos5430_mmc_config(int peripheral, int flags)
{
	struct exynos5430_gpio_fsys *gpio_fsys =
			(struct exynos5430_gpio_fsys *)
			    exynos5430_get_base_gpio_fsys();
	struct s5p_gpio_bank *bank, *bank_data, *bank_ext_data;
	int i, bank_bit, bank_bit_cnt;
	int bank_data_bit, bank_ext_data_bit;

	switch (peripheral) {
	case PERIPH_ID_SDMMC0:
		bank = &gpio_fsys->r0;
		bank_data = &gpio_fsys->r1;
		bank_ext_data = &gpio_fsys->r1;
		bank_bit = 0;
		bank_bit_cnt = 4;
		bank_data_bit = 0;
		bank_ext_data_bit = 4;
		break;
	case PERIPH_ID_SDMMC1:
		bank = &gpio_fsys->r2;
		bank_data = &gpio_fsys->r3;
		bank_ext_data = &gpio_fsys->r3;
		bank_bit = 0;
		bank_bit_cnt = 2;
		bank_data_bit = 0;
		bank_ext_data_bit = 4;
		break;
	case PERIPH_ID_SDMMC2:
		bank = &gpio_fsys->r4;
		bank_data = &gpio_fsys->r4;
		bank_ext_data = NULL;
		bank_bit = 0;
		bank_bit_cnt = 4;
		bank_data_bit = 4;
		break;
	default:
		debug("%s: invalid peripheral %d", __func__, peripheral);
		return -1;
	}
	if ((flags & PINMUX_FLAG_8BIT_MODE) && !bank_ext_data) {
		debug("SDMMC device %d does not support 8bit mode",
				peripheral);
		return -1;
	}
	for (i = bank_bit; i < bank_bit_cnt; i++) {
		s5p_gpio_cfg_pin(bank, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_set_drv(bank, i, GPIO_DRV_4X);
	}
	for (i = bank_data_bit; i < (bank_data_bit + 4); i++) {
		s5p_gpio_cfg_pin(bank_data, i, GPIO_FUNC(0x2));
		s5p_gpio_set_pull(bank_data, i, GPIO_PULL_UP);
		s5p_gpio_set_drv(bank_data, i, GPIO_DRV_4X);
	}
	if (flags & PINMUX_FLAG_8BIT_MODE) {
		for (i = bank_ext_data_bit; i < (bank_ext_data_bit + 4); i++) {
			s5p_gpio_cfg_pin(bank_ext_data, i, GPIO_FUNC(0x2));
			s5p_gpio_set_pull(bank_ext_data, i, GPIO_PULL_UP);
			s5p_gpio_set_drv(bank_ext_data, i, GPIO_DRV_4X);
		}
	}
	return 0;
}

static void exynos5_sromc_config(int flags)
{
	struct exynos5_gpio_part1 *gpio1 =
		(struct exynos5_gpio_part1 *) samsung_get_base_gpio_part1();
	int i;

	/*
	 * SROM:CS1 and EBI
	 *
	 * GPY0[0]	SROM_CSn[0]
	 * GPY0[1]	SROM_CSn[1](2)
	 * GPY0[2]	SROM_CSn[2]
	 * GPY0[3]	SROM_CSn[3]
	 * GPY0[4]	EBI_OEn(2)
	 * GPY0[5]	EBI_EEn(2)
	 *
	 * GPY1[0]	EBI_BEn[0](2)
	 * GPY1[1]	EBI_BEn[1](2)
	 * GPY1[2]	SROM_WAIT(2)
	 * GPY1[3]	EBI_DATA_RDn(2)
	 */
	s5p_gpio_cfg_pin(&gpio1->y0, (flags & PINMUX_FLAG_BANK),
				GPIO_FUNC(2));
	s5p_gpio_cfg_pin(&gpio1->y0, 4, GPIO_FUNC(2));
	s5p_gpio_cfg_pin(&gpio1->y0, 5, GPIO_FUNC(2));

	for (i = 0; i < 4; i++)
		s5p_gpio_cfg_pin(&gpio1->y1, i, GPIO_FUNC(2));

	/*
	 * EBI: 8 Addrss Lines
	 *
	 * GPY3[0]	EBI_ADDR[0](2)
	 * GPY3[1]	EBI_ADDR[1](2)
	 * GPY3[2]	EBI_ADDR[2](2)
	 * GPY3[3]	EBI_ADDR[3](2)
	 * GPY3[4]	EBI_ADDR[4](2)
	 * GPY3[5]	EBI_ADDR[5](2)
	 * GPY3[6]	EBI_ADDR[6](2)
	 * GPY3[7]	EBI_ADDR[7](2)
	 *
	 * EBI: 16 Data Lines
	 *
	 * GPY5[0]	EBI_DATA[0](2)
	 * GPY5[1]	EBI_DATA[1](2)
	 * GPY5[2]	EBI_DATA[2](2)
	 * GPY5[3]	EBI_DATA[3](2)
	 * GPY5[4]	EBI_DATA[4](2)
	 * GPY5[5]	EBI_DATA[5](2)
	 * GPY5[6]	EBI_DATA[6](2)
	 * GPY5[7]	EBI_DATA[7](2)
	 *
	 * GPY6[0]	EBI_DATA[8](2)
	 * GPY6[1]	EBI_DATA[9](2)
	 * GPY6[2]	EBI_DATA[10](2)
	 * GPY6[3]	EBI_DATA[11](2)
	 * GPY6[4]	EBI_DATA[12](2)
	 * GPY6[5]	EBI_DATA[13](2)
	 * GPY6[6]	EBI_DATA[14](2)
	 * GPY6[7]	EBI_DATA[15](2)
	 */
	for (i = 0; i < 8; i++) {
		s5p_gpio_cfg_pin(&gpio1->y3, i, GPIO_FUNC(2));
		s5p_gpio_set_pull(&gpio1->y3, i, GPIO_PULL_UP);

		s5p_gpio_cfg_pin(&gpio1->y5, i, GPIO_FUNC(2));
		s5p_gpio_set_pull(&gpio1->y5, i, GPIO_PULL_UP);

		s5p_gpio_cfg_pin(&gpio1->y6, i, GPIO_FUNC(2));
		s5p_gpio_set_pull(&gpio1->y6, i, GPIO_PULL_UP);
	}
}

static void exynos5_input_config(int peripheral)
{
	struct exynos5_gpio_part1 *gpio1 =
		(struct exynos5_gpio_part1 *) samsung_get_base_gpio_part1();
	struct s5p_gpio_bank *bank;
	int i, start = 0, count = 0;

	switch (peripheral) {
	case PERIPH_ID_INPUT_X0_0:
		bank = &gpio1->x0;
		start = 0;
		count = 1;
		break;
	case PERIPH_ID_INPUT_X2_0:
		bank = &gpio1->x2;
		start = 0;
		count = 1;
		break;
	}
	for (i = start; i < start + count; i++) {
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_direction_input(bank, i);
	}
}

static void exynos5430_input_config(int peripheral)
{
	struct exynos5430_gpio_alive *gpio_alive =
			(struct exynos5430_gpio_alive *)
			    exynos5430_get_base_gpio_alive();
	struct s5p_gpio_bank *bank;
	int i, start = 0, count = 0;

	switch (peripheral) {
	case PERIPH_ID_INPUT_A0_3:
		bank = &gpio_alive->a0;
		start = 3;
		count = 1;
		break;
	case PERIPH_ID_INPUT_A2_0:
		bank = &gpio_alive->a2;
		start = 0;
		count = 1;
		break;
	case PERIPH_ID_INPUT_A2_1:
		bank = &gpio_alive->a2;
		start = 1;
		count = 1;
		break;
	}
	for (i = start; i < start + count; i++) {
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_direction_input(bank, i);
	}
}

void exynos5_spi_config(int peripheral)
{
	int cfg = 0, pin = 0, i;
	struct s5p_gpio_bank *bank = NULL;
	struct exynos5_gpio_part1 *gpio1 =
		(struct exynos5_gpio_part1 *) samsung_get_base_gpio_part1();
	struct exynos5_gpio_part2 *gpio2 =
		(struct exynos5_gpio_part2 *) samsung_get_base_gpio_part2();

	switch (peripheral) {
	case PERIPH_ID_SPI0:
		bank = &gpio1->a2;
		cfg = GPIO_FUNC(0x2);
		pin = 0;
		break;
	case PERIPH_ID_SPI1:
		bank = &gpio1->a2;
		cfg = GPIO_FUNC(0x2);
		pin = 4;
		break;
	case PERIPH_ID_SPI2:
		bank = &gpio1->b1;
		cfg = GPIO_FUNC(0x5);
		pin = 1;
		break;
	case PERIPH_ID_SPI3:
		bank = &gpio2->f1;
		cfg = GPIO_FUNC(0x2);
		pin = 0;
		break;
	case PERIPH_ID_SPI4:
		for (i = 0; i < 2; i++) {
			s5p_gpio_cfg_pin(&gpio2->f0, i + 2, GPIO_FUNC(0x4));
			s5p_gpio_cfg_pin(&gpio2->e0, i + 4, GPIO_FUNC(0x4));
		}
		break;
	}
	if (peripheral != PERIPH_ID_SPI4) {
		for (i = pin; i < pin + 4; i++)
			s5p_gpio_cfg_pin(bank, i, cfg);
	}
}

static int exynos5_pinmux_config(int peripheral, int flags)
{
	switch (peripheral) {
	case PERIPH_ID_UART0:
	case PERIPH_ID_UART1:
	case PERIPH_ID_UART2:
	case PERIPH_ID_UART3:
		if (s5p_cpu_id == 0x5430) {
			exynos5430_uart_config(peripheral);
		} else {
			exynos5_uart_config(peripheral);
		}
		break;
	case PERIPH_ID_SDMMC0:
	case PERIPH_ID_SDMMC1:
	case PERIPH_ID_SDMMC2:
	case PERIPH_ID_SDMMC3:
		if (s5p_cpu_id == 0x5430) {
			return exynos5430_mmc_config(peripheral, flags);
		} else if (s5p_cpu_id == 0x5260) {
			return exynos5260_mmc_config(peripheral, flags);
		} else if (s5p_cpu_id == 0x5420 || s5p_cpu_id == 0x5422) {
			return exynos5420_mmc_config(peripheral, flags);
		} else {
			return exynos5_mmc_config(peripheral, flags);
		}
	case PERIPH_ID_SROMC:
		if (s5p_cpu_id != 0x5260) {
			exynos5_sromc_config(flags);
		}
		break;
	case PERIPH_ID_INPUT_X0_0:
	case PERIPH_ID_INPUT_X2_0:
	case PERIPH_ID_INPUT_A0_3:
	case PERIPH_ID_INPUT_A2_0:
	case PERIPH_ID_INPUT_A2_1:
		if (s5p_cpu_id == 0x5430) {
			exynos5430_input_config(peripheral);
		} else {
			exynos5_input_config(peripheral);
		}
		break;
	case PERIPH_ID_SPI0:
	case PERIPH_ID_SPI1:
	case PERIPH_ID_SPI2:
	case PERIPH_ID_SPI3:
	case PERIPH_ID_SPI4:
		exynos5_spi_config(peripheral);
		break;
	default:
		debug("%s: invalid peripheral %d", __func__, peripheral);
		return -1;
	}

	return 0;
}

static void exynos4_spi_config(int peripheral)
{
	struct exynos4_gpio_part1 *gpio1 =
		(struct exynos4_gpio_part1 *) samsung_get_base_gpio_part1();
	struct s5p_gpio_bank *bank;
	int start = 0, count = 0;
	int i, cfg;

	switch (peripheral) {
	case PERIPH_ID_SPI0:
		bank = &gpio1->b;
		start = 0;
		count = 4;
		cfg = GPIO_FUNC(0x2);
		break;
	case PERIPH_ID_SPI1:
		bank = &gpio1->b;
		start = 4;
		count = 4;
		cfg = GPIO_FUNC(0x2);
		break;
	default:
		return;
	}

	for (i = start; i < start + count; i++)
		s5p_gpio_cfg_pin(bank, i, cfg);
}

static void exynos4_input_config(int peripheral)
{
	struct exynos4_gpio_part2 *gpio2 =
		(struct exynos4_gpio_part2 *) samsung_get_base_gpio_part2();
	struct s5p_gpio_bank *bank;
	int i, start = 0, count = 0;

	switch (peripheral) {
	case PERIPH_ID_INPUT_X0_0:
		bank = &gpio2->x0;
		start = 0;
		count = 1;
		break;
	}
	for (i = start; i < start + count; i++) {
		s5p_gpio_set_pull(bank, i, GPIO_PULL_NONE);
		s5p_gpio_direction_input(bank, i);
	}
}

static int exynos4_pinmux_config(int peripheral, int flags)
{
	switch (peripheral) {
	case PERIPH_ID_UART0:
	case PERIPH_ID_UART1:
	case PERIPH_ID_UART2:
	case PERIPH_ID_UART3:
		exynos4_uart_config(peripheral);
		break;
	case PERIPH_ID_SDMMC0:
	case PERIPH_ID_SDMMC1:
	case PERIPH_ID_SDMMC2:
	case PERIPH_ID_SDMMC3:
		return exynos4_mmc_config(peripheral, flags);
	case PERIPH_ID_SPI0:
	case PERIPH_ID_SPI1:
		exynos4_spi_config(peripheral);
		break;
	case PERIPH_ID_INPUT_X0_0:
		exynos4_input_config(peripheral);
		break;
	default:
		debug("%s: invalid peripheral %d", __func__, peripheral);
		return -1;
	}

	return 0;
}

int exynos_pinmux_config(int peripheral, int flags)
{
	if (cpu_is_exynos5())
		return exynos5_pinmux_config(peripheral, flags);
	else if (cpu_is_exynos4())
		return exynos4_pinmux_config(peripheral, flags);
	else {
		debug("pinmux functionality not supported\n");
		return -1;
	}
}
