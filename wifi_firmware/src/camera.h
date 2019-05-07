/*
 * camera.h
 *
 * Created: 2/14/2019 3:07:06 AM
 *  Author: jihoo
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>
#include <string.h>



/** PCK1 */
#define PIN_PCK1                       (PIO_PA17_IDX)
#define PIN_PCK1_FLAGS                 (PIO_PERIPH_B | PIO_DEFAULT)



/** TWI0 pins */
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0

/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     (400000UL)


/** TWI0 data pin */
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}



/** TWI0 pins */
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0

/** TWI0 pins */
#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn

/** TWI0 clock pins definition */
#define TWI0_CLK_GPIO                  PIO_PA4_IDX
#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_MASK                  PIO_PA4
#define TWI0_CLK_PIO                   PIOA
#define TWI0_CLK_ID                    ID_PIOA
#define TWI0_CLK_TYPE                  PIO_PERIPH_A
#define TWI0_CLK_ATTR                  PIO_DEFAULT



/** TWI0 Data pins definition */
#define TWI0_DATA_GPIO                 PIO_PA3_IDX
#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_DATA_MASK                 PIO_PA3
#define TWI0_DATA_PIO                  PIOA
#define TWI0_DATA_ID                   ID_PIOA
#define TWI0_DATA_TYPE                 PIO_PERIPH_A
#define TWI0_DATA_ATTR                 PIO_DEFAULT




/** OV Data Bus pins */
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT
#define OV_DATA_BUS_MASK               (0xFF000000UL)


// Define data buses from camera to MCU
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_D0                 PIO_PA24_IDX
#define OV_DATA_BUS_D1                 PIO_PA25_IDX
#define OV_DATA_BUS_D2                 PIO_PA26_IDX
#define OV_DATA_BUS_D3                 PIO_PA27_IDX
#define OV_DATA_BUS_D4                 PIO_PA28_IDX
#define OV_DATA_BUS_D5                 PIO_PA29_IDX
#define OV_DATA_BUS_D6                 PIO_PA30_IDX
#define OV_DATA_BUS_D7                 PIO_PA31_IDX




/** HSYNC pin */
#define PIN_OV_HSYNC                   { PIO_PA16, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE }

/** VSYNC pin */
#define PIN_OV_VSYNC                   { PIO_PA15, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE }


// OV VSYNC pin configuration
#define VSYNC_PIO					PIOA         
#define VSYNC_ID					ID_PIOA      
#define VSYNC_MASK					PIO_PA15    
#define VSYNC_TYPE					PIO_PULLUP   
#define VSYNC_GPIO					PIO_PA15_IDX
#define VSYNC_FLAGS					(PIO_PULLUP	| PIO_IT_RISE_EDGE)


// OV HSYNC pin configuration
#define HSYNC_PIO					PIOA
#define HSYNC_ID					ID_PIOA
#define HSYNC_MASK					PIO_PA16
#define HSYNC_TYPE					PIO_PULLUP
#define HSYNC_GPIO					PIO_PA16_IDX
#define HSYNC_FLAGS					(PIO_PULLUP	| PIO_IT_RISE_EDGE)



#define PIN_OV_RST                     { PIO_PA20, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}

// OV RESET pin configuration
#define OV_RST_PIO                     PIOA
#define OV_RST_ID                      ID_PIOA
#define OV_RST_MASK                    PIO_PA20
#define OV_RST_TYPE                    PIO_OUTPUT_1
#define OV_RST_GPIO                    PIO_PA20_IDX
#define OV_RST_FLAGS                   (PIO_OUTPUT_1 | PIO_DEFAULT)





#define MAX_BUFFER					100000//20000			//100000
volatile uint32_t g_ul_vsync_flag;    /* Vsync signal information (true if it's triggered and false otherwise) */
uint32_t start_point;
uint32_t end_point;


volatile char image_buffer[MAX_BUFFER];



void vsync_handler(uint32_t ul_id, uint32_t ul_mask);                           // brief Handler for vertical synchronization using by the OV7740 image sensor.
void init_vsync_interrupts(void);												// brief Initialize Vsync_Handler.
void configure_twi(void);														// configuration of TWI (two ;wire interface)
void pio_capture_init(Pio *p_pio, uint32_t ul_id);								// brief Initialize PIO capture for the OV7740 image sensor communication.
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size);   // brief Capture OV7740 data to a buffer.
void init_camera(void);															// Configuration of camera pins, camera clock (XCLK), and calling the configure_twi function
void configure_camera(void);													// Configuration of OV2640 registers for desired operation
uint8_t start_capture(void);													// Captures an image after a rising edge of VSYNC, and gets image length. Returns 1 on success (i.e. a nonzero image lenth), 0 on error
uint8_t find_image_len(void);													// Finds image length based on JPEG protocol. Returns 1 on success



#endif /* CAMERA_H_ */