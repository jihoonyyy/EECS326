/*
 * wifi.c
 *
 * Created: 2/14/2019 3:00:31 AM
 *  Author: jihoo
 */ 

#include "wifi.h"


volatile uint32_t received_byte_wifi = 0;
volatile uint32_t input_pos_wifi = 0;
volatile bool new_rx_wifi = false;
volatile bool wifi_comm_success = false;


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
	input_line_wifi[input_pos_wifi] = in_byte;            // Saves the incoming byte into a next position
	input_pos_wifi = input_pos_wifi + 1;				  // Increments the position
}


// Handler for "command complete" rising-edge interrupt from AMW136. Process the response of the AMW136
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)						// id of what is going on, mask of which pin what triggered the interrupt	
{
	unused(ul_id);				// unused is a silencer to avoid compiler warning
	unused(ul_mask);
	
	wifi_comm_success = true;
	process_data_wifi();
	for (int jj=0;jj<MAX_INPUT_WIFI;jj++) input_line_wifi[jj] = 0;                          // Once the data was processed, clear the buffer
	input_pos_wifi = 0;
}


// Processes the response of the AMW136, which should be stored in the buffer filled by process_incoming_byte_wifi. Looking for certain responses that the AMW136 gives
void process_data_wifi(void)
{
		if (strstr(input_line_wifi, "")){
			ioport_toggle_pin_level(PIN_LED);
		}
}


void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	
	
}


// Configuration of USART port used to communicate with the AMW136.
void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	//gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);
	
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