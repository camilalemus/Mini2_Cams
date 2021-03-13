/*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */
#include "I2C.h"
//******************************************************************************
// Función para inicializar I2C Maestro
//******************************************************************************

void I2C_Master_Init(const unsigned long c) {
    SSPCON = 0b00101000;            //Config de la comunicacion serial
    SSPCON2 = 0;                    
//    SSPADD = (_XTAL_FREQ / (4 * c)) - 1; 
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 0;       //Se configura como salida los pines de comunicacion
    TRISCbits.TRISC4 = 0;       //SDA y SCL
}
//******************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//******************************************************************************

void I2C_Master_Wait() {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Esperar a que se activen las banderas de q llego algo al buffer
}                                                 //y que la comunicacion no esta ocupada
//******************************************************************************
// Función de inicio de la comunicación I2C PIC
//******************************************************************************

void I2C_Master_Start() {
    I2C_Master_Wait(); 
    SSPCON2bits.SEN = 1; //inicia la comunicación i2c
}
//******************************************************************************
// Función de reinicio de la comunicación I2C PIC
//******************************************************************************

void I2C_Master_RepeatedStart() {
    I2C_Master_Wait(); 
    SSPCON2bits.RSEN = 1; //reinicia la comunicación i2c
}
//******************************************************************************
// Función de parada de la comunicación I2C PIC
//******************************************************************************

void I2C_Master_Stop() {
    I2C_Master_Wait(); 
    SSPCON2bits.PEN = 1; //detener la comunicación i2c
}
//******************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//******************************************************************************

void I2C_Master_Write(unsigned d) {
    I2C_Master_Wait(); 
    SSPBUF = d;             //Copia la data al buffer y comienza a mandar la informacion
}
//******************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//******************************************************************************

unsigned short I2C_Master_Read(unsigned short a) {
    unsigned short temp1;
    I2C_Master_Wait(); 
    SSPCON2bits.RCEN = 1;   //Modo recepcion del master
    I2C_Master_Wait(); 
    temp1 = SSPBUF;         //Copia el buffer a una variable
    I2C_Master_Wait();
    if (a == 1) {
        SSPCON2bits.ACKDT = 0; //Acknowledge
    } else {
        SSPCON2bits.ACKDT = 1; //No acknowledge
    }
    SSPCON2bits.ACKEN = 1; // Iniciar secuencia de Acknowledge
    return temp1; // Regresar valor del dato leído
}
//******************************************************************************
// Función para inicializar I2C Esclavo
//******************************************************************************

void I2C_Slave_Init(uint8_t address) {
    SSPADD = address;    //Direccion que identifica al esclavo
    SSPCON = 0x36; // 0b00110110 CONFIGURACION DEL ESCLAVO
    SSPSTAT = 0x80; // 0b10000000
    SSPCON2 = 0x01; // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
//******************************************************************************