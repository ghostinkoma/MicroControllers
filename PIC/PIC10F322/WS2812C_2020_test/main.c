/* Build on MPLAB IDE 5.35 xc8 2.32
 * File:   ws2812.h
 * Author:  ghostinkoma
 * Comments: 
 * Revision history:  first relase
 */

#include<xc.h>
#include"ws2812.h" 

#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define UP 1
#define DOWN 0
#define DelayMax 8000
#define DelayStep 80
#define DelayMinum 100

volatile unsigned char Ptern = 0;
volatile unsigned char UP_DOWN = DOWN;

void setDefaltColor();
void setColor();
void SystemSetup();

void main(void){
    unsigned int delay_interval_count;
    unsigned int delay_interval  = DelayMax;
    
    SystemSetup();
 
    while (1){
        SendLED();
        for(delay_interval_count = 0;delay_interval > delay_interval_count; delay_interval_count++) asm("nop");
        if(delay_interval < DelayMinum){
            delay_interval = DelayMax;
            if(UP_DOWN == DOWN){
                asm("bsf    _UP_DOWN, 0");
                if(Ptern == 0) setDefaltColor();
                else setColor();
                if(Ptern >6) Ptern =0; else Ptern ++;
            }else asm("bcf    _UP_DOWN, 0");
        }else delay_interval = delay_interval - DelayStep;
        if(UP_DOWN == UP) Led_Scroll_UP();
        else Led_Scroll_DOWN();
    }
}

void SystemSetup(){
    // initialize the device
    OSCCON =        0x70;   //IRCF 16MHz; 
    OPTION_REG =    0x07;	//16M / 256
    //CLKRCON =     0x00;   //CLKROE disabled; 
    BORCON =        0x00;   // SBOREN disabled; BORFS disabled; BORRDY BOR Circuit is inactive; 
    WDTCON =        0x16;   // WDTPS 1:65536; SWDTEN OFF; 
    LATA =          0x00;   //LATx registers
    TRISA =         0x00;   //TRISx registers
    ANSELA =        0x00;   //ANSELx registers
    WPUA =          0x00;   //WPUx registers
    OPTION_REGbits.nWPUEN = 1;
}

void setDefaltColor(){
    LED_G[0] = LED_B[0] = 
    LED_G[1] = 
    LED_G[2] = 
    LED_R[3] = LED_G[3] = 
    LED_R[4] = 
    LED_R[5] = LED_B[5] = 
    LED_B[6] =
    LED_R[9] = LED_G[9] = LED_B[9] = 0;
    LED_R[2] = 20;
    LED_R[0] = LED_R[1] = LED_G[4] =  LED_G[7] = LED_B[7] =  LED_G[6] = 55;
    LED_B[1] = LED_B[4] = LED_G[5] = LED_R[6] = 
    LED_R[8] = LED_G[8] = LED_B[8] = 100;
    LED_R[7] = 120;
    LED_B[2] = 200;
    LED_B[3] = 255;
}

void setColor(){
    LED_Value_Tmp = Ptern;
    asm("BTFSC	_LED_Value_Tmp,0"); 
    asm("GOTO   RedFF");
    asm("GOTO   Red00");
    asm("RedFF:");
    asm("MOVLW  0xFF");
    asm("MOVWF  _LEDR_Value_Tmp");
    asm("GOTO   SetGreen");
    asm("Red00:");
    asm("clrf    _LEDR_Value_Tmp");
    
    asm("SetGreen:");
    asm("BTFSC	_LED_Value_Tmp,1"); 
    asm("GOTO   GreenFF");
    asm("GOTO   Green00");
    asm("GreenFF:");
    asm("MOVLW  0xFF");
    asm("MOVWF  _LEDG_Value_Tmp");
    asm("GOTO SetBlue");
    asm("Green00:");
    asm("clrf    _LEDG_Value_Tmp");
    
    asm("SetBlue:");
    asm("BTFSC	_LED_Value_Tmp,2"); 
    asm("GOTO   BlueFF");
    asm("GOTO   Blue00");
    asm("BlueFF:");
    asm("MOVLW  0xFF");
    asm("MOVWF  _LEDB_Value_Tmp");
    asm("GOTO endOfColorset");
    asm("Blue00:");
    asm("clrf   _LEDB_Value_Tmp");

    asm("endOfColorset:");
    for(unsigned char i=0;i <LED_Length;i++){
        LED_R[i] = LEDR_Value_Tmp;
        asm("RRF _LEDR_Value_Tmp,1");
        
        LED_G[i] = LEDG_Value_Tmp ;
        asm("RRF _LEDG_Value_Tmp,1");
        
        LED_B[i] = LEDB_Value_Tmp ;
        asm("RRF _LEDB_Value_Tmp,1");
    }
}
