/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "math.h"
#include "float.h"

uint16 voltage_measured, rpm, voltage, back_emf,current_1,current;
uint8 a;
float  back_emf_in_volts;
int dac;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_Start();
    PWM_Start();
    Comp_Start();
    Sample_Hold_Start();
    VDAC8_Start();
    PGA_Start();
    Clock_1_Start();
    Clock_2_Start();
    LCD_Start();
    
    
    LCD_Position(0,0);
    LCD_PrintString("Motor Loop Ctrl");
    CyDelay(1000);
    LCD_Position(0,0);
    LCD_ClearDisplay();
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    while(1)
    {
        /* Place your application code here. */
        for( dac=0; dac<=200;dac++)
        {
        VDAC8_SetValue(dac);
        
        CyDelay(100);
    
        ADC_StartConvert();
        a = ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        voltage_measured = ADC_GetResult16();
        voltage = ADC_CountsTo_mVolts(voltage_measured);
       
        current = voltage / 0.6;
        current_1= current;
        
       
        /* Display values on LCD*/
        LCD_Position(0,0);
        LCD_PrintString("I=    ");
        LCD_Position(0,2);
        LCD_PrintNumber(current);
        LCD_Position(0,3);
        LCD_PrintString(".");             
     
      
        // display value of DAC
        LCD_Position(1,0);
        LCD_PrintString("DAC=      ");
        LCD_Position(1,5);
        LCD_PrintNumber(dac);
        
        
       /* LCD_Position(1,8);
        LCD_PrintString("V=      ");
        LCD_Position(1,11);
        LCD_PrintNumber(a);*/
        
        
        }
        
        }
}

/* [] END OF FILE */