#include<xc.h>

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

#define LED_Length 10
volatile unsigned int * LED_R[10];
volatile unsigned int * LED_G[10];
volatile unsigned int * LED_B[10];

volatile unsigned int LEDR_Value_Tmp;
volatile unsigned int LEDG_Value_Tmp;
volatile unsigned int LEDB_Value_Tmp;
volatile unsigned int bitCount;

void SendLED(){
    unsigned int i =0;
    for(i = 0; i < LED_Length; i ++){
        LEDG_Value_Tmp = LED_G[i];
        LEDR_Value_Tmp = LED_R[i];
        LEDB_Value_Tmp = LED_B[i];

        asm("bsf    _bitCount, 3");//	; 1
        asm("BitLoopG:");
        asm("BTFSC  _LEDG_Value_Tmp,7");
        asm("BSF	PORTA,2"); 
        asm("BSF	PORTA,2"); 
        asm("BCF	PORTA,2");
        asm("RLF    _LEDG_Value_Tmp,1");
        asm("decfsz	_bitCount, F");
        asm("GOTO	BitLoopG");

        asm("bsf    _bitCount, 3");//	; 1
        asm("BitLoopR:");
        asm("BTFSC  _LEDR_Value_Tmp,7");
        asm("BSF	PORTA,2"); 
        asm("BSF	PORTA,2"); 
        asm("BCF	PORTA,2");
        asm("RLF    _LEDR_Value_Tmp,1");
        asm("decfsz	_bitCount, F");
        asm("GOTO	BitLoopR");

        asm("bsf    _bitCount, 3");//	; 1
        asm("BitLoopB:");
        asm("BTFSC  _LEDB_Value_Tmp,7");
        asm("BSF	PORTA,2"); 
        asm("BSF	PORTA,2"); 
        asm("BCF	PORTA,2");
        asm("RLF    _LEDB_Value_Tmp,1");
        asm("decfsz	_bitCount, F");
        asm("GOTO	BitLoopB");
    }
}
void setDefaltColor(){
    LED_R[0] = 100;
    LED_G[0] = 0;
    LED_B[0] = 0;
        
    LED_R[1] = 100;
    LED_G[1] = 0;
    LED_B[1] = 100;
        
    LED_R[2] = 50;
    LED_G[2] = 0;
    LED_B[2] = 100;

    LED_R[3] = 0;
    LED_G[3] = 0;
    LED_B[3] = 200;

    LED_R[4] = 0;
    LED_G[4] = 50;
    LED_B[4] = 100;
        
    LED_R[5] = 0;
    LED_G[5] = 250;
    LED_B[5] = 0;
        
    LED_R[6] = 100;
    LED_G[6] = 70;
    LED_B[6] = 0;
        
    LED_R[7] = 120;
    LED_G[7] = 50;
    LED_B[7] = 50;
        
    LED_R[8] = 100;
    LED_G[8] = 100;
    LED_B[8] = 100;
        
    LED_R[9] = 0;
    LED_G[9] = 0;
    LED_B[9] = 0;
}
void main(void){
    // initialize the device
    OSCCON = 0x70;          //IRCF 16MHz; 
    OPTION_REG = 0x07;				//8M / 256
    //CLKRCON = 0x00;         //CLKROE disabled; 
    BORCON = 0x00;          // SBOREN disabled; BORFS disabled; BORRDY BOR Circuit is inactive; 
    WDTCON = 0x16;          // WDTPS 1:65536; SWDTEN OFF; 
    LATA = 0x00;            //LATx registers
    TRISA = 0x00;           //TRISx registers
    ANSELA = 0x00;          //ANSELx registers
    WPUA = 0x00;            //WPUx registers
    OPTION_REGbits.nWPUEN = 1;
    unsigned int i;
    unsigned long delay_interval_count;
    unsigned long delay_interval;
    
     delay_interval = 25535;
    setDefaltColor();
    
    while (1){
        SendLED();

        for(delay_interval_count = 0;i < delay_interval; i++){
            asm("nop");
        }
        if(delay_interval < 100){
            delay_interval = 25500;
        }else{
            delay_interval = delay_interval - 50;
        }
        LEDR_Value_Tmp = LED_R[0];
        LEDG_Value_Tmp = LED_G[0];
        LEDB_Value_Tmp = LED_B[0];
        for( i = 0 ; i < LED_Length -1; i++){
            LED_R[i] = LED_R[i + 1];
            LED_G[i] = LED_G[i + 1];
            LED_B[i] = LED_B[i + 1];
        }
        LED_R[9] = LEDR_Value_Tmp;
        LED_G[9] = LEDG_Value_Tmp;
        LED_B[9] = LEDB_Value_Tmp;
    }
}
