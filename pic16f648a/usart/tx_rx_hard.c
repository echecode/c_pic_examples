// CONFIG
#pragma config FOSC = EXTRCCLK  // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, Resistor and Capacitor on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <stdint.h>

void configurarPuertoSerie(){
//Baud Rate High Speed Mode
TXSTAbits.BRGH=1;
//9600
//En Low Speed BRGH=0
//FOSC/(64(x+1)) = 9600
// X= (FOSC/64*9600)-1 
//En High Speed BRGH=1
//FOSC/(16(x+1)) = 9600
// X= (FOSC/16*9600)-1 
SPBRG = 25;// para 9600 @ 4 Mhz

TRISBbits.TRISB1=1;//entrada rx    
TRISBbits.TRISB2=0;//salida tx     

RCSTAbits.SPEN=1; //HABILITA SERIAL PORT
RCSTAbits.RX9=0;  //8 BITS
RCSTAbits.CREN=1; //RECEPCION CONTINUA
PIE1bits.RCIE=1; //habilitainterrupcion

//Transmision (sin interrupcion)
TXSTAbits.TX9=0;
TXSTAbits.TXEN=1;
//Habilita interrupciones de periféricos
INTCONbits.PEIE=1;
//Habilita Interrupciones Generales
INTCONbits.GIE=1;
}

uint8_t buffer;
uint8_t recibido=0;
interrupt void isr(){    
    if(RCIF!=0){             //RCIF bandera interrupcion
        recibido=1;
        buffer=RCREG;        //El dato se lee en RCREG
    }
    //RCSTABits.FERR  //ERROR FRAMMING
    //RCSTABits.OERR  //ERROR OVERRUN
}

void transmitir( uint8_t dato){
    TXREG=dato;
}

void main(void) {
    configurarPuertoSerie();          
    do{
        while(recibido==0);//espera dato
        recibido=0;
        transmitir(buffer);                       
    }while(1);        
}
