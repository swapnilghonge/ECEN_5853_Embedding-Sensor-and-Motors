/*******************************************************************************
* File Name: DAC_3.c  
* Version 1.90
*
* Description:
*  This file provides the source code to the API for the 8-bit Voltage DAC 
*  (VDAC8) User Module.
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "DAC_3.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 DAC_3_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 DAC_3_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static DAC_3_backupStruct DAC_3_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: DAC_3_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_Init(void) 
{
    DAC_3_CR0 = (DAC_3_MODE_V );

    /* Set default data source */
    #if(DAC_3_DEFAULT_DATA_SRC != 0 )
        DAC_3_CR1 = (DAC_3_DEFAULT_CNTL | DAC_3_DACBUS_ENABLE) ;
    #else
        DAC_3_CR1 = (DAC_3_DEFAULT_CNTL | DAC_3_DACBUS_DISABLE) ;
    #endif /* (DAC_3_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(DAC_3_DEFAULT_STRB != 0)
        DAC_3_Strobe |= DAC_3_STRB_EN ;
    #endif/* (DAC_3_DEFAULT_STRB != 0) */

    /* Set default range */
    DAC_3_SetRange(DAC_3_DEFAULT_RANGE); 

    /* Set default speed */
    DAC_3_SetSpeed(DAC_3_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: DAC_3_Enable
********************************************************************************
* Summary:
*  Enable the VDAC8
* 
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_Enable(void) 
{
    DAC_3_PWRMGR |= DAC_3_ACT_PWR_EN;
    DAC_3_STBY_PWRMGR |= DAC_3_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(DAC_3_restoreVal == 1u) 
        {
             DAC_3_CR0 = DAC_3_backup.data_value;
             DAC_3_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: DAC_3_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:
*  Power: Sets power level between off (0) and (3) high power
*
* Return:
*  void 
*
* Global variables:
*  DAC_3_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void DAC_3_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(DAC_3_initVar == 0u)
    { 
        DAC_3_Init();
        DAC_3_initVar = 1u;
    }

    /* Enable power to DAC */
    DAC_3_Enable();

    /* Set default value */
    DAC_3_SetValue(DAC_3_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: DAC_3_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_Stop(void) 
{
    /* Disble power to DAC */
    DAC_3_PWRMGR &= (uint8)(~DAC_3_ACT_PWR_EN);
    DAC_3_STBY_PWRMGR &= (uint8)(~DAC_3_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        DAC_3_backup.data_value = DAC_3_CR0;
        DAC_3_CR0 = DAC_3_CUR_MODE_OUT_OFF;
        DAC_3_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: DAC_3_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    DAC_3_CR0 &= (uint8)(~DAC_3_HS_MASK);
    DAC_3_CR0 |=  (speed & DAC_3_HS_MASK);
}


/*******************************************************************************
* Function Name: DAC_3_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:
*  Range: Sets one of Three valid ranges.
*
* Return:
*  void 
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_SetRange(uint8 range) 
{
    DAC_3_CR0 &= (uint8)(~DAC_3_RANGE_MASK);      /* Clear existing mode */
    DAC_3_CR0 |= (range & DAC_3_RANGE_MASK);      /*  Set Range  */
    DAC_3_DacTrim();
}


/*******************************************************************************
* Function Name: DAC_3_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  void 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 DAC_3_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    DAC_3_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        DAC_3_Data = value;
        CyExitCriticalSection(DAC_3_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: DAC_3_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  range:  1V or 4V range.  See constants.
*
* Return:
*  void
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void DAC_3_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((DAC_3_CR0 & DAC_3_RANGE_MASK) >> 2) + DAC_3_TRIM_M7_1V_RNG_OFFSET;
    DAC_3_TR = CY_GET_XTND_REG8((uint8 *)(DAC_3_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
