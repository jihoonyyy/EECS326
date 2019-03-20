/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "camera.h"

void board_init(void)
{
	
	ioport_init();
	
	ioport_set_pin_dir(OV2640_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OV2640_RESET_PIN, true);
	
	
	ioport_set_pin_dir(WIFI_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_RESET_PIN, false);


	ioport_set_pin_dir(WIFI_COMM_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_level(WIFI_COMM_PIN, IOPORT_MODE_PULLDOWN);

	ioport_set_pin_dir(WIFI_WEB_SETUP_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_level(WIFI_WEB_SETUP_PIN, IOPORT_MODE_PULLDOWN);

	// pull the CTS pin (RTS of MCU) low, accordingly to task 4
	ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CTS_PIN, false);
	
	
	
	
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);				// configure TWI clock
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);			// configure TWI data pin
	
	
	gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);
	
	
	// configure reset
	gpio_configure_pin(OV_RST_GPIO, OV_RST_FLAGS);
	
		
	// configure HSYNC and VSYNC
	gpio_configure_pin(HSYNC_GPIO, HSYNC_FLAGS);
	gpio_configure_pin(VSYNC_GPIO, VSYNC_FLAGS);
	
	// configure DATA pins
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
}
