#include<pic.h>
//
//
//USART = Universal asynchronous receiver/transmitter
//
//8N1 = 8 bits, No-parity, 1 stop
//
//        start                                        stop
//_________                                           __________________
// -IDLE- |___| b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 |    | -IDLE-
//
//En este caso solo vamos a hacer un transmisor
//

/**
 * Numero de bit del puerto B elegido para usar como transmisior serie (TX)
 */
#define BIT_TRANSMISION 0
/*
 * ATENCION la velocidad es de 1200,8N1
 * Se usa una velocidad lenta de 1200 para que los pequeños cambios en el tiempo
 * de los retardos (que dependerán del compilador) sean mucho menores al tiempo de bit
 * y no afecten la transmisión
 */
#define TIEMPO_BIT_US 833 //Para BAUD=1200 833uS por BIT

//Instrucciones del micro a 1uS con XTAL=4Mhz

void enviarUSART(const unsigned char data) {
    unsigned char repetir = 0;
    unsigned char index = 0;
    unsigned char valor;
    unsigned char andVar;

    //Tiempo entre envíos
    for (repetir = 0; repetir < 5; repetir++) { // Repetidas veces para separar entre datos
        _delay(TIEMPO_BIT_US); // - espera tiempo de bit -
    }

    PORTB &= ~(1 << BIT_TRANSMISION); //Pone en 0 (BIT DE START)
    _delay(TIEMPO_BIT_US); // - espera tiempo de bit -
    for (index = 0; index < 8; index++) { //Bucle que recorre los 8 bits del dato
        andVar = (0x1 << index); //Desplaza a la izquierda un bit
        valor = data & andVar; //"Lee" el estado de ese bit mediante AND
        if (valor != 0) { //Si lee 1
            PORTB |= (1 << BIT_TRANSMISION); // pone en 1 el BIT del PORTB elegido como TX
        } else { //Sino (si lee 0)
            PORTB &= ~(1 << BIT_TRANSMISION); // pone en 0 el BIT del PORTB elegido como TX
        }
        _delay(TIEMPO_BIT_US); // - espera tiempo de bit -
    }
    PORTB |= (1 << BIT_TRANSMISION); //Pone en 1 (Bit de STOP)
    _delay(TIEMPO_BIT_US); // - espera tiempo de bit -

}
//------------------------------------------------------------------

/**
 * Función que permite enviar varios Bytes consecutivos
 * Recibe como argumento strings, en C, son arreglos de caracteres terminados en 0.
 * No permite enviar el valor cero porque lo usa para marcar el fin del array.
 */
void escribirUSART(const char *arreglo) {
    unsigned char index = 0;

    /*Una forma de hacerlo*/
    while (arreglo[index] != 0) { //Compara si es diferente de cero (y sino, cero indica el final)
        enviarUSART(arreglo[index]); //Llama a la función que envía mediante protocolo serie
        index++; //Aumenta en uno el índice para acceder al siguiente byte
    }

    /*Otra forma de hacer lo mismo*/
    //while(*arreglo!=0){
    // enviar(*(arreglo+index));
    // index++;
    //}
}
//------------------------------------------------------------------

void main() {
    //Se configura todo el Puerto B como salida.
    TRISB = 0x00; //la selección de banco la hace el compilador (automática)
    escribirUSART("Hola Mundo!"); //envía texto en forma serie
    while (1); //Queda en bucle vacío para detener el procesamiento
} 
