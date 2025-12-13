/*
 * led.h
 *
 *  Created on: 11 déc. 2025
 *      Author: ja.MOPAKA
 */

#ifndef LED_CONTROL_LED_H_
#define LED_CONTROL_LED_H_

#include <stdio.h>
#include "common_data.h"
#include "r_ioport_api.h"

#define LED_OFF     1
#define LED_ON      0

#define LED_GRN     IOPORT_PORT_06_PIN_00
#define LED_YLW     IOPORT_PORT_06_PIN_01
#define LED_RED     IOPORT_PORT_06_PIN_02

void hmi_set_status_led(unsigned int green_state, unsigned int yellow_state, unsigned int red_state);

#endif /* LED_CONTROL_LED_H_ */
