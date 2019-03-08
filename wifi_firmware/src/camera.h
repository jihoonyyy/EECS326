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



// Image sensor VSYNC pin.
#define OV7740_VSYNC_PIO	       OV_VSYNC_PIO
#define OV7740_VSYNC_ID		       OV_VSYNC_ID
#define OV7740_VSYNC_MASK              OV_VSYNC_MASK
#define OV7740_VSYNC_TYPE              OV_VSYNC_TYPE


/** TWI0 pins */
#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn

/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     (400000UL)


/** OV Data Bus pins */
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT


/* Image sensor board defines. */
// Image sensor Power pin.
#define OV_POWER_PIO                   OV_SW_OVT_PIO
#define OV_POWER_MASK                  OV_SW_OVT_MASK



volatile uint32_t g_ul_vsync_flag;    /* Vsync signal information (true if it's triggered and false otherwise) */




void vsync_handler(uint32_t ul_id, uint32_t ul_mask);                           // brief Handler for vertical synchronization using by the OV7740 image sensor.
void init_vsync_interrupts(void);												// brief Initialize Vsync_Handler.
void configure_twi(void);														// configuration of TWI (two ;wire interface)
void pio_capture_init(Pio *p_pio, uint32_t ul_id)								// brief Initialize PIO capture for the OV7740 image sensor communication.
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size);   // brief Capture OV7740 data to a buffer.
void init_camera(void);															// Configuration of camera pins, camera clock (XCLK), and calling the configure_twi function
void configure_camera(void);													// Configuration of OV2640 registers for desired operation
uint8_t start_capture(void);													// Captures an image after a rising edge of VSYNC, and gets image length. Returns 1 on success (i.e. a nonzero image lenth), 0 on error
uint8_t find_image_len(void);													// Finds image length based on JPEG protocol. Returns 1 on success



#endif /* CAMERA_H_ */