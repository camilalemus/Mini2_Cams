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
#include <xc.h>

#define _XTAL_FREQ 8000000


//******************************************************************************
//                          Instanciar funciones
//******************************************************************************
void setup(void);


//******************************************************************************
//                          Ciclo principal
//******************************************************************************

void main(void) {
    setup();
    while (1) {
        __delay_ms(200);
        PORTA = 0b11111111;
    }
    return;
}

//******************************************************************************
//                                Setup
//******************************************************************************

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTA = 0;
    TRISA = 0;
//    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

