/*
 * FlashOperations_APP.c
 *
 *  Created on: Jul 16, 2023
 *      Author: TAISAT MARM-99
  ******************************************************************************
  * @attention
  *		This library will be updated in soon with many features
  *					**Will be added clear option that can clean selected addresess on the memory
  *					**Will be added choose program parallelism option for function's parameters 	(it's completed)
  *
  *					**Will be added FLASH_CheckBusy() function 							(it's completed)
  *					**Will be added sector & program size enumeration 					(it's completed)
  *					**Will be added FLASH_AccCntrl() funtion for some parameter			(it's completed)
  *					**Will be added FLASH status mask in soon and other masks will be wirtten
  *					**Will be added simple exam and test program
  ******************************************************************************
  */

/**Include header files */
#include "FlashOperations_APP.h"
#include "main.h"

//Kontrol edildi HAL ile uyumlu gözüküyor
void FLASH_CheckBusy(){
	/**
	 * This bit indicates that a Flash memory operation is in progress
	 * FLASH-SR bit is juct read from user. @arg 0 : no Flash memory operation ongoing
	 * 										@arg 1 : Flash memory operation ongoing
	 * Wait until BSY bit to be cleared
	 */
	while((FLASH->SR & FLASH_SR_BSY) != 0);
}


//Kontrol edildi HAL ile uyumlu gözüküyor
void FLASH_Unlocker(){

	/*! Wait until BSY bit to be cleared */
	FLASH_CheckBusy();

	/*! These keys bits are about unlocking the memory for Read/Write Operations */
	FLASH->KEYR = FLASH_KEY_1;
	FLASH->KEYR = FLASH_KEY_2;

	/*! Wait until BSY bit to be cleared */
	FLASH_CheckBusy();

}

//Kontrol edildi HAL ile uyumlu gözüküyor
void FLASH_Locker(){


	FLASH_CheckBusy();

	/**
	 *  Write to FLASH_CR_LOCK only. When it is set, this bit indicates that the FLASH_CR register is locked.
	 *  It is cleared by hardware after detecting the unlock sequence.
	 *
	 *  In the event of an unsuccessful unlock operation, this bit remains set until the next reset.
	 */
	FLASH->CR |= FLASH_CR_LOCK;

	/*! Wait until BSY bit to be cleared */
	FLASH_CheckBusy();

}



HAL_StatusTypeDef FLASH_Erase_Sec(uint32_t NumberOfSector){


	/*Unlock the flash memory*/
	FLASH_Unlocker();

	FLASH->ACR |= 3; //Congfig the Wait State (LATENCY) according to my supply voltage

	/*! Clear Parallelsize bits of CR Register */
	FLASH->CR &= (~FLASH_CR_PSize_Clr);
	/*! Config Program Size */
	FLASH->CR |= FLASH_PSIZE_HALF_WORD;

	/*! Clear Number of Sectors bits of CR Register */
	FLASH->CR &= (~FLASH_CR_SNb_Clr);

	FLASH->CR |= FLASH_CR_SER | (NumberOfSector << FLASH_CR_SNb_Pos);
	FLASH->CR |= FLASH_CR_STRT;
	FLASH_CheckBusy();

	FLASH_Locker();

return HAL_OK;

}


HAL_StatusTypeDef FLASH_Write(uint32_t SectorAddress , uint16_t UserData){



}

