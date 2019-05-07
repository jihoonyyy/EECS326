/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS  (12000000U)
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
//
///** Configure TWI0 pins (for OV7740  communications). */
//#define CONF_BOARD_TWI0
//
///** Configure PCK0 pins (for OV7740  communications). */
//#define CONF_BOARD_PCK1

/** TWI0 pins */
#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn

// OV VSYNC pin configuration
#define OV2640_VSYNC_PIO			VSYNC_PIO
#define OV2640_VSYNC_ID				VSYNC_ID
#define OV2640_VSYNC_MASK			VSYNC_MASK
#define OV2640_VSYNC_TYPE			VSYNC_TYPE

// OV DATA pin configuration
#define OV2640_DATA_BUS_PIO			OV_DATA_BUS_PIO
#define OV2640_DATA_BUS_ID          OV_DATA_BUS_ID



#define WIFI_STATUS_PIN		PIO_PB11_IDX
#define OV2640_RESET_PIN    PIO_PA20_IDX
#define WIFI_CTS_PIN		PIO_PA7_IDX
#define WIFI_RESET_PIN      PIO_PB0_IDX
#define WIFI_WEB_SETUP_PIN  PIO_PA22_IDX          // Used PB4 for setup pin
#define WIFI_COMM_PIN		PIO_PB10_IDX
// #define WIFI_

#endif // CONF_BOARD_H
