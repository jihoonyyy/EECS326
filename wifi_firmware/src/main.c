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
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_uart_serial.h"
#include "timer_interface.h"



int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	wdt_disable();
	board_init();
	
	configure_tc();						// Configuring the timer from timer_interface
	
	configure_usart_wifi();				// Configuring WIFI USART
	configure_wifi_comm_pin();			// Configuring WIFI command pin
	configure_wifi_web_setup_pin();     // Configuring WIFI web setup pin
	
	// Hard reset the wifi chip. Free "connected-to-mcu" GPIOs are PB4, PB3, PA11, 
	gpio_set_pin_low(WIFI_RESET_PIN);   // Pulling the pin low, hard resetting
	delay_ms(100);                      // Delaying for 100 ms, as instructed
	gpio_set_pin_high(WIFI_RESET_PIN);  // Pulling the pin high
	
	
	// Wait for the WIFI to connect to a network. Listen for the Web Setup Pin
	while(ioport_get_pin_level(WIFI_WEB_SETUP_PIN) == 0)
	{
		if (web_setup_flag == true)
		{
			usart_write_line(WIFI_USART, "Setting up web\r\n");
			web_setup_flag = false;
		}
		
	}
	
	// Initialize the camera
	init_camera();
	
	
	// Configure the camera
	configure_camera();
	
	usart_write_line(WIFI_USART, "set sy c p off\r\n");					// Telling the wifi chip to turn off the command prompt
	usart_write_line(WIFI_USART, "set sy c e off\r\n");					// Telling the wifi chip to turn off the echo
	
	
	
	while(1)
	{
		if (web_setup_flag) {											// Check for Web Setup Request, and goes through this "if" if flag is up
			usart_write_line(WIFI_USART, "Setting up web\r\n");
			web_setup_flag = false;			
			}
			
		else {															// wifi setup flag is false, so check if network is connected
			if (ioport_get_pin_level(WIFI_WEB_SETUP_PIN) == 1) {		// if connected*
				if (!wifi_websocket_flag)					
				{
					delay_ms(1000);							// if not, delay 1s and start over
				}
	
				else										// if there is an open stream
				{
						start_capture();					// capturing
						write_image_to_file();				// transferring it to AMW136
				}
			}
			
			else {
				gpio_set_pin_low(WIFI_RESET_PIN);   // Pulling the pin low, hard resetting
				delay_ms(100);                      // Delaying for 100 ms, as instructed
				gpio_set_pin_high(WIFI_RESET_PIN);  // Pulling the pin high
			}
		}
	}
	
	
}
