/*******************************************************************************
* File Name: Pin0_0.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin0_0_ALIASES_H) /* Pins Pin0_0_ALIASES_H */
#define CY_PINS_Pin0_0_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Pin0_0_0			(Pin0_0__0__PC)
#define Pin0_0_0_INTR	((uint16)((uint16)0x0001u << Pin0_0__0__SHIFT))

#define Pin0_0_INTR_ALL	 ((uint16)(Pin0_0_0_INTR))

#endif /* End Pins Pin0_0_ALIASES_H */


/* [] END OF FILE */
