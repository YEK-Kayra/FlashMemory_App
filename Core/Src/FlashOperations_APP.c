/*
 * FlashOperations_APP.c
 *
 *  Created on: Jul 16, 2023
 *      Author: TAISAT MARM-99
  ******************************************************************************
  * @attention
  *		This library will be updated in soon with many features
  *					**Will be added clear option that can clean selected addresess on the memory
  *					**Will be added choose program parallelism option for function's parameters
  *					**Will be added FLASH_CheckBusy() function
  *					**Will be added sector & program size enumeration
  *					**Will be added FLASH_AccCntrl() funtion for some parameter
  ******************************************************************************
  */

/**Include header files */
#include "FlashOperations_APP.h"
#include "main.h"


/**
 * @fn     void FLASH_Unlocker()
 * @brief  This function to be unlock the flash memory for read/write operations
 * @param  None
 * @retval None
 */
void FLASH_Unlocker(){

	/**
	 * This bit indicates that a Flash memory operation is in progress
	 * FLASH-SR bit is juct read from user. @arg 0 : no Flash memory operation ongoing
	 * 										@arg 1 : Flash memory operation ongoing
	 * Wait until BSY bit to be cleared
	 */
	while((FLASH->SR & 0x00010000) != 0);

	/*! These keys bits are about unlocking the memory for Read/Write Operations */
	FLASH->KEYR = FLASH_KEY_1;
	FLASH->KEYR = FLASH_KEY_2;

	/*! Wait until BSY bit to be cleared */
	while((FLASH->SR & 0x00010000) != 0);

}

/**
 * @fn     void FLASH_Locker()
 * @brief  This function to be lock the flash memory
 * @param  None
 * @retval None
 */
void FLASH_Locker(){

	/**
	 * This bit indicates that a Flash memory operation is in progress
	 * FLASH-SR bit is juct read from user. @arg 0 : no Flash memory operation ongoing
	 * 										@arg 1 : Flash memory operation ongoing
	 * Wait until BSY bit to be cleared
	 */
	while((FLASH->SR & 0x00010000) != 0);  //Wait until BSY bit to be cleared

	/**
	 *  Write to 1 only. When it is set, this bit indicates that the FLASH_CR register is locked.
	 *  It is cleared by hardware after detecting the unlock sequence.
	 *
	 *  In the event of an unsuccessful unlock operation, this bit remains set until the next reset.
	 */
	FLASH->CR |= 1<<31;

	/*! Wait until BSY bit to be cleared */
	while((FLASH->SR & 0x00010000) != 0);

}

/**
 * @fn     HAL_StatusTypeDef FLASH_Erase(uint8_t NumberOfSector, char EraseMode)
 * @brief  This function to be erase flash memory
 * @param[in]   NumberOfSector : It's the sector number that is used
 * @param[in]   EraseMode 	   : It's the mode of Erase Type
 *                                  @arg : "Mass Erase" it's written "M"
 *                                  @arg : "Sector Erase" it's written "S"
 *
 * @attention  If MERx and SER bits are both set in the FLASH_CR register, mass erase is performed.
 * @attention  If both MERx and SER bits are reset and the STRT bit is set, an unpredictable behavior may
			   occur without generating any error flag. This condition should be forbidden.

 * @retval HAL Status
 */
HAL_StatusTypeDef FLASH_Erase(uint8_t NumberOfSector, char EraseMode){

	while((FLASH->SR & 0x00010000) != 0);  //Wait until BSY bit to be cleared
	FLASH_Unlocker();

	if(EraseMode == 'M'){
		FLASH->CR |= 1<<2; 			  			//Erase activated for all user sectors(MER bit is set)
		FLASH->CR |= 1<<16;           			//This bit triggers to start an erase operation(STRT bit is set)
		while((FLASH->SR & 0x00010000) != 0); 	//Wait for the BSY bit to be cleared
		FLASH->CR &= 0<<1; 			  			//Sector erase de-activated
	}
	else if(EraseMode == 'S'){
		FLASH->CR |= 1<<1;    			  //Sector erase activated
		FLASH->CR |= NumberOfSector<<3;	  //These bits select the sector to erase
		FLASH->CR |= 1<<16;               //This bit triggers to start an erase operation
		FLASH->CR &= 0<<1; 				  //Sector erase de-activated
	}
	else{
		return HAL_ERROR;
	}

	return HAL_OK;


}

/**
 * @fn     void FLASH_Write(uint32_t SectorAddress , uint32_t UserData, uint8_t Psize)
 * @brief  This function to be written datas in Address of sector of Flash Memory
 * @param[in]  Address    : It's the sector's address that is used
 * @param[in]  Data 	  : It's the data that is sent by user
 * @param[in]  Psize 	  : These bits select the program parallelism
 * 						     This parameter can be one of the following values:
 * 											@arg "0" 8 Byte  Operations
 * 											@arg "1" 16 Byte Operations
 * 											@arg "2" 32 Byte Operations
 * 											@arg "3" 64 Byte Operations
 *
 * @attention : If an erase and a program operation are requested simultaneously, the erase operation is
 *			    performed first.
 *
 * @retval HAL Status
 */
HAL_StatusTypeDef FLASH_Write(uint32_t SectorAddress , uint16_t UserData, uint8_t Psize){

	FLASH_Unlocker();

	while((FLASH->SR & 0x00010000) != 0);    	//Wait until BSY bit to be cleared

	FLASH->CR |= 1<<0;                       	//Flash programming activated(PG bit is set)
	FLASH->CR |= Psize<<8;

	*(__IO uint32_t*)SectorAddress = UserData;	//Sending data to the Address

	while((FLASH->SR & 0x00010000) != 0);

	FLASH_Locker();

	return HAL_OK;

}

/**
 * @fn     uint32_t FLASH_Read(uint32_t Address)
 * @brief  This function to be read datas from Address of sector of Flash Memory
 * @param[in]  Address : It's the sector's address that is used
 *
 * @retval ReceivedData
 */
uint32_t FLASH_Read(uint32_t Address){

	uint32_t ReceivedData;

	FLASH_Unlocker();

	ReceivedData = *(uint32_t *)Address;

	FLASH_Locker();

	return ReceivedData;

}
