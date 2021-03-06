/*
 * File:   mainm2.c
 * Author: camil
 *
 * Created on March 10, 2021, 7:22 PM
 */

//******************************************************************************
//                          Configuration Bits
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//******************************************************************************
//                          Variables
//******************************************************************************

#include <xc.h>
#include "I2C.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "BMP280.h"
#include "UART.h"


#define _XTAL_FREQ 8000000

uint32_t temp;
char dec;
char uni;
char deci;
char centi;
char RX;

signed long temperature;
unsigned long pressure;


//typedef enum {
//    MODE_SLEEP = 0x00, // sleep mode
//    MODE_FORCED = 0x01, // forced mode
//    MODE_NORMAL = 0x03 // normal mode
//} BMP280_mode;
//
//// oversampling setting. osrs_t[2:0], osrs_p[2:0]
//
//typedef enum {
//    SAMPLING_SKIPPED = 0x00, //skipped, output set to 0x80000
//    SAMPLING_X1 = 0x01, // oversampling x1
//    SAMPLING_X2 = 0x02, // oversampling x2
//    SAMPLING_X4 = 0x03, // oversampling x4
//    SAMPLING_X8 = 0x04, // oversampling x8
//    SAMPLING_X16 = 0x05 // oversampling x16
//} BMP280_sampling;
//
//// filter setting filter[2:0]
//
//typedef enum {
//    FILTER_OFF = 0x00, // filter off
//    FILTER_2 = 0x01, // filter coefficient = 2
//    FILTER_4 = 0x02, // filter coefficient = 4
//    FILTER_8 = 0x03, // filter coefficient = 8
//    FILTER_16 = 0x04 // filter coefficient = 16
//} BMP280_filter;
//
//
//// standby (inactive) time in ms (used in normal mode), t_sb[2:0]
//typedef enum
//{
//  STANDBY_0_5   =  0x00,  // standby time = 0.5 ms
//  STANDBY_62_5  =  0x01,  // standby time = 62.5 ms
//  STANDBY_125   =  0x02,  // standby time = 125 ms
//  STANDBY_250   =  0x03,  // standby time = 250 ms
//  STANDBY_500   =  0x04,  // standby time = 500 ms
//  STANDBY_1000  =  0x05,  // standby time = 1000 ms
//  STANDBY_2000  =  0x06,  // standby time = 2000 ms
//  STANDBY_4000  =  0x07   // standby time = 4000 ms
//} standby_time;
//
//struct {
//    uint16_t dig_T1;
//    int16_t dig_T2;
//    int16_t dig_T3;
//
//    uint16_t dig_P1;
//    int16_t dig_P2;
//    int16_t dig_P3;
//    int16_t dig_P4;
//    int16_t dig_P5;
//    int16_t dig_P6;
//    int16_t dig_P7;
//    int16_t dig_P8;
//    int16_t dig_P9;
//} BMP280_calib;



//******************************************************************************
//                          Instanciar funciones
//******************************************************************************
void setup(void);
void Send_Temp(void);

//******************************************************************************
//                          Ciclo principal
//******************************************************************************

void main(void) {
    setup();
    BMP280_begin(MODE_NORMAL, SAMPLING_X1, SAMPLING_X1, FILTER_OFF, STANDBY_0_5);
    while (1) {
        __delay_ms(200);
        PORTA = 0b11111111;
        BMP280_readTemperature(&temperature); // read temperature
        BMP280_readPressure(&pressure);
        Send_Temp();
        Send_String("S");
        Send_Data(dec);
        Send_Data(uni);
        Send_String(".");
        Send_Data(deci);
        Send_Data(centi);

        switch (RX) {
            case 49:
                PORTBbits.RB7 = 1;
                break;
            case 48:
                PORTBbits.RB7 = 0;
                break;
            case 50:
                PORTBbits.RB6 = 0;
                break;
            case 51:
                PORTBbits.RB6 = 1;
                break;
            default:
                PORTBbits.RB7 = 0;
                PORTBbits.RB6 = 0;
                break;
        }

    }
   
}

//******************************************************************************
//                                Setup
//******************************************************************************

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    TRISA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTA = 0;
    
    I2C_Master_Init(100000);        // Inicializar Comuncaci??n I2C
}

//******************************************************************************
//                                  Funciones
//******************************************************************************

void Send_Temp(void){
    temp = temperature;
    dec = temp /1000;
    temp = temp - (dec*1000);
    uni = temp /100;
    temp = temp - (uni*100);
    deci = temp /10;
    temp = temp - (deci*10);
    centi = temp;
}

//******************************************************************************
//                                Interrupciones
//******************************************************************************

void __interrupt() isr(void) {
    if (PIR1bits.RCIF == 1) {
        RX = Receive_Data(); //Recibir la data proviniente del uart
        PIR1bits.RCIF = 0;
    }
}