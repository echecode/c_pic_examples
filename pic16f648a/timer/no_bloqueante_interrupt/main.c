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

int oscilador=0;
int contador=0;
int contadorSegundos=0;
int unSegundo=0;

interrupt void rutinaInterrupcion(){
    //aca viene cuando ocurre la interrupcion
    if(INTCONbits.T0IF != 0) //Si la interrupción fue por timer9
    {   INTCONbits.T0IF=0;        
          
        TMR0=15500; //Inicializo, Cuenta desde 15535 hasta 65535 = 50000 cuentas = 50ms
        
        contador++;
        if(contador>=2){
            contador=0;
            //100 ms
            contadorSegundos++;
            if(contadorSegundos>=10){
                contadorSegundos=0;
            
                        unSegundo=1;
                }
            }            
        
    }
}

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
  
    INTCONbits.T0IE=1;  //habilita interrupcion por timer0
    INTCONbits.GIE=1;    //habilita interrupciones generales
 
    do{
    if(unSegundo!=0){
        unSegundo=0;
   
            oscilador=!oscilador;
    
            if(oscilador==0){
                PORTB=0x01; //0000 0001 enciende led   
            }else{
                PORTB=0x00; //0000 0001 enciende led   
            }
    }
    
}while(1);
    
    return;
}
