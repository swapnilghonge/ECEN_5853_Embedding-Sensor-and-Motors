/*******************************************************************************
* File Name: Thermistor_1.c
* Version 1.20
*
* Description:
*  This file provides the source code to the API for the ThermistorCalc
*  Component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Thermistor_1.h"


/*******************************************************************************
* Function Name: Thermistor_1_GetResistance
********************************************************************************
*
* Summary:
*  The digital values of the voltages across the reference resistor and the
*  thermistor are passed to this function as parameters. The function returns
*  the resistance, based on the voltage values.
*
* Parameters:
*  vReference: the voltage across the reference resistor;
*  vThermistor: the voltage across the thermistor.
*  The ratio of these two voltages is used by this function. Therefore, the
*  units for both parameters must be the same.
*
* Return:
*  The return value is the resistance across the thermistor. The value returned
*  is the resistance in Ohms.
*
*******************************************************************************/
uint32 Thermistor_1_GetResistance(int16 vReference, int16 vThermistor)
                                      
{
    int32 resT;
    int16 temp;

    /* Calculate thermistor resistance from the voltages */
    resT = Thermistor_1_REF_RESISTOR * ((int32)vThermistor);
    if (vReference < 0)
    {
        temp = -vReference;
        temp = (int16)((uint16)((uint16)temp >> 1u));
        temp = -temp;
    }
    else
    {
        temp = (int16)((uint16)((uint16)vReference >> 1u));
    }
    resT += temp;
    resT /= vReference;

    /* The ordering of Reference resistor value is specifically designed to keep the result from overflowing */
    return ((uint32)((uint32)resT << Thermistor_1_REF_RES_SHIFT));
}


