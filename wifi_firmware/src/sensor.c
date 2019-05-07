/*
 * Sensor.c
 *
 * Created: 4/21/2019 8:34:23 PM
 *  Author: jihoo
 */ 


#include "asf.h"
#include "sensor.h"
#include "conf_clock.h"


/**
 * \brief Handler for acc comparison event
 */
void ACC_Handler(void)
{
	re200b_motion_detect_handler();
}

/**
 * \brief Application entry point for smc_lcd example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t i = 0;

	sysclk_init();
	board_init();


	/* Initialize Pir sensor */
	re200b_motion_detect_init(ACC_MR_SELMINUS_AD0, ACC_MR_SELPLUS_AD4);

	while (1) {
		/* Reset motion detection (enable interrupt, clear variable,
		 * clear status register) */
		re200b_motion_detect_enable();
		re200b_motion_detect_reset();
		acc_get_interrupt_status(ACC);

		/* Enter sleep mode */
		pmc_enable_sleepmode(0);

		/* Check if a motion was detected */
		if (re200b_motion_detection() != 0UL) {
			/* Disable motion detection */
			re200b_motion_detect_disable();
		}
	}
}


