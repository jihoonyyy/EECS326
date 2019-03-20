/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <string.h>
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"

#include "timer_interface.h"



int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	wdt_disable(WDT);
	board_init();
	
	
	web_setup_flag = false;
	
	
	
	configure_tc();						// Configuring the timer from timer_interface
	
	configure_usart_wifi();				// Configuring WIFI USART
	configure_wifi_comm_pin();			// Configuring WIFI command pin
	configure_wifi_web_setup_pin();     // Configuring WIFI web setup pin
	
	// Hard reset the wifi chip. Free "connected-to-mcu" GPIOs are PB4, PB3, PA11, 
	gpio_set_pin_low(WIFI_RESET_PIN);   // Pulling the pin low, hard resetting
	delay_ms(100);                      // Delaying for 100 ms, as instructed
	gpio_set_pin_high(WIFI_RESET_PIN);  // Pulling the pin high


	while(!ioport_get_pin_level(WIFI_STATUS_PIN)) {
		if (web_setup_flag == true)
		{
			usart_write_line(WIFI_USART, "setup web\r\n");
			web_setup_flag = false;
		}
	}
	

	
	// Initialize the camera
	init_camera();
	
	
	// Configure the camera
	configure_camera();

	write_wifi_command("set sy c p off\r\n", 3);					// Telling the wifi chip to turn off the command prompt
	write_wifi_command("set sy c e off\r\n", 3);					// Telling the wifi chip to turn off the echo
	
	
	
	while(1)
	{
		if (web_setup_flag) {
			write_wifi_command("setup web\r\n", 3);
			web_setup_flag = false;
			while (!ioport_get_pin_level(WIFI_STATUS_PIN)) {
			}
		}
		
		else {
			if (wifi_status == false) {
 				gpio_set_pin_low(WIFI_RESET_PIN);   // Pulling the pin low, hard resetting
 				delay_ms(100);                      // Delaying for 100 ms, as instructed
 				gpio_set_pin_high(WIFI_RESET_PIN);  // Pulling the pin high
				while (!wifi_status) {
					write_wifi_command("get wl n s\r\n",2);
				}
			}
			else 
			{
				if (wifi_websocket_flag == false)
				{
					write_wifi_command("poll all\r\n",3);
					delay_ms(1000);
				}
				else
				{
					start_capture();
					write_image_to_file();
				}
			}
			
		}
	}
}
	
