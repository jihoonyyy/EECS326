/*
 * wifi.c
 *
 * Created: 2/14/2019 3:00:31 AM
 *  Author: jihoo
 */ 

#include "wifi.h"
#include "camera.h"
#include "timer_interface.h"

volatile uint32_t received_byte_wifi = 0;
volatile uint32_t input_pos_wifi = 0;
volatile bool new_rx_wifi = false;
volatile bool wifi_comm_success = false;
volatile uint32_t web_setup_flag = false;
volatile bool image_transfer_flag = false;
volatile bool wifi_websocket_flag = false;
volatile bool wifi_status = false;


// Handler for incoming data from the WIFI. Calls process_incoming_byte_wifi when a new byte arrives
void WIFI_USART_HANDLER(void)														
{
	uint32_t ul_status;									// 8 byte variable for status

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);           // Gets Channel Status Register data 

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {                    // Checking if we received a byte
		usart_read(WIFI_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}


// Stores every incoming by (in_byte) from the AMW136 in a buffer
void process_incoming_byte_wifi(uint8_t in_byte)									
{
	input_line_wifi[input_pos_wifi++] = in_byte;            // Saves the incoming byte into a next position, and increment
}


// Handler for "command complete" rising-edge interrupt from AMW136. Process the response of the AMW136
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)						// id of what is going on, mask of which pin what triggered the interrupt	
{
	unused(ul_id);				// unused is a silencer to avoid compiler warning
	unused(ul_mask);
	
	wifi_comm_success = true;
	process_data_wifi();
	for (int jj=0; jj<MAX_INPUT_WIFI; jj++) {input_line_wifi[jj] = 0; }                         // Once the data was processed, clear the buffer, but consider not using this loop
	input_pos_wifi = 0;
}


// Processes the response of the AMW136, which should be stored in the buffer filled by process_incoming_byte_wifi. Looking for certain responses that the AMW136 gives
void process_data_wifi(void)
{
		if (strstr(input_line_wifi, "Start transfer")){                 // string provided from the firmware design pdf
			image_transfer_flag = true;
			wifi_status = true;
		}
		
		if (strstr(input_line_wifi, "None"))
		{
			wifi_websocket_flag = false;		
		}
		
		if (strstr(input_line_wifi, "Client not connected")) 
		{
			wifi_websocket_flag = false;
		}
		
		if (strstr(input_line_wifi, "Websocket connected"))            
		{
			wifi_websocket_flag = true;
		}
		
		if (strstr(input_line_wifi, "Websocket disconnected"))  
		{
			wifi_websocket_flag = false;
		}
		
		if (strstr(input_line_wifi, ","))             // not necessarily required
		{
			wifi_websocket_flag = true;
		}
		
		if (strstr(input_line_wifi, "2"))
		{
			wifi_status = true;
		}
}




void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	web_setup_flag = true;
	
}


// Configuration of USART port used to communicate with the AMW136.
void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(WIFI_USART_ID);

	/* Configure USART. */
	usart_init_hw_handshaking(WIFI_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);
	
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}



// Configuration of "command complete" rising-edge interrupt.
void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_NUM, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM);
}


// Configuration of button interrupt to initiate the web setup
void configure_wifi_web_setup_pin(void)
{
	
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_SETUP_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_SETUP_PIO, WIFI_SETUP_ID, WIFI_SETUP_PIN_NUM, WIFI_SETUP_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_SETUP_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_SETUP_PIO, WIFI_SETUP_PIN_NUM);
	
}


/* Writes a command (comm) to the AMW136, and waits either for an acknowledgment or a timeout. The timeout can be created by setting the global variable "counts" to zero, 
which will automatically increment every second, and waiting while counts < cnt */
void write_wifi_command(char* comm, uint8_t cnt)
{
	usart_write_line(WIFI_USART, comm);
	counts = 0;
	while (counts < cnt)
	{
		if(wifi_comm_success)						// if wifi_comm_success is true
		{
			wifi_comm_success = false;				// reset the flag
			break;									// get out of the loop
		}
	}
}
		


// Writes an image from SAM4s8B to the AMW136. If the length of the image is zero (i.e.. the image is not valid), return. Otherwise, follow the protocol
void write_image_to_file(void)
{
	image_available = find_image_len();
	img_length = end_point - start_point;
	
	if (image_available == 0) {
		return;
	}
	else {
		char img_array[1000];
		sprintf(img_array, "image_transfer %d\r\n", img_length);
		
		write_wifi_command(img_array, 3);				// write command to the AMW136
		if(!image_transfer_flag) {						// if image is not passed, don't finish it
			return;  
		}

		for (uint32_t i = start_point; i <= end_point + 1; i++) {
				usart_putchar(WIFI_USART, image_buffer[i]);
		}
		image_transfer_flag = false;						// reset the flag after data transfer
		delay_ms(50);										// slight delay according to the step
	}		
	
}
