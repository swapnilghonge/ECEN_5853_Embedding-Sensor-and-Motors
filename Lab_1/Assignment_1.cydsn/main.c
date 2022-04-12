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
#include "math.h."
#include "float.h"

uint32 loop_counter, LCD_write_counter, thermistor_R;
uint16 R_1_result, thermistor_result;
int16 thermistor_mV, R_1_mV, temperature;
float R_1_result_f, R_1_mV_f, thermistor_mV_f, ratio_f, voltage_f;
float thermistor_RA_f, thermistor_RB_f;
float current_mA_f, temperature_K_f;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_DelSig_1_Start();
    AMux_1_Start();
    
    LCD_Start();
    LCD_Position(0,0);
    
    LCD_PrintString("Thermistor Test ");
    CyDelay(1000);
    
    LCD_Position(0,0);
    
    while(1)
    {
        loop_counter++;
        LCD_write_counter++;
        P3_1_Write(1);
        
        AMux_1_Select(0);
        ADC_DelSig_1_StartConvert();
        
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
        
        
        P3_1_Write(0);
        
        R_1_result = ADC_DelSig_1_GetResult16();
        R_1_result_f = (float)R_1_result;
        
        R_1_mV = (int16)ADC_DelSig_1_CountsTo_mVolts(R_1_result);
        R_1_mV_f = (float)R_1_mV;
        
        current_mA_f = R_1_mV_f/10000.;
        
        AMux_1_Select(1);
       
        ADC_DelSig_1_StartConvert();
        
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
        
        thermistor_result = ADC_DelSig_1_GetResult16();
        
        thermistor_mV = (int16)ADC_DelSig_1_CountsTo_mVolts(thermistor_result);
        
        thermistor_mV_f = (float)thermistor_mV;
        
        thermistor_RA_f = thermistor_mV_f / current_mA_f;
        
        thermistor_R = Thermistor_1_GetResistance(R_1_mV, thermistor_mV);
        
        thermistor_RB_f = (float)thermistor_R;
        
        temperature = Thermistor_1_GetTemperature(thermistor_R);
        
        if(LCD_write_counter++ > 2500)
        {
            LCD_write_counter = 0;
            P1_7_Write(1);
            LCD_IsReady();
            LCD_Position(0,0);
            LCD_PrintString("loop cnt:     ");
            LCD_Position(0,10);
            LCD_PrintU32Number(loop_counter);
            LCD_Position(1,0);
            
            LCD_Position(1,0);
            LCD_PrintString("       degrees ");
            LCD_Position(1,0);
            LCD_PrintNumber(temperature/100);
            LCD_Position(1,2);
            LCD_PrintString(".");
            LCD_PrintNumber(temperature%100);
            
            P1_7_Write(0); // Oscilloscope
        }   
    }   
}

/* [] END OF FILE */
