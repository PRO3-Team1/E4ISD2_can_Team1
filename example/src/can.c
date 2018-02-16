/*
 * @brief CAN example
 * This example show how to use CAN peripheral
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "board.h"
#include "button.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define CAN_CTRL_NO         0
#define LPC_CAN             (LPC_CAN1)
#define LPC_CAN_2           (LPC_CAN2)

// changed to mikkels id
#define CAN_TX_MSG_STD_ID (0x15)


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
static char WelcomeMenu[] =
		"\n\rHello NXP Semiconductors \r\n"
				"CAN DEMO : Use CAN to transmit and receive Message from CAN Analyzer\r\n"
				"CAN bit rate : 250kBit/s\r\n";

/*****************************************************************************
 * Private functions
 ****************************************************************************/
/* Print error */
static void PrintCANErrorInfo(uint32_t Status) {
	if (Status & CAN_ICR_EI) {
		DEBUGOUT("Error Warning!\r\n");
	}
	if (Status & CAN_ICR_DOI) {
		DEBUGOUT("Data Overrun!\r\n");
	}
	if (Status & CAN_ICR_EPI) {
		DEBUGOUT("Error Passive!\r\n");
	}
	if (Status & CAN_ICR_ALI) {
		DEBUGOUT("Arbitration lost in the bit: %d(th)\r\n",
				CAN_ICR_ALCBIT_VAL(Status));
	}
	if (Status & CAN_ICR_BEI) {

		DEBUGOUT("Bus error !!!\r\n");

		if (Status & CAN_ICR_ERRDIR_RECEIVE) {
			DEBUGOUT("\t Error Direction: Transmiting\r\n");
		} else {
			DEBUGOUT("\t Error Direction: Receiving\r\n");
		}

		DEBUGOUT("\t Error Location: 0x%2x\r\n", CAN_ICR_ERRBIT_VAL(Status));
		DEBUGOUT("\t Error Type: 0x%1x\r\n", CAN_ICR_ERRC_VAL(Status));
	}
}

/* Print CAN Message */
static void PrintCANMsg(CAN_MSG_T *pMsg) {
	uint8_t i;
	DEBUGOUT("\t**************************\r\n");
	DEBUGOUT("\tMessage Information: \r\n");
	DEBUGOUT("\tMessage Type: ");
	if (pMsg->ID & CAN_EXTEND_ID_USAGE) {
		DEBUGOUT(" Extend ID Message");
	} else {
		DEBUGOUT(" Standard ID Message");
	}
	if (pMsg->Type & CAN_REMOTE_MSG) {
		DEBUGOUT(", Remote Message");
	}
	DEBUGOUT("\r\n");
	DEBUGOUT("\tMessage ID :0x%x\r\n", (pMsg->ID & (~CAN_EXTEND_ID_USAGE)));
	DEBUGOUT("\tMessage Data :");
	for (i = 0; i < pMsg->DLC; i++)
		DEBUGOUT("%x ", pMsg->Data[i]);
	DEBUGOUT("\r\n\t**************************\r\n");
}

/* Reply message received */
static void ReplyNormalMessage(CAN_MSG_T *pRcvMsg) {
	CAN_MSG_T SendMsgBuf = *pRcvMsg;
	CAN_BUFFER_ID_T TxBuf;
	// Increment id
	SendMsgBuf.ID++;
	// id is made to extended id (toggle bit 30)
	SendMsgBuf.ID |= CAN_EXTEND_ID_USAGE;
 	TxBuf = Chip_CAN_GetFreeTxBuf(LPC_CAN_2);
	Chip_CAN_Send(LPC_CAN_2, TxBuf, &SendMsgBuf);
	DEBUGOUT("Message Replied!!!\r\n");
	PrintCANMsg(&SendMsgBuf);
}
/*****************************************************************************
 * Public functions
 ****************************************************************************/
void CAN_IRQHandler(void) {
	uint32_t IntStatus;
	CAN_MSG_T RcvMsgBuf;
	IntStatus = Chip_CAN_GetIntStatus(LPC_CAN_2);

	PrintCANErrorInfo(IntStatus);

	/* New Message came */
	if (IntStatus & CAN_ICR_RI) {
		Chip_CAN_Receive(LPC_CAN_2, &RcvMsgBuf);
		//DEBUGOUT("Message Received!!!\r\n");
		//PrintCANMsg(&RcvMsgBuf);

		if (RcvMsgBuf.ID & CAN_EXTEND_ID_USAGE) {
			DEBUGOUT("Was Extended id ignored");

		} else {
			//DEBUGOUT("Reply Long id Message");
			ReplyNormalMessage(&RcvMsgBuf);
		}
	}
}

int main(void) {
	CAN_BUFFER_ID_T TxBuf;
	CAN_MSG_T SendMsgBuf;

	SystemCoreClockUpdate();
	Board_Init();
	DEBUGOUT(WelcomeMenu);
	/* LPC_CAN use pins 0.0 and 0.1 */
	Board_CAN_Init(LPC_CAN);
	/* LPC_CAN_2 use pins 0.4 and 0.5 */
	Board_CAN_Init(LPC_CAN_2);

	Chip_CAN_Init(LPC_CAN, LPC_CANAF, LPC_CANAF_RAM);
	// LPC_CANAF = accepcens filter
	Chip_CAN_Init(LPC_CAN_2, LPC_CANAF, LPC_CANAF_RAM);
	// Set baurate on CAN 1
	Chip_CAN_SetBitRate(LPC_CAN, 250000);
	// Set baurate on CAN 2
	Chip_CAN_SetBitRate(LPC_CAN_2, 250000);
	Chip_CAN_EnableInt(LPC_CAN, CAN_IER_BITMASK);
	Chip_CAN_EnableInt(LPC_CAN_2, CAN_IER_BITMASK);

	/* set acceptsance filters to bypass */
	Chip_CAN_SetAFMode(LPC_CANAF, CAN_AF_BYBASS_MODE);

	/* enable interrupts */
	NVIC_EnableIRQ(CAN_IRQn);

	/* fill data in TX buffer */
	SendMsgBuf.ID = CAN_TX_MSG_STD_ID;
	SendMsgBuf.DLC = 4;
	SendMsgBuf.Type = 0;
	SendMsgBuf.Data[0] = 'M';
	SendMsgBuf.Data[1] = 'i';
	SendMsgBuf.Data[2] = 'c';
	SendMsgBuf.Data[3] = 'h';

	while (1) {
		// if user button is pressed go in if
		if (!button_get(0))
		{

			// get free TX buffer
			TxBuf = Chip_CAN_GetFreeTxBuf(LPC_CAN);

			// and transmit data
			Chip_CAN_Send(LPC_CAN, TxBuf, &SendMsgBuf);

			// inform about the message sent
			//DEBUGOUT("Message Sent!\r\n");
			//PrintCANMsg(&SendMsgBuf);

		}
	}
}
