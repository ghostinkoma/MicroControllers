/* 
 * File:   ws2812.h
 * Author:  ghostinkoma
 * Comments: 
 * Revision history:  first relase
 */

#include <xc.h> // include processor files - each processor file is guarded.  

//toDO set your LED strip length
#define LED_Length 10

//toDo Set your out put pin for LED control. can't set MCLR pin.
#define LED_cont_Pin_On  asm("BSF	PORTA,2"); 
#define LED_cont_Pin_Off asm("BCF	PORTA,2");

volatile unsigned char * LED_R[LED_Length];
volatile unsigned char * LED_G[LED_Length];
volatile unsigned char * LED_B[LED_Length];
volatile unsigned char LED_Value_Tmp;
volatile unsigned int LEDR_Value_Tmp;
volatile unsigned int LEDG_Value_Tmp;
volatile unsigned int LEDB_Value_Tmp;
volatile unsigned char bitCount;

//core
void SendLED();

//util
void Led_Scroll_UP();
void Led_Scroll_DOWN();

void SendLED(){
    for(unsigned int Led_Index = 0;Led_Index < 10;Led_Index++){
        LEDG_Value_Tmp = LED_G[Led_Index];
        LEDR_Value_Tmp = LED_R[Led_Index];
        LEDB_Value_Tmp = LED_B[Led_Index];
        //Send LED Red value   
       
        asm("bsf    _bitCount, 3");//	; 1
        asm("BitLoopG:");
        asm("BTFSC  _LEDG_Value_Tmp,7");
        LED_cont_Pin_On 
        LED_cont_Pin_On
        LED_cont_Pin_Off
        asm("RLF    _LEDG_Value_Tmp,1");
        asm("decfsz	_bitCount, F");
        asm("GOTO	BitLoopG");
        //Send LED Green value
        asm("bsf    _bitCount, 3");//	; 1
        asm("BitLoopR:");
        asm("BTFSC  _LEDR_Value_Tmp,7");
        LED_cont_Pin_On 
        LED_cont_Pin_On
        LED_cont_Pin_Off
        asm("RLF    _LEDR_Value_Tmp,1");
        asm("decfsz	_bitCount, F");
        asm("GOTO	BitLoopR");
        //Send LED Blue value
        asm("bsf    _bitCount, 3");//	; 1
        asm("BitLoopB:");
        asm("BTFSC  _LEDB_Value_Tmp,7");
        LED_cont_Pin_On
        LED_cont_Pin_On
        LED_cont_Pin_Off
        asm("RLF    _LEDB_Value_Tmp,1");
        asm("decfsz	_bitCount, F");
        asm("GOTO	BitLoopB");
    }
}

void Led_Scroll_UP(){
    LEDR_Value_Tmp = LED_R[0];
    LEDG_Value_Tmp = LED_G[0];
    LEDB_Value_Tmp = LED_B[0];
    for(unsigned char i = 0 ;i <  LED_Length ;i++){
        LED_R[i] = LED_R[i + 1];
        LED_G[i] = LED_G[i + 1];
        LED_B[i] = LED_B[i + 1];
    }
    LED_R[9] = LEDR_Value_Tmp;
    LED_G[9] = LEDG_Value_Tmp;
    LED_B[9] = LEDB_Value_Tmp;
}

void Led_Scroll_DOWN(){
    LEDR_Value_Tmp = LED_R[9];
    LEDG_Value_Tmp = LED_G[9];
    LEDB_Value_Tmp = LED_B[9];
    for(unsigned char i = LED_Length -1 ;i >  0 ;i--){
        LED_R[i] = LED_R[i - 1];
        LED_G[i] = LED_G[i - 1];
        LED_B[i] = LED_B[i - 1];
    }
    LED_R[0] = LEDR_Value_Tmp;
    LED_G[0] = LEDG_Value_Tmp;
    LED_B[0] = LEDB_Value_Tmp;
}
