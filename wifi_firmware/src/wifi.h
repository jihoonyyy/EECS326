/*
 * wifi.h
 *
 * Created: 2/14/2019 3:01:30 AM
 *  Author: jihoo
 */ 


#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>



#define WIFI_USART                  USART0
#define WIFI_USART_BAUDRATE			115200
#define WIFI_USART_ID				ID_USART0     // Peripheral id
#define WIFI_USART_IRQn				USART0_IRQn	  // Interrupt 
#define WIFI_USART_HANDLER			USART0_Handler


// USART byte setup
#define WIFI_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT		// 8 bit long
#define WIFI_USART_PARITY			US_MR_PAR_NO			// no parity bit
#define WIFI_USART_STOP_BITS		US_MR_NBSTOP_1_BIT		// 1 stop bit
#define WIFI_USART_MODE				US_MR_CHMODE_NORMAL		// normal mode


/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)


/** All interrupt mask. */
#define ALL_INTERRUPT_MASK  0xffffffff



// Pin setups, these should be changed depending on our setting
#define WIFI_COMM_PIN_NUM			PIO_PB10
#define WIFI_COMM_PIO				PIOB
#define WIFI_COMM_ID				ID_PIOB
#define WIFI_COMM_ATTR				PIO_PULLUP | PIO_IT_RISE_EDGE                


// Pin setups for wifi setup, can be changed depending on our setting
#define WIFI_SETUP_PIN_NUM			PIO_PB4
#define WIFI_SETUP_PIO				PIOB
#define WIFI_SETUP_ID				ID_PIOB
#define WIFI_SETUP_ATTR				PIO_PULLUP | PIO_IT_RISE_EDGE
#define WIFI_SETUP_PIN				PIO_PB4_IDX

#define MAX_INPUT_WIFI				1000



volatile uint32_t received_byte_wifi;
volatile uint32_t input_pos_wifi;
volatile bool wifi_comm_success;
volatile bool new_rx_wifi;
volatile char input_line_wifi[MAX_INPUT_WIFI];    // giving 10000 bytes so that buffer doesn't run over easily
volatile uint32_t web_setup_flag;
volatile bool image_transfer_flag;
volatile bool wifi_websocket_flag;
volatile bool wifi_status;
uint8_t image_available;
uint32_t img_length;


// void wifi_usart_handler(void);
void process_incoming_byte_wifi(uint8_t in_byte);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void process_data_wifi(void);
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);
void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void configure_wifi_web_setup_pin(void);
void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_file(void);



#endif /* WIFI_H_ */