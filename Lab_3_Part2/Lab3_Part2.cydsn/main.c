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

// global variable declaration



int LastState=0, State, State, count, count = 0; // intitializing counter to 0

int main(void)

{
    
    
    CyGlobalIntEnable; // Enable Global interrupts
    

    QuadDec_2_Start();
    Clock_2_Start();
    LCD_Char_2_Start();
    LCD_Char_2_Position(0,0);
    LCD_Char_2_PrintString("Encoder Test");
    LCD_Char_2_Position(0,0);
    CyDelay(3000);
    LCD_Char_2_ClearDisplay();
    
    while(1)
    {   
        count = QuadDec_2_GetCounter();
        
        State = P3_0_Read();
        State = P3_1_Read();   
        
        if(State != LastState)// checking the state wrt last state
        {
            if( State != State)
            {
                count = count - 1; // clockwise
                
            }else {
                count = count + 1; //counterclockwise
                
            }
        }
        LastState = State;

        int counter = count;

        if (counter< 0){
            counter = counter/2.0; // diving by 2
            counter = 65537 - counter; // substracting from the max value
            LCD_Char_2_Position(1,0);
            LCD_Char_2_PrintString("Software = -");
            LCD_Char_2_Position(1,12);
            LCD_Char_2_PrintNumber(counter);
        }else{
            counter = counter/2.0; //diving by 2
            LCD_Char_2_Position(1,0);
            LCD_Char_2_PrintString("Software =   ");
            LCD_Char_2_Position(1,11);
            LCD_Char_2_PrintNumber(counter);
        
        }
        
        if (count >= 0){
            LCD_Char_2_Position(0,0); // for hardware
            LCD_Char_2_PrintString("Hardware =   ");
            LCD_Char_2_Position(0,11);
            LCD_Char_2_PrintNumber(count);
           
        }else{
            count = 65536 - count; // for hardware
            LCD_Char_2_Position(0,0);
            LCD_Char_2_PrintString("Hardware = -");
            LCD_Char_2_Position(0,12);
            LCD_Char_2_PrintNumber(count);
             
        }    
    }
}
/* [] END OF FILE */
