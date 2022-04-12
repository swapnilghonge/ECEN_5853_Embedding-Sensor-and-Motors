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
#include "stdio.h"


#define SAMPLE_LIMIT 250

uint8 current_1 = 50, source = 0;
uint32 adc_wait_count, last_adc_wait_count, test_mode = 0;
uint32 button_not_pushed_count;
uint16 adc_result;
uint16 adc_limit = 4000;
int sample_count = 0;

char TransmitBuffer[16];

//Variables forPID Control
uint8 Vmeas_raw, Vmeas_buffer[SAMPLE_LIMIT];
uint32 Vmeas_buffer_index = 0, buffer_limit_reached;
int32 Vref = 90, Vmeas, Vmeas_last, Vmeas_delta, Verror, Vout, Vout_limited;
int32 Verror_sum, Verror_sum_limit = 10000;
int32 Kp = 120, Kd = -500, Ki = 0;
int32 Kp_term, Kd_term, Ki_term, FF_term = 95;
int32 Kp_term_limited, Kd_term_limited, Ki_term_limited;
uint32 Kp_term_overflow_count, Kp_term_underflow_count, loop_count;
uint32 Kd_term_overflow_count, Kd_term_underflow_count;
uint32 Kd_display_gain = 8;
uint32 Ki_term_overflow_count, Ki_term_underflow_count;
uint32 Vout_term_underflow_count, Vout_term_overflow_count;
int32 Kp_scale_factor = 128, Kd_scale_factor = 128, Ki_scale_factor = 1024;
int32 feedforward_only = 0;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_Start(); 
    ADC_StartConvert();
    DAC_1_Start(); DAC_2_Start(); DAC_3_Start();
    Opamp_1_Start(); Opamp_2_Start(); Opamp_3_Start(); Opamp_4_Start();
    //UART_Start();

    //UART_PutString("\r\n\n\n\n\n\n\n");
    //UART_PutString("\rXXXXXXXX\r\n\n");
   // CyDelay(500);
    //UART_PutString("PID_Lab\n\n\r");

    /*P1_6_Write(1);
    P1_6_Write(0);
    P1_6_Write(1);
    P1_6_Write(0);*/

    
    while(1)
    {
        /* Place your application code here. */
     loop_count++;
    Vmeas_last = Vmeas;
    adc_wait_count = 0;
   // P1_6_Write(1);
    ADC_StartConvert();

    while(ADC_IsEndConversion(ADC_RETURN_STATUS) == 0)
    adc_wait_count++;

    //P1_6_Write(0);
    last_adc_wait_count = adc_wait_count;

    Vmeas_raw = (int32)ADC_GetResult8();
    DAC_2_SetValue(127 + Vmeas_raw);

    Vmeas_buffer[loop_count] = Vmeas_raw;

    Vmeas = (uint32)Vmeas_raw;

    if(loop_count == 1)
    {
        Vmeas_delta = 0;
    }
    else
    {
        Vmeas_delta = Vmeas - Vmeas_last;
    }
	

    DAC_3_SetValue(127 + Kd_display_gain * Vmeas_delta);

    Kd_term = Kd * Vmeas_delta / Kd_scale_factor;

    Kd_term_limited = Kd_term;
    if(Kd_term_limited > 127)
    {
        Kd_term_limited = 127; 
        Kd_term_overflow_count++;
    }
    if(Kd_term_limited < -127) 
    {
        Kd_term_limited = -127; 
        Kd_term_underflow_count++;
    }

    Verror = Vref - Vmeas;

    Kp_term = Kp * Verror / Kp_scale_factor;

    Kp_term_limited = Kp_term;
    if(Kp_term_limited > 127) 
    {
        Kp_term_limited = 127;
        Kp_term_overflow_count++;
    }
    if(Kp_term_limited < -127) 
    {
        Kp_term_limited = -127; 
        Kp_term_underflow_count++;
    }
    Verror_sum += Verror;

    if(Verror_sum > Verror_sum_limit) 
    {
        Verror_sum = Verror_sum_limit; 
        Ki_term_overflow_count++;
    }
    if(Verror_sum < -Verror_sum_limit)
    {
        Verror_sum = -Verror_sum_limit;
        Ki_term_underflow_count++;
    }

    Ki_term = Ki * Verror_sum / Ki_scale_factor;

    if(feedforward_only == 1)
    {
        Vout = FF_term;
    }
    else
    {
        Vout = FF_term + Kp_term + Ki_term + Kd_term;
    }

    Vout_limited = Vout;
    if(Vout_limited > 255)
    {
        Vout_limited = 255; 
        Vout_term_overflow_count++;
    }
    
    if(Vout_limited < 0) 
    {
        Vout_limited = 0;
        Vout_term_underflow_count++;
    }

    DAC_1_SetValue(Vout_limited);

    if(loop_count < SAMPLE_LIMIT)

    {
    }
    if(loop_count >= SAMPLE_LIMIT)
    {
    loop_count = 0;
   // P12_3_Write(0);

    DAC_1_SetValue(0);

    /*CyDelay(100);
    UART_PutString("\r\n    Index     Vcap \n\r");
    UART_PutString("    ____    ____  \n\n\r");*/

    Vmeas_buffer_index = 0;

    while(Vmeas_buffer_index < SAMPLE_LIMIT)
    {
    sprintf(TransmitBuffer, "    %d     %d \r\n",
    	(uint16) Vmeas_buffer_index, Vmeas_buffer[Vmeas_buffer_index]);

    //UART_PutString(TransmitBuffer);
    Vmeas_buffer_index++;
    }
    /*UART_PutString("\r\n\nTest Complete.\n\n");
    UART_PutString("\r************\n\n\n\n\n");*/
    loop_count = 0;
    }
    }
}

/* [] END OF FILE */
