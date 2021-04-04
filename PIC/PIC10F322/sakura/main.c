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

volatile unsigned int * LED_R[6];
volatile unsigned int * LED_G[11];
volatile unsigned int * LED_B[11];

volatile unsigned int LED_Value_Tmp;
volatile unsigned int bitCount;

void Send1Byte(){
    asm("bsf    _bitCount, 3");//	; 1
    asm("BitLoop:");
    asm("BSF	PORTA,2"); 
    asm("BTFSS  _LED_Value_Tmp,7");
    asm("BCF	PORTA,2");
    asm("BCF	PORTA,2");
    asm("RLF    _LED_Value_Tmp,1");
    asm("decfsz	_bitCount, F");
    asm("GOTO	BitLoop");
}

void Send1LED(char R_value,char G_value,char B_value){
    LED_Value_Tmp= R_value;
    Send1Byte();
    LED_Value_Tmp= G_value;
    Send1Byte();
    LED_Value_Tmp= B_value;
    Send1Byte();
}
void main(void){
    // initialize the device
    OSCCON = 0x70;          //IRCF 16MHz; 
    CLKRCON = 0x00;         //CLKROE disabled; 
    BORCON = 0x00;          // SBOREN disabled; BORFS disabled; BORRDY BOR Circuit is inactive; 
    WDTCON = 0x16;          // WDTPS 1:65536; SWDTEN OFF; 
    LATA = 0x00;            //LATx registers
    TRISA = 0x00;           //TRISx registers
    ANSELA = 0x00;          //ANSELx registers
    WPUA = 0x00;            //WPUx registers
    OPTION_REGbits.nWPUEN = 1;
    unsigned int i;
    for(int i =0; i < 11; i++){
        LED_G[i] = 70  + i + i + i + i + i + i + i + i + i + i;
        LED_B[i] = 100 + i + i + i + i + i;
    }
    
    while (1){
        Send1LED(200,LED_G[10],LED_B[10]);
        Send1LED(205,LED_G[9],LED_B[9]);
        Send1LED(210,LED_G[8],LED_B[8]);
        Send1LED(215,LED_G[7],LED_B[7]);
        Send1LED(220,LED_G[6],LED_B[6]);
        Send1LED(225,LED_G[5],LED_B[5]);
        Send1LED(230,LED_G[4],LED_B[4]);
        Send1LED(235,LED_G[3],LED_B[3]);
        Send1LED(240,LED_G[2],LED_B[2]);
        Send1LED(245,LED_G[1],LED_B[1]);
        Send1LED(250,LED_G[0],LED_B[0]);

        Send1LED(250,LED_G[10],LED_B[10]);
        Send1LED(245,LED_G[9],LED_B[9]);
        Send1LED(240,LED_G[8],LED_B[8]);
        Send1LED(235,LED_G[7],LED_B[7]);
        Send1LED(230,LED_G[6],LED_B[6]);
        Send1LED(225,LED_G[5],LED_B[5]);
        Send1LED(220,LED_G[4],LED_B[4]);
        Send1LED(215,LED_G[3],LED_B[3]);
        Send1LED(210,LED_G[2],LED_B[2]);
        Send1LED(205,LED_G[1],LED_B[9]);
        Send1LED(200,LED_G[0],LED_B[0]);
        
        for( i =0; i < 11; i++){
            if(LED_G[i] < 150)LED_G[i] = LED_G[i] + 8;else LED_G[i] = 40; 
            if(LED_B[i] < 225)LED_B[i] = LED_B[i] + 6;else LED_B[i] = 80;   
        }
        for(long i = 0;i < 13192; i++){
            asm("nop");
        }
    }
}
