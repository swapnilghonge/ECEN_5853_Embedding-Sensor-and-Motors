/*******************************************************************************
* File Name: DAC_1_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DAC_1.h"

static DAC_1_backupStruct DAC_1_backup;


/*******************************************************************************
* Function Name: DAC_1_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void DAC_1_SaveConfig(void) 
{
    if (!((DAC_1_CR1 & DAC_1_SRC_MASK) == DAC_1_SRC_UDB))
    {
        DAC_1_backup.data_value = DAC_1_Data;
    }
}


/*******************************************************************************
* Function Name: DAC_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void DAC_1_RestoreConfig(void) 
{
    if (!((DAC_1_CR1 & DAC_1_SRC_MASK) == DAC_1_SRC_UDB))
    {
        if((DAC_1_Strobe & DAC_1_STRB_MASK) == DAC_1_STRB_EN)
        {
            DAC_1_Strobe &= (uint8)(~DAC_1_STRB_MASK);
            DAC_1_Data = DAC_1_backup.data_value;
            DAC_1_Strobe |= DAC_1_STRB_EN;
        }
        else
        {
            DAC_1_Data = DAC_1_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: DAC_1_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  DAC_1_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void DAC_1_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(DAC_1_ACT_PWR_EN == (DAC_1_PWRMGR & DAC_1_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        DAC_1_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        DAC_1_backup.enableState = 0u;
    }
    
    DAC_1_Stop();
    DAC_1_SaveConfig();
}


/*******************************************************************************
* Function Name: DAC_1_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  DAC_1_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DAC_1_Wakeup(void) 
{
    DAC_1_RestoreConfig();
    
    if(DAC_1_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        DAC_1_Enable();

        /* Restore the data register */
        DAC_1_SetValue(DAC_1_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
