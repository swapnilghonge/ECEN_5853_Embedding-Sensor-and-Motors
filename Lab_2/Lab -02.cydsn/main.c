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

int16 voltage_measured, rpm, voltage, back_emf,current_1;
float  back_emf_in_volts,current;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_SAR_1_Start();
    PGA_1_Start();
    LCD_Start();
    LCD_Position(0,0);
    LCD_PrintString("Back EMF Test");
    CyDelay(2000);
    LCD_Position(0,0);
    LCD_ClearDisplay();
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    while(1)
    {
        /* Place your application code here. */
        
        P3_5_Write(1); //gate FET high
        
        CyDelay(500);
        
        P3_5_Write(0); // gate FET low
        
        
        ADC_SAR_1_StartConvert();
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        voltage_measured = ADC_SAR_1_GetResult16();
        voltage = ADC_SAR_1_CountsTo_mVolts(voltage_measured);
        
        /* Formulae used:
        Ke = rpm/back_emf
        V= (Istall- In/l)/R values refered from datasheet of the motor
         = (1.02-012)*5
            = 4 Volts
        Ke= 11000/4 =  2750*/
        
        back_emf = 450 - voltage;
        back_emf_in_volts = (float)back_emf/100.0;
        rpm = 2623 * back_emf_in_volts;
        int rpm_1 =rpm;
        int16 voltage_motor = 450 - voltage;
        current = (float)voltage_motor / 5;
        current_1= current;
        
        P3_6_Write(1);
        /* Display values on LCD*/
        LCD_Position(0,0);
        LCD_PrintString("Vb=    ");
        LCD_Position(0,2);
        LCD_PrintNumber(back_emf/100);
        LCD_Position(0,3);
        LCD_PrintString(".");             
        LCD_PrintNumber(back_emf%100);
      
        // display value of rpm
        LCD_Position(1,0);
        LCD_PrintString("RPM=      ");
        LCD_Position(1,5);
        LCD_PrintNumber(rpm_1);
        
        
        //display value of Current
        LCD_Position(0,7);
        LCD_PrintString("I=  mA    ");
        LCD_Position(0,9);
        LCD_PrintNumber(current_1);
        LCD_Position(0,11);
        LCD_PrintString(".");
        LCD_PrintNumber(current_1%100);
        P3_6_Write(0);
    }
}

/* [] END OF FILE */