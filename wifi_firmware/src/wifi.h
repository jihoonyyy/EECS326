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



#define WIFI_USART                USART0






volatile uint32_t received_byte_wifi;
volatile uint32_t input_pos_wifi;
volatile char input_line_wifi[1000];    // giving 1000 bytes so that buffer doesn't run over easily






#endif /* WIFI_H_ */