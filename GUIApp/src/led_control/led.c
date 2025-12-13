/*
 * led.c
 *
 *  Created on: 11 déc. 2025
 *      Author: ja.MOPAKA
 */
#include "led.h"

void hmi_set_status_led(unsigned int green_state, unsigned int yellow_state, unsigned int red_state)
{
    g_ioport.p_api->pinWrite(LED_GRN, green_state);
    g_ioport.p_api->pinWrite(LED_YLW, yellow_state);
    g_ioport.p_api->pinWrite(LED_RED, red_state);
}


