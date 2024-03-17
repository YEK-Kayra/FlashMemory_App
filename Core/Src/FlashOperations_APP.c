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



HAL_StatusTypeDef FLASH_Erase(uint32_t NumberOfSector, char EraseMode){

	/*Unlock the flash memory*/
	FLASH_Unlocker();

	//FLASH_Erase_Sector(Sector, VoltageRange)

	  	  /*
		  *	     These bits select the program parallelism.
		  *		 @param  VoltageRange The device voltage range which defines the erase parallelism.
		  *
		  *            This parameter can be one of the following values:
		  *            @arg FLASH_VOLTAGE_RANGE_1: when the device voltage range is 1.8V to 2.1V,
		  *                                  the operation will be done by byte (8-bit)
		  *            @arg FLASH_VOLTAGE_RANGE_2: when the device voltage range is 2.1V to 2.7V,
		  *                                  the operation will be done by half word (16-bit)
		  *            @arg FLASH_VOLTAGE_RANGE_3: when the device voltage range is 2.7V to 3.6V,
		  *                                  the operation will be done by word (32-bit)
		  *            @arg FLASH_VOLTAGE_RANGE_4: when the device voltage range is 2.7V to 3.6V + External Vpp,
		  *                                  the operation will be done by double word (64-bit)
		  */

		  /*! Clear Parallelsize bits of CR Register */
		  	  FLASH->CR &=(~FLASH_CR_PSize_Clr);
	 	  /*! Config Program Size */
			  FLASH->CR |= FLASH_VOLTAGE_RANGE_3;

		  /*! Clear Number of Sectors bits of CR Register */
		  FLASH->CR &= (~FLASH_CR_SNb_Clr);

	  FLASH->CR |= FLASH_CR_SER | (NumberOfSector << FLASH_CR_SNb_Pos);
	  FLASH->CR |= FLASH_CR_STRT;

  FLASH_Locker();

return HAL_OK;

}


HAL_StatusTypeDef FLASH_Write(uint32_t SectorAddress , uint16_t UserData){

	FLASH_Unlocker();
	FLASH_CheckBusy();
	FLASH->CR &= (~FLASH_CR_PSize_Clr); 			//Clear Parallelsize bits of CR Register.
	FLASH->CR |= FLASH_PSIZE_HALF_WORD;
	FLASH->CR |= FLASH_CR_PG;
	*(__IO uint16_t*)SectorAddress = UserData;		//Sending data to the Address
	 /* If the program operation is completed, disable the PG Bit */
	FLASH->CR &= (~FLASH_CR_PG);
	FLASH_CheckBusy();
	FLASH_Locker();

	return HAL_OK;

}

