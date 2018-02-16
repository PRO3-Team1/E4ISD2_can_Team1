/*
 * button.c
 *
 *  Created on: 6. nov. 2017
 *      Author: Test Bruger
 */

#include "button.h"

void button_init() {

	//Um10562. UM10562. Table 16.
	LPC_SYSCTL->PCONP |= 1 <<15;

	//FUNC 00 + PULL DOWN + DIGITAL MODE. UM10562. Table 85
	LPC_IOCON->p[BTN0PORT][BTN0PIN] = 0X88;

	//Set as input. UM10562. Table 86
	LPC_GPIO[BTN0PORT].DIR &= ~(1UL << BTN0PIN);

	LPC_IOCON->p[BTN1PORT][BTN1PIN] = 0X88;
	LPC_GPIO[BTN1PORT].DIR &= ~(1UL << BTN1PIN);

	LPC_IOCON->p[BTN2PORT][BTN2PIN] = 0x88;
	LPC_GPIO[BTN2PORT].DIR &= ~(1UL << BTN2PIN);
}



int button_get(enum button_t btn) {
	switch(btn)
	{
	case BTN_0:
		return BTN0GET();
		break;
	case BTN_1:
		return BTN1GET();
		break;
	case BTN_2:
		return BTN2GET();
		break;

	default:
		return 0;
	}

}
