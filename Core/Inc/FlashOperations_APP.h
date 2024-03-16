/*
 * FlashOperations_APP.h
 *
 *  Created on: Jul 16, 2023
 *      Author: TAISAT MARM-99
 */

#ifndef INC_FLASHOPERATIONS_APP_H_
#define INC_FLASHOPERATIONS_APP_H_

/**Include header files*/
#include "main.h"

/**Define constant variables*/	// Buralar daha modüler hale getirilecek  bir flash_handle strcut ile
#define FLASH_KEY_1  0x45670123U
#define FLASH_KEY_2  0xCDEF89ABU


/*! Clear Parallel Size value in the FLASH_CR register*/
#define FLASH_CR_PSize_Pos			   (8U)
#define FLASH_CR_PSize_Msk			   (0x3UL << FLASH_CR_PSize_Pos)
#define FLASH_CR_PSize_Clr			   (FLASH_CR_PSize_Msk)

/*! Clear Sector Number value in the FLASH_CR register*/
#define FLASH_CR_SNb_Pos				(3U)
#define FLASH_CR_SNb_Msk				(0xFUL << FLASH_CR_SNb_Pos)
#define FLASH_CR_SNb_Clr				FLASH_CR_SNb_Msk

/**Identify Functions Prototype*/




/**
 * @fn     void FLASH_CheckBusy();
 * @brief This function stops the other flash memory operations when one of them is operating
 * @param  None
 * @retval None
 */
void FLASH_CheckBusy();

/**
 * @fn     void FLASH_Locker()
 * @brief  This function to be lock the flash memory
 * @param  None
 * @retval None
 */
void FLASH_Locker();


/**
 * @fn     void FLASH_Unlocker()	//Burası init taarafında kullanılacak hatta flash a ait tek bir init fonksiyou yazılacak
 * @brief  This function to be unlock the flash memory for read/write operations
 * @param  None
 * @retval None
 */
void FLASH_Unlocker();


/**
 * @fn     HAL_StatusTypeDef FLASH_Erase(uint8_t NumberOfSector, char EraseMode)
 * @brief  This function to be erase flash memory
 * @param[in]   NumberOfSector : It's the sector number that is used
 * @param[out]  EraseMode 	  : It's the mode of Erase Type
 * 									**For "Mass Erase" it's written "M"
 * 									**For "Sector Erase" it's written "S"
 * @retval HAL Status
 */
HAL_StatusTypeDef FLASH_Erase(uint32_t  NumberOfSector, char EraseMode);


/**
 * @fn     void FLASH_Write(uint32_t Address , uint32_t Data)
 * @brief  This function to be written datas in Flash Memory's sectors
 * @param[in]  Address    : It's the sector's address that is used
 * @param[in]  Data 	  : It's the data that is sent by user
 * @param[in]  Psize 	  : These bits select the program parallelism
 * 											Examples:
 * 											*"0" 8 Byte Operations
 * 											*"1" 16 Byte Operations
 * 											*"2" 32 Byte Operations
 * 											*"3" 64 Byte Operations
 * @retval HAL Status
 */
HAL_StatusTypeDef FLASH_Write(uint32_t SectorAddress , uint16_t UserData);



/**
 * @fn     uint32_t FLASH_Read(uint32_t Address)
 * @brief  This function to be read datas from Address of sector of Flash Memory
 * @param[in]  Address : It's the sector's address that is used
 *
 * @retval ReceivedData
 */
uint32_t FLASH_Read(uint32_t Address);


#endif /* INC_FLASHOPERATIONS_APP_H_ */



