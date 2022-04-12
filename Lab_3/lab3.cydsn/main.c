3/* ========================================
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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
	ADC_SAR_1_Start();
	LCD_Char_1_Start();
	uint16 pot_raw, mV, angle;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    while(1)
    {
        /* Place your application code here. */
        ADC_SAR_1_StartConvert();
		ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        //CyDelay(100);
		pot_raw = ADC_SAR_1_GetResult16();
       // CyDelay(100);
		mV= ADC_SAR_1_CountsTo_mVolts(pot_raw);
		angle = (mV*270)/5000;
        
		LCD_Char_1_ClearDisplay();
        LCD_Char_1_Position(1,0);
		LCD_Char_1_PrintString("PotmV:   ");     
		LCD_Char_1_Position(1,7);
		LCD_Char_1_PrintNumber(mV);
		LCD_Char_1_Position(0,0);
		LCD_Char_1_PrintString("Angle:   ");     
		LCD_Char_1_Position(0,7);
		LCD_Char_1_PrintNumber(angle);
		CyDelay(100);
    }
}

/* [] END OF FILE */