/*******************************************************************************
* Function Name: Thermistor_1_GetTemperature
********************************************************************************
*
* Summary:
*  The value of the thermistor resistance is passed to this function as a
*  parameter. The function returns the temperature, based on the resistance
*  value. The method used to calculate the temperature is dependent on whether
*  Equation or LUT was selected in the Customizer.
*
* Parameters:
*  resT: the resistance across the thermistor in Ohms.
*
* Return:
*  The return value is the temperature in 1/100ths of degrees C. For example,
*  the return value is 2345, when the actual temperature is 23.45 degrees C.
*
*******************************************************************************/
int16 Thermistor_1_GetTemperature(uint32 resT) 
{
    int16 tempTR;
    static const uint32 CYCODE Thermistor_1_LUT[] = { 32790u, 32626u, 32462u, 32300u, 32138u, 31977u, 31817u, 31658u,
 31500u, 31343u, 31187u, 31031u, 30876u, 30722u, 30569u, 30417u, 30266u, 30115u, 29965u, 29817u, 29668u, 29521u, 29375u,
 29229u, 29084u, 28940u, 28796u, 28654u, 28512u, 28371u, 28231u, 28091u, 27952u, 27814u, 27677u, 27540u, 27405u, 27269u,
 27135u, 27001u, 26869u, 26736u, 26605u, 26474u, 26344u, 26215u, 26086u, 25958u, 25831u, 25704u, 25578u, 25453u, 25328u,
 25204u, 25081u, 24958u, 24836u, 24715u, 24594u, 24474u, 24355u, 24236u, 24118u, 24001u, 23884u, 23767u, 23652u, 23537u,
 23422u, 23309u, 23195u, 23083u, 22971u, 22860u, 22749u, 22639u, 22529u, 22420u, 22311u, 22203u, 22096u, 21989u, 21883u,
 21778u, 21672u, 21568u, 21464u, 21361u, 21258u, 21155u, 21054u, 20952u, 20852u, 20751u, 20652u, 20553u, 20454u, 20356u,
 20258u, 20161u, 20065u, 19968u, 19873u, 19778u, 19683u, 19589u, 19496u, 19402u, 19310u, 19218u, 19126u, 19035u, 18944u,
 18854u, 18764u, 18675u, 18586u, 18498u, 18410u, 18323u, 18236u, 18149u, 18063u, 17977u, 17892u, 17807u, 17723u, 17639u,
 17556u, 17473u, 17390u, 17308u, 17226u, 17145u, 17064u, 16984u, 16904u, 16824u, 16745u, 16666u, 16588u, 16510u, 16432u,
 16355u, 16278u, 16202u, 16125u, 16050u, 15975u, 15900u, 15825u, 15751u, 15678u, 15604u, 15531u, 15459u, 15386u, 15315u,
 15243u, 15172u, 15101u, 15031u, 14961u, 14891u, 14822u, 14753u, 14684u, 14616u, 14548u, 14481u, 14413u, 14347u, 14280u,
 14214u, 14148u, 14082u, 14017u, 13952u, 13888u, 13824u, 13760u, 13696u, 13633u, 13570u, 13507u, 13445u, 13383u, 13322u,
 13260u, 13199u, 13138u, 13078u, 13018u, 12958u, 12899u, 12839u, 12780u, 12722u, 12663u, 12605u, 12548u, 12490u, 12433u,
 12376u, 12320u, 12263u, 12207u, 12152u, 12096u, 12041u, 11986u, 11931u, 11877u, 11823u, 11769u, 11716u, 11662u, 11609u,
 11557u, 11504u, 11452u, 11400u, 11348u, 11297u, 11246u, 11195u, 11144u, 11093u, 11043u, 10993u, 10944u, 10894u, 10845u,
 10796u, 10747u, 10699u, 10651u, 10603u, 10555u, 10507u, 10460u, 10413u, 10366u, 10320u, 10273u, 10227u, 10181u, 10136u,
 10090u, 10045u, 10000u, 9955u, 9911u, 9866u, 9822u, 9778u, 9735u, 9691u, 9648u, 9605u, 9562u, 9519u, 9477u, 9435u,
 9393u, 9351u, 9309u, 9268u, 9227u, 9186u, 9145u, 9104u, 9064u, 9024u, 8984u, 8944u, 8904u, 8865u, 8826u, 8787u, 8748u,
 8709u, 8671u, 8632u, 8594u, 8556u, 8519u, 8481u, 8444u, 8407u, 8370u, 8333u, 8296u, 8260u, 8223u, 8187u, 8151u, 8116u,
 8080u, 8045u, 8009u, 7974u, 7939u, 7904u, 7870u, 7836u, 7801u, 7767u, 7733u, 7699u, 7666u, 7632u, 7599u, 7566u, 7533u,
 7500u, 7468u, 7435u, 7403u, 7371u, 7339u, 7307u, 7275u, 7244u, 7212u, 7181u, 7150u, 7119u, 7088u, 7057u, 7027u, 6996u,
 6966u, 6936u, 6906u, 6876u, 6847u, 6817u, 6788u, 6758u, 6729u, 6700u, 6672u, 6643u, 6614u, 6586u, 6558u, 6529u, 6501u,
 6474u, 6446u, 6418u, 6391u, 6363u, 6336u, 6309u, 6282u, 6255u, 6228u, 6202u, 6175u, 6149u, 6123u, 6097u, 6071u, 6045u,
 6019u, 5993u, 5968u, 5943u, 5917u, 5892u, 5867u, 5842u, 5817u, 5793u, 5768u, 5744u, 5719u, 5695u, 5671u, 5647u, 5623u,
 5599u, 5576u, 5552u, 5529u, 5506u, 5482u, 5459u, 5436u, 5413u, 5391u, 5368u, 5345u, 5323u, 5300u, 5278u, 5256u, 5234u,
 5212u, 5190u, 5168u, 5147u, 5125u, 5104u, 5082u, 5061u, 5040u, 5019u, 4998u, 4977u, 4956u, 4936u, 4915u, 4894u, 4874u,
 4854u, 4834u, 4813u, 4793u, 4773u, 4754u, 4734u, 4714u, 4695u, 4675u, 4656u, 4637u, 4617u, 4598u, 4579u, 4560u, 4541u,
 4523u, 4504u, 4485u, 4467u, 4448u, 4430u, 4412u, 4393u, 4375u, 4357u, 4339u, 4322u, 4304u, 4286u, 4268u, 4251u, 4233u,
 4216u, 4199u, 4182u, 4164u, 4147u, 4130u, 4114u, 4097u, 4080u, 4063u, 4047u, 4030u, 4014u, 3997u, 3981u, 3965u, 3949u,
 3932u, 3916u, 3901u, 3885u, 3869u, 3853u, 3837u, 3822u, 3806u, 3791u, 3775u, 3760u, 3745u, 3730u, 3715u, 3700u, 3685u,
 3670u, 3655u, 3640u, 3625u, 3611u, 3596u, 3582u, 3567u, 3553u, 3539u, 3524u, 3510u, 3496u, 3482u, 3468u, 3454u };


    #if (Thermistor_1_IMPLEMENTATION == Thermistor_1_EQUATION_METHOD)

        float32 stEqn;
        float32 logrT;

        /* Calculate thermistor resistance from the voltages */
        #if(!CY_PSOC3)
            logrT = (float32)log((float64)resT);
        #else
            logrT = log((float32)resT);
        #endif  /* (!CY_PSOC3) */

        /* Calculate temperature from the resistance of thermistor using Steinhart-Hart Equation */
        #if(!CY_PSOC3)
            stEqn = (float32)(Thermistor_1_THA + (Thermistor_1_THB * logrT) + 
                             (Thermistor_1_THC * pow((float64)logrT, (float32)3)));
        #else
            stEqn = (float32)(Thermistor_1_THA + (Thermistor_1_THB * logrT) + 
                             (Thermistor_1_THC * pow(logrT, (float32)3)));
        #endif  /* (!CY_PSOC3) */

        tempTR = (int16)((float32)((((1.0 / stEqn) - Thermistor_1_K2C) * (float32)Thermistor_1_SCALE) + 0.5));

    #else /* Thermistor_1_IMPLEMENTATION == Thermistor_1_LUT_METHOD */

        uint16 mid;
        uint16 first = 0u;
        uint16 last = Thermistor_1_LUT_SIZE - 1u;

        /* Calculate temperature from the resistance of thermistor by using the LUT */
        while (first < last)
        {
            mid = (first + last) >> 1u;

            if ((0u == mid) || ((Thermistor_1_LUT_SIZE - 1u) == mid) || (resT == Thermistor_1_LUT[mid]))
            {
                last = mid;
                break;
            }
            else if (Thermistor_1_LUT[mid] > resT)
            {
                first = mid + 1u;
            }
            else /* (Thermistor_1_LUT[mid] < resT) */
            {
                last = mid - 1u;
            }
        }

        /* Calculation the closest entry in the LUT */
        if ((Thermistor_1_LUT[last] > resT) && (last < (Thermistor_1_LUT_SIZE - 1u)) &&
           ((Thermistor_1_LUT[last] - resT) > (resT - Thermistor_1_LUT[last + 1u])))
        {
            last++;
        }
        else if ((Thermistor_1_LUT[last] < resT) && (last > 0u) &&
                ((Thermistor_1_LUT[last - 1u] - resT) < (resT - Thermistor_1_LUT[last])))
        {
            last--;
        }
        else
        {
            /* Closest entry in the LUT already found */
        }

        tempTR = Thermistor_1_MIN_TEMP + (int16)((uint16)(last * Thermistor_1_ACCURACY));

    #endif /* (Thermistor_1_IMPLEMENTATION == Thermistor_1_EQUATION_METHOD) */

    return (tempTR);
}


/* [] END OF FILE */
