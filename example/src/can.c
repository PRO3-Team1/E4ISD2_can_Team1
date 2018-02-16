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
#define LPC_CAN             (LPC_CAN1)
#define CAN_TX_MSG_STD_ID (0x15)
#define CAN_TX_MSG_EXT_ID (CAN_TX_MSG_STD_ID & CAN_EXTEND_ID_USAGE)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
static char WelcomeMenu[] =
		"\n\rHello NXP Semiconductors \r\n"
				"CAN DEMO : Use CAN to transmit and receive Message from CAN Analyzer\r\n"
				"CAN bit rate : 125kBit/s\r\n";

/*****************************************************************************
 * Private functions
 ****************************************************************************/
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
	SendMsgBuf.ID &= CAN_EXTEND_ID_USAGE;
	TxBuf = Chip_CAN_GetFreeTxBuf(LPC_CAN);
	Chip_CAN_Send(LPC_CAN, TxBuf, &SendMsgBuf);
	DEBUGOUT("Message Replied!!!\r\n");
	PrintCANMsg(&SendMsgBuf);
}
/*****************************************************************************
 * Public functions
 ****************************************************************************/
void CAN_IRQHandler(void) {
	uint32_t IntStatus;
	CAN_MSG_T RcvMsgBuf;
	IntStatus = Chip_CAN_GetIntStatus(LPC_CAN);

	PrintCANErrorInfo(IntStatus);

	/* New Message came */
	if (IntStatus & CAN_ICR_RI) {
		Chip_CAN_Receive(LPC_CAN, &RcvMsgBuf);
		DEBUGOUT("Message Received!\r\n");
		PrintCANMsg(&RcvMsgBuf);

		if (RcvMsgBuf.ID & CAN_EXTEND_ID_USAGE) {
			DEBUGOUT("Ignored. Message was extended ID!!!\r\n");
		} else {
			DEBUGOUT("Replied. Message was STANDARD ID!!!\r\n");
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

	/* use pins 0.0 and 0.1 */
	Board_CAN_Init(LPC_CAN);
	//LPC_CANAF & LPC_CANAF_RAM is used to initialize acceptence
	Chip_CAN_Init(LPC_CAN, LPC_CANAF, LPC_CANAF_RAM);
	Chip_CAN_SetBitRate(LPC_CAN, 250000);
	Chip_CAN_EnableInt(LPC_CAN, CAN_IER_BITMASK);
	/* set acceptance filters to bypass */
	Chip_CAN_SetAFMode(LPC_CANAF, CAN_AF_BYBASS_MODE);
	/* enable interrupts */
	NVIC_EnableIRQ(CAN_IRQn);

	/* fill data in TX buffer */
	//ID svarer til den dag Mikkel er født på året.
	SendMsgBuf.ID = 15; //CAN_TX_MSG_STD_ID;
	SendMsgBuf.DLC = 4;
	SendMsgBuf.Type = 0;
	SendMsgBuf.Data[0] = 'A';
	SendMsgBuf.Data[1] = 'B';
	SendMsgBuf.Data[2] = 'C';
	SendMsgBuf.Data[3] = 'D';

	while (1) {
		//implement a button with delay.

		if (!button_get(0)) {
			/* get free TX buffer no */
			TxBuf = Chip_CAN_GetFreeTxBuf(LPC_CAN);
			/* and transmit data */
			Chip_CAN_Send(LPC_CAN, TxBuf, &SendMsgBuf);
			//wait for TX buffer empty
			while ((Chip_CAN_GetStatus(LPC_CAN) & CAN_SR_TCS(TxBuf)) == 0) {
			}

			/* inform about the message sent */
			DEBUGOUT("Message Sent!\r\n");
			PrintCANMsg(&SendMsgBuf);
		}
	}
}
