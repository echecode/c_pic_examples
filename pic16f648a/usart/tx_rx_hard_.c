
    // CONFIG
    #pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
    #pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
    #pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
    #pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
    #pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
    #pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
    #pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
    #pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
    
    #include <xc.h>
    #include <stdint.h>

void configurarPuertoSerie(){

//RECEPCION
TXSTAbits.BRGH=0;

//9600
//FOSC/(64(x+1)) = 9600
// X= (FOSC/64*9600)-1 
SPBRG = 12;// para 9600
TRISCbits.TRISC6=0;//salida tx
TRISCbits.TRISC7=1;//entrada rx

RCSTAbits.SPEN=1;//HABILITA SERIAL PORT
RCSTAbits.RX9=0;//8 BITS
RCSTAbits.CREN=1;//RECEPCION CONTINUA
//RCSTABits.FERR //ERROR FRAMMING
//RCSTABits.OERR //ERROR OVERRUN

//el dato se lee en RCREG
//RCIF bandera interrupcion
PIE1bits.RCIE=1; //habilitainterrupcion

//Transmision (sin interripcion)
TXSTAbits.TX9=0;
TXSTAbits.TXEN=1;

//habilita interrupciones de perifericos
INTCONbits.PEIE=1;

//habilita interrupciones generales
INTCONbits.GIE=1;
}

uint8_t buffer;
uint8_t recibido=0;
interrupt void isr(){    
    if(RCIF!=0){      
        recibido=1;
        buffer=RCREG;        
    }
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
