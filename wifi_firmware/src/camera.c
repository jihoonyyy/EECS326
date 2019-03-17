/*
 * camera.c
 *
 * Created: 2/14/2019 3:06:50 AM
 *  Author: jihoo
 */ 


#include "asf.h"
#include "conf_board.h"
#include "camera.h"

volatile uint32_t g_ul_vsync_flag = false;


// brief Handler for vertical synchronization using by the OV7740 image sensor.
void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}



// brief Initialize Vsync_Handler. 
void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV2640_VSYNC_PIO, OV2640_VSYNC_ID, OV2640_VSYNC_MASK,
			OV2640_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV2640_VSYNC_ID);

	
}


void configure_twi(void)
{
	
	twi_options_t opt;
	
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);				// configure TWI clock
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);			// configure TWI data pin
	
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);
	

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
	
}




/**
 * \brief Initialize PIO capture for the OV7740 image sensor communication.
 *
 * \param p_pio PIO instance to be configured in PIO capture mode.
 * \param ul_id Corresponding PIO ID.
 */
void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable peripheral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);

}


/**
 * \brief Capture OV7740 data to a buffer.
 *
 * \param p_pio PIO instance which will capture data from OV7740 image sensor.
 * \param p_uc_buf Buffer address where captured data must be stored.
 * \param ul_size Data frame size.
 */
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size)     
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
		} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
		} else {
		return 0;
	}
}



void init_camera(void)
{
	// configure clock pins
	gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);
		
	// configure HSYNC and VSYNC
	gpio_configure_pin(HSYNC_GPIO, HSYNC_FLAGS);
	gpio_configure_pin(VSYNC_GPIO, VSYNC_FLAGS);
	
	// configure DATA pins
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	
	
	
	pmc_enable_pllack(7, 0x1, 1); /* PLLA work at 96 Mhz */
	
	
	/* Init Vsync handler*/
	init_vsync_interrupts();
	g_ul_vsync_flag = false;	// reset the flag
	
	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);

	/* Turn on ov7740 image sensor using power pin */
	ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK0 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);				// changed from 0 to 1 since we are using PCK1, for address info, refer to page 529 of MCU data sheet
	PMC->PMC_SCER = PMC_SCER_PCK1;												// changed from 0 to 1 since we are using PCK1
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {									// changed from 0 to 1 since we are using PCK1
	}
	
	
	configure_twi();              // initialize twi 
	

	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);	

}

void configure_camera(void)
{
		/* ov7740 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}

	/* ov7740 configuration */
	ov_configure(BOARD_TWI, JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_640x480);
	
}





uint8_t start_capture(void)
{

	/* Enable vsync interrupt*/
	pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait g_vsync_flag = 1 before start process
	 */
	while (!g_ul_vsync_flag) {
	}

	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV2640_DATA_BUS_PIO);

	
	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	pio_capture_to_buffer(OV2640_DATA_BUS_PIO, image_buffer, 25000);
	
	/* Wait end of capture*/
	while (!((OV2640_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}

	/* Disable pio capture*/
	pio_capture_disable(OV2640_DATA_BUS_PIO);

	/* Reset vsync flag*/
	g_ul_vsync_flag = false;
	
	uint8_t img_len = find_image_len();
	return img_len;
}






uint8_t find_image_len (void)
{
	start_point = 0;
	end_point = 0;
	bool no_error_flag = false;																					// This flag is used for checking error
	
	for (uint32_t i = 0; i+1 < sizeof(image_buffer); i++)
	{
		if (image_buffer[i] == 0xff && image_buffer[i + 1] == 0xd8)												// Checking for initial value for JPEG
		{
			start_point = i;
			no_error_flag = true;																				// we've detected initial value for JPEG
		}
		
		if (image_buffer[i] == 0xff && image_buffer[i + 1] == 0xd9 && no_error_flag == true) {
			end_point = i+2;
			return 1;
		}
		
	}
	
	
	// reset everything if not proper
	start_point = 0;																						
	end_point = 0;
	return 0;
}





