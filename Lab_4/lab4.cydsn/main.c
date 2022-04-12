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
#include "stdio.h"
#define USBFS_DEVICE (0u)
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_Start();
    PGA_Start();
    LCD_Start();
    VDAC_Start();
    Opamp_Start();
    
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    char string[100];
    char string_1[100];

    while(1)
    {
        ADC_StartConvert();//ADC start conversion
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);//waiting for result
        int adc = ADC_GetResult16();//start conversion
        int adc_mv = ADC_CountsTo_mVolts(adc);//count value to millivolts
        int grams = adc_mv * 2/3;//values in grams
        

        LCD_Position(0,0);
        LCD_PrintString("Grams:   ");
        LCD_Position(0,7);
        LCD_PrintNumber(grams);
	
        if(0u != USBUART_GetConfiguration()){//USB UART configuration
            sprintf(string_1, "Grams = %d  \r\n", grams);
            USBUART_PutString(string_1);
            printf("\n\r");
             sprintf(string, "mV = %d  \r\n", adc_mv);
            USBUART_PutString(string);
        }
         LCD_Position(1,0);
       // LCD_PrintNumber(adc_mv);
        LCD_PrintString("mV:   ");
        LCD_Position(1,4);
        LCD_PrintNumber(adc_mv);
        CyDelay(100);
        LCD_ClearDisplay();
        
    }
}
