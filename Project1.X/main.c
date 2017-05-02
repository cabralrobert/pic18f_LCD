// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#include <xc.h>
#include <pic18f4550.h>
#include <time.h>
#include <pic18.h>

#define _XTAL_FREQ 8000000

void enable(){     
    PORTEbits.RE1=1;   
    __delay_ms(10);
    PORTEbits.RE1=0;
    __delay_ms(10);
    PORTEbits.RE1=1;   
    __delay_ms(10);
}

void sendCMD(int value){
    PORTEbits.RE2 = 0; //RS
    PORTD = value;
    enable();
    __delay_ms(10);
}

void setLine(int a, int b){
	if(b == 2)
		sendCMD(0xC0 + a);
	else
		sendCMD(0x80 + a);
}

void initLCD(){
    sendCMD(0x01);    
    sendCMD(0x38);
    sendCMD(0x80);    
    sendCMD(0xf);
    sendCMD(0x01);
    __delay_ms(10);    
}

void writeChar(char value){    
    PORTEbits.RE2 = 1; //RS    
    PORTD = value;
    enable();
    __delay_ms(10);        
    PORTEbits.RE2 = 0; //RS                    
}

void writeLCD(char *a){
	int i = 0;
	for(;a[i] != '\0';i++)
		writeChar(a[i]);    
}
/*
void interrupt ISR(){
 
    switch(INTCONbits.TMR0IF){
        case 1:
            INTCONbits.TMR0IF=0;
            
            TMR0H = 0x0;
            TMR0L = 0xDC;
            
            if(i==0){
                i=1;
                //PORTDbits.RD0=1;
                PORTD=0xff;
            }else{
                i=0;
                //PORTD=0x0;
                PORTDbits.RD4=0;
            }
            break;
    }
    
}

void init_time(){
    INTCONbits.GIE=0;
    INTCONbits.PEIE=1;
    INTCONbits.TMR0IF=0x0;
    INTCONbits.TMR0IE=1;
    
    T0CONbits.T08BIT=0;
    T0CONbits.T0CS=0;
    T0CONbits.PSA=0;
    T0CONbits.T0PS2=1;
    T0CONbits.T0PS1=0;
    T0CONbits.T0PS0=0;
    TMR0H = 0x0;
    TMR0L = 0x0C;
    T0CONbits.TMR0ON=1;
    
    INTCONbits.GIE=1;
}*/

void main(void) {    
    TRISD = 0x00;
    TRISE = 0x00;    
    PORTE = 0x00;
    
    initLCD();
    setLine(2,1);
    writeLCD("Bem vindo ao");     
    setLine(3,2);
    writeLCD("toc toc :D");
    
    while(1);
    return;
}