/*CPE9344 board support
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/phy.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "pci.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-nfc.h"
#include "dev-spi.h"
#include "dev-wmac.h"
#include "dev-usb.h"
#include "machtypes.h"

#define CPE9344_GPIO_BTN_RESET          16

#define CPE9344_KEYS_POLL_INTERVAL      20	/* msecs */
#define CPE9344_KEYS_DEBOUNCE_INTERVAL  (3 * CPE9344_KEYS_POLL_INTERVAL)

#define CPE9344_MAC0_OFFSET             0
#define CPE9344_MAC1_OFFSET             6
#define CPE9344_WMAC_CALDATA_OFFSET     0x1000

static const char *cpe9344_part_probes[] = {
	"tp-link",
	NULL,
	};

static struct flash_platform_data cpe9344_flash_data = {
	.part_probes	= cpe9344_part_probes,
};

static struct gpio_keys_button cpe9344_gpio_keys[] __initdata = {
	{
		.desc		= "Reset Button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = CPE9344_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= CPE9344_GPIO_BTN_RESET,
                .active_low     = 1,
	},
};


static void __init cpe9344_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(&cpe9344_flash_data);
	ath79_register_gpio_keys_polled(-1, CPE9344_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(cpe9344_gpio_keys), cpe9344_gpio_keys);

	ath79_register_wmac(art + CPE9344_WMAC_CALDATA_OFFSET, NULL);
        ath79_setup_ar934x_eth_cfg(AR934X_ETH_CFG_SW_ONLY_MODE);
	ath79_register_mdio(1, 0x0);


	/* LAN */
	ath79_init_mac(ath79_eth1_data.mac_addr, art + CPE9344_MAC1_OFFSET, 0);

	/* GMAC1 is connected to the internal switch */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_register_eth(1);

	/* WAN */
	ath79_init_mac(ath79_eth0_data.mac_addr, art + CPE9344_MAC0_OFFSET, 0);

	/* GMAC0 is connected to the PHY4 of the internal switch */
	ath79_switch_data.phy4_mii_en = 1;
	ath79_switch_data.phy_poll_mask = BIT(4);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio1_device.dev;
	ath79_register_eth(0);


	ath79_register_usb();
	ath79_register_pci();
	ath79_register_nfc();
}

MIPS_MACHINE(ATH79_MACH_GS_CPE9344, "CPE9344", "CPE9344 board", cpe9344_setup)

