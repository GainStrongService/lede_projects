/*
 * MiniBox V3.2 board support
 *
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
 * Copyright (c) 2012 Gabor Juhos <juhosg@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "pci.h"

#define MINIBOX_V3_2_GPIO_LED_STATUS                14
#define MINIBOX_V3_2_GPIO_BTN_RST                   17
#define MINIBOX_V3_2_KEYS_POLL_INTERVAL             20	/* msecs */
#define MINIBOX_V3_2_KEYS_DEBOUNCE_INTERVAL         (3 * MINIBOX_V3_2_KEYS_POLL_INTERVAL)

#define MINIBOX_V3_2_MAC0_OFFSET                    0
#define MINIBOX_V3_2_MAC1_OFFSET                    6
#define MINIBOX_V3_2_WMAC_CALDATA_OFFSET            0x1000

static const char *minibox_v3_2_part_probes[] = {
        "tp-link",
        NULL,
};

static struct flash_platform_data minibox_v3_2_flash_data = {
        .part_probes    = minibox_v3_2_part_probes,
};

static struct gpio_led minibox_v3_2_leds_gpio[] __initdata = {
	{
		.name		= "minibox_v3_2:status",
		.gpio		= MINIBOX_V3_2_GPIO_LED_STATUS,
		.active_low	= 1,
	},
};

static struct gpio_keys_button minibox_v3_2_gpio_keys[] __initdata = {
	{
		.desc		= "reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MINIBOX_V3_2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= MINIBOX_V3_2_GPIO_BTN_RST,
		.active_low	= 1,
	},
};

static void __init minibox_v3_2_gpio_led_setup(void)
{
	ath79_register_leds_gpio(-1, ARRAY_SIZE(minibox_v3_2_leds_gpio),
			minibox_v3_2_leds_gpio);
	ath79_register_gpio_keys_polled(-1, MINIBOX_V3_2_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(minibox_v3_2_gpio_keys),
			minibox_v3_2_gpio_keys);
}

static void __init minibox_v3_2_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(&minibox_v3_2_flash_data);
	minibox_v3_2_gpio_led_setup();

        ath79_register_usb();
        ath79_register_pci();

	ath79_register_wmac(art + MINIBOX_V3_2_WMAC_CALDATA_OFFSET, NULL);
	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + MINIBOX_V3_2_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + MINIBOX_V3_2_MAC1_OFFSET, 0);

	/* WAN port */
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.speed = SPEED_100;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_register_eth(0);

	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;
	ath79_switch_data.phy_poll_mask |= BIT(4);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_MINIBOX_V3_2, "MINIBOX_V3_2", "MiniBox V3.2 board", minibox_v3_2_setup);
