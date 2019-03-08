/*
 * wifi.c
 *
 * Created: 2/14/2019 3:00:31 AM
 *  Author: jihoo
 */ 

#include "wifi.h"


volatile uint32_t received_byte_wifi = 0;
volatile uint32_t input_pos_wifi = 0;


// Handler for incoming data from the WIFI. Calls process_incoming_byte_wifi when a new byte arrives
void wifi_usart_handler(void)															
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


// Stores every incoming by (in_byte) from the AMW136 in a buffer
static void process_incoming_byte_wifi(uint8_t in_byte)									
{
	input_line_wifi[input_pos_wifi] = in_byte;            // Saves the incoming byte into a next position
	input_pos_wifi = input_pos_wifi + 1;				  // Increments the position
}


// Handler for "command complete" rising-edge interrupt from AMW136. Process the response of the AMW136
static void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask) {				// id of what is going on, mask of which pin what triggered the interrupt	
	unused(ul_id);				// unused is a silencer to avoid compiler warning
	unused(ul_mask);

	process_data_wifi();
}



// Processes the response of the AMW136, which should be stored in the buffer filled by process_incoming_byte_wifi. Looking for certain responses that the AMW136 gives
static void process_data_wifi(void)
{
		if (strstr(input_line_wifi, "")){
			ioport_toggle_pin_level(PIN_LED);
		}
}

