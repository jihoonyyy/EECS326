/*
 * wifi.c
 *
 * Created: 2/14/2019 3:00:31 AM
 *  Author: jihoo
 */ 

#include "wifi.h"


volatile uint32_t received_byte_wifi = 0;
volatile uint32_t input_pos_wifi = 0;


void wifi_usart_handler(void)															// Handler for incoming data from the WIFI. Calls process_incoming_byte_wifi when a new byte arrives
{
	uint32_t ul_status;									// 8 byte variable for status

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);           // Gets Channel Status Register data 

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {                    // Checking if we received a byte
		usart_read(WIFI_USART, &received_byte_wifi);
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}


static void process_incoming_byte_wifi(uint8_t in_byte)									// Stores every incoming by (in_byte) from the AMW136 in a buffer
{
	input_line_wifi[input_pos_wifi] = in_byte;            // Saves the incoming byte into a next position
	input_pos_wifi = input_pos_wifi + 1;				  // Increments the position
}


static void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask) {			// Handler for "command complete" rising-edge interrupt from AMW136. Process the response of the AMW136
	unused(ul_id);
	unused(ul_mask);

	process_data_wifi();
}


