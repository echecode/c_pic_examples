#include <xc.h>

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

void main(void) {
    
    TRISB=0x00; //todo PORTB como salida
    CMCON=0x07; //Apaga comparadores para poder usar i/o digital puerto A 
            
    OPTION_REGbits.T0CS=0;//Fuente de Clock Interna
    OPTION_REGbits.PSA=0; //Utiliza Prescaler en TMR0
    
    //Prescaler en 111 para que cuente cada 256
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;    
    
    TMR0=0;
    
    PORTB=0x00; //0000 0000
   
    while(INTCONbits.T0IF != 0);//bloquea, espera que  se desborda el timer0
    INTCONbits.T0IF=0;
             
    PORTB=0x01; //0000 0001
    
    while(1);
    
    return;
}
