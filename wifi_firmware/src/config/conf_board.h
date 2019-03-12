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


/** TWI0 pins */
#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn


// Image sensor VSYNC pin. Make sure we change these pins for our own configuration!!!!!!!!!
#define OV7740_VSYNC_PIO	       PIOA          //  OV_VSYNC_PIO
#define OV7740_VSYNC_ID		       ID_PIOA       //  OV_VSYNC_ID
#define OV7740_VSYNC_MASK          PIO_PA15      //  OV_VSYNC_MASK
#define OV7740_VSYNC_TYPE          PIO_PULLUP    //  OV_VSYNC_TYPE





#define LED_PIN				PIO_PA20_IDX
#define WIFI_CTS_PIN		PIO_PA7_IDX
#define WIFI_RESET_PIN      PIO_PB3_IDX
#define WIFI_WEB_SETUP_PIN  PIO_PB4_IDX          // Used PB4 for setup pin



#endif // CONF_BOARD_H
