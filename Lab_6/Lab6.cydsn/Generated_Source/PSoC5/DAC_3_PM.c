/*******************************************************************************
* File Name: DAC_3_PM.c  
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

#include "DAC_3.h"

static DAC_3_backupStruct DAC_3_backup;


/*******************************************************************************
* Function Name: DAC_3_SaveConfig
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
void DAC_3_SaveConfig(void) 
{
    if (!((DAC_3_CR1 & DAC_3_SRC_MASK) == DAC_3_SRC_UDB))
    {
        DAC_3_backup.data_value = DAC_3_Data;
    }
}


/*******************************************************************************
* Function Name: DAC_3_RestoreConfig
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
void DAC_3_RestoreConfig(void) 
{
    if (!((DAC_3_CR1 & DAC_3_SRC_MASK) == DAC_3_SRC_UDB))
    {
        if((DAC_3_Strobe & DAC_3_STRB_MASK) == DAC_3_STRB_EN)
        {
            DAC_3_Strobe &= (uint8)(~DAC_3_STRB_MASK);
            DAC_3_Data = DAC_3_backup.data_value;
            DAC_3_Strobe |= DAC_3_STRB_EN;
        }
        else
        {
            DAC_3_Data = DAC_3_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: DAC_3_Sleep
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
*  DAC_3_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void DAC_3_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(DAC_3_ACT_PWR_EN == (DAC_3_PWRMGR & DAC_3_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        DAC_3_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        DAC_3_backup.enableState = 0u;
    }
    
    DAC_3_Stop();
    DAC_3_SaveConfig();
}


/*******************************************************************************
* Function Name: DAC_3_Wakeup
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
*  DAC_3_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DAC_3_Wakeup(void) 
{
    DAC_3_RestoreConfig();
    
    if(DAC_3_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        DAC_3_Enable();

        /* Restore the data register */
        DAC_3_SetValue(DAC_3_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
