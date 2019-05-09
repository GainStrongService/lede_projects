/*
* Atheros OOLITEBOX reference board support
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
#include "eeprom.h"
#define OOLITEBOX_GPIO_BTN_RST                   17
#define OOLITEBOX_KEYS_POLL_INTERVAL             20 /* msecs */
#define OOLITEBOX_MAC0_OFFSET                    0
#define OOLITEBOX_MAC1_OFFSET                    6
#define OOLITEBOX_WMAC_CALDATA_OFFSET            0x1000
#define OOLITEBOX_KEYS_DEBOUNCE_INTERVAL         (3 * OOLITEBOX_KEYS_POLL_INTERVAL)
#define OOLITEBOX_GPIO_LED_WLAN					 12
#define OOLITEBOX_GPIO_LED_WAN					 4
#define OOLITEBOX_GPIO_LED_SYSTEM                13
#define OOLITEBOX_GPIO_LED_LAN1					 16
#define OOLITEBOX_GPIO_LED_LAN2					 15
#define OOLITEBOX_GPIO_LED_LAN3					 14
#define OOLITEBOX_GPIO_LED_LAN4					 11
#define OOLITEBOX_GPIO_LED_3G  					 0
#define OOLITEBOX_GPIO_PCIE1_CTR				 1
#define OOLITEBOX_GPIO_PCIE2_CTR 				 2
static const char *oolitebox_part_probes[] = {
    "tp-link",
    NULL,
};

static struct flash_platform_data oolitebox_flash_data = {
    .part_probes    = oolitebox_part_probes,
};


static struct gpio_keys_button oolitebox_gpio_keys[] __initdata = {
{
    .desc   = "reset button",
    .type   = EV_KEY,
    .code   = KEY_RESTART,
    .debounce_interval = OOLITEBOX_KEYS_DEBOUNCE_INTERVAL,
    .gpio   = OOLITEBOX_GPIO_BTN_RST,
    .active_low = 1,
},
    };

static struct gpio_led oolitebox_leds_gpio[] __initdata = {
        {
                .name           = "oolitebox:wifi0",
                .gpio           = OOLITEBOX_GPIO_LED_WLAN,
                .active_low     = 1,
        }, {
                .name           = "oolitebox:wan",
                .gpio           = OOLITEBOX_GPIO_LED_WAN,
                .active_low     = 1,
        }, {
                .name           = "oolitebox:system",
                .gpio           = OOLITEBOX_GPIO_LED_SYSTEM,
                .active_low     = 1,
        }, {
                .name           = "oolitebox:lan1",
                .gpio           = OOLITEBOX_GPIO_LED_LAN1,
                .active_low     = 1,
        }, {
				.name		    = "oolitebox:lan2",
				.gpio	   		= OOLITEBOX_GPIO_LED_LAN2,
				.active_low		= 1,
		}, {
				.name			= "oolitebox:lan3",
				.gpio 			= OOLITEBOX_GPIO_LED_LAN3,
				.active_low		= 1,
		}, {
				.name 			= "oolitebox:lan4",
				.gpio 			= OOLITEBOX_GPIO_LED_LAN4,
				.active_low		= 1,
		}, {
				.name 			= "oolitebox:3g",
				.gpio 			= OOLITEBOX_GPIO_LED_3G,
				.active_low	 	= 1,
		}
};

static void __init oolitebox_gpio_led_setup(void)
{
	unsigned int old_func,new_func;
	void __iomem *QCA9531_GPIO_FUNC = ioremap_nocache(AR71XX_GPIO_BASE +0x6c, 0x04);
	old_func = __raw_readl(QCA9531_GPIO_FUNC);
	new_func = old_func & (1 << 1);
	__raw_writel(new_func, QCA9531_GPIO_FUNC);
	iounmap(QCA9531_GPIO_FUNC);

    ath79_gpio_direction_select(OOLITEBOX_GPIO_LED_WAN, true);
    ath79_gpio_direction_select(OOLITEBOX_GPIO_LED_LAN1, true);
    ath79_gpio_direction_select(OOLITEBOX_GPIO_LED_LAN2, true);
    ath79_gpio_direction_select(OOLITEBOX_GPIO_LED_LAN3, true);
    ath79_gpio_direction_select(OOLITEBOX_GPIO_LED_LAN4, true);

    ath79_gpio_output_select(OOLITEBOX_GPIO_LED_WAN,QCA953X_GPIO_OUT_MUX_LED_LINK5);
    ath79_gpio_output_select(OOLITEBOX_GPIO_LED_LAN1,QCA953X_GPIO_OUT_MUX_LED_LINK1);
    ath79_gpio_output_select(OOLITEBOX_GPIO_LED_LAN2,QCA953X_GPIO_OUT_MUX_LED_LINK2);
    ath79_gpio_output_select(OOLITEBOX_GPIO_LED_LAN3,QCA953X_GPIO_OUT_MUX_LED_LINK3);
    ath79_gpio_output_select(OOLITEBOX_GPIO_LED_LAN4,QCA953X_GPIO_OUT_MUX_LED_LINK4);



	ath79_register_leds_gpio(-1,ARRAY_SIZE(oolitebox_leds_gpio),oolitebox_leds_gpio);
	ath79_register_gpio_keys_polled(-1,OOLITEBOX_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(oolitebox_gpio_keys),oolitebox_gpio_keys);
}

static void __init oolitebox_setup(void)
{
   // u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);
    u8 *art = ath79_get_eeprom();
    ath79_register_m25p80(&oolitebox_flash_data);

 	oolitebox_gpio_led_setup();
    ath79_gpio_function_enable(AR934X_GPIO_FUNC_JTAG_DISABLE);
    ath79_register_usb();

    ath79_register_pci();


    ath79_wmac_set_led_pin(OOLITEBOX_GPIO_LED_WLAN);
    ath79_register_wmac(art + OOLITEBOX_WMAC_CALDATA_OFFSET, NULL);

    ath79_register_mdio(0, 0x0);
    ath79_register_mdio(1, 0x0);

    ath79_init_mac(ath79_eth1_data.mac_addr, art + OOLITEBOX_MAC1_OFFSET, 0);

    ath79_init_mac(ath79_eth0_data.mac_addr, art + OOLITEBOX_MAC0_OFFSET, 0);
    

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
    MIPS_MACHINE(ATH79_MACH_OOLITEBOX, "OOLITEBOX", "OOLITEBOX board",
    oolitebox_setup);


