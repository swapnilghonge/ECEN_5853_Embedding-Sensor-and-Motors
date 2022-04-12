/*******************************************************************************
* File Name: DAC_2_PM.c  
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

#include "DAC_2.h"

static DAC_2_backupStruct DAC_2_backup;


/*******************************************************************************
* Function Name: DAC_2_SaveConfig
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
void DAC_2_SaveConfig(void) 
{
    if (!((DAC_2_CR1 & DAC_2_SRC_MASK) == DAC_2_SRC_UDB))
    {
        DAC_2_backup.data_value = DAC_2_Data;
    }
}


/*******************************************************************************
* Function Name: DAC_2_RestoreConfig
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
void DAC_2_RestoreConfig(void) 
{
    if (!((DAC_2_CR1 & DAC_2_SRC_MASK) == DAC_2_SRC_UDB))
    {
        if((DAC_2_Strobe & DAC_2_STRB_MASK) == DAC_2_STRB_EN)
        {
            DAC_2_Strobe &= (uint8)(~DAC_2_STRB_MASK);
            DAC_2_Data = DAC_2_backup.data_value;
            DAC_2_Strobe |= DAC_2_STRB_EN;
        }
        else
        {
            DAC_2_Data = DAC_2_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: DAC_2_Sleep
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
*  DAC_2_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void DAC_2_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(DAC_2_ACT_PWR_EN == (DAC_2_PWRMGR & DAC_2_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        DAC_2_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        DAC_2_backup.enableState = 0u;
    }
    
    DAC_2_Stop();
    DAC_2_SaveConfig();
}


/*******************************************************************************
* Function Name: DAC_2_Wakeup
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
*  DAC_2_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DAC_2_Wakeup(void) 
{
    DAC_2_RestoreConfig();
    
    if(DAC_2_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        DAC_2_Enable();

        /* Restore the data register */
        DAC_2_SetValue(DAC_2_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
