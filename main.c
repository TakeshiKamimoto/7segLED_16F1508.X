/*
 * File:   main.c
 * Author: takes
 *
 * Created on 2022/01/23, 14:07
 */

// PIC16F1508 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ  4000000

#include "i2c_slave.h"
#define I2C_ADDRESS 0x10
        
#define aA  LATCbits.LATC2 //7�Z�OLED�̊e�Z�O�����g�̃A�m�[�h(�v���X��)��Port C�Ɋ��蓖��)
#define aB  LATCbits.LATC0 //�����͊e���ɋ��ʁB�܂�A�m�[�h�R�����B
#define aC  LATCbits.LATC3
#define aD  LATCbits.LATC6
#define aE  LATCbits.LATC4
#define aF  LATCbits.LATC1
#define aG  LATCbits.LATC5

#define k1  LATAbits.LATA4 //7�Z�OLED�e���̃J�\�[�h�i�}�C�i�X�Ɂj��Port A�Ɋ��蓖��
#define k2  LATAbits.LATA2
#define k3  LATAbits.LATA1
#define k4  LATAbits.LATA0

#define colon  LATCbits.LATC7 //7�Z�OLED�́u�R�����v(���ƕ��̋�؂�)�\����Port C�֊��蓖��

unsigned char d1, d2, d3, d4, pt;
unsigned char digit;

void bcd(unsigned char figure);
void digitSelect(unsigned char d);

// ���l����7�Z�OLED�̕\���p�^�[���֕ϊ����ĕ\������
void bcd(unsigned char figure){
    switch (figure){
        case 0:
            aA = 1;
            aB = 1;
            aC = 1;
            aD = 1;

            aE = 1;
            aF = 1;
            aG = 0;
            break;
        case 1 :
            aA = 0;
            aB = 1;
            aC = 1;
            aD = 0;
            
            aE = 0;
            aF = 0;
            aG = 0;
            break;
        case 2 :
            aA = 1;
            aB = 1;
            aC = 0;
            aD = 1;
            
            aE = 1;
            aF = 0;
            aG = 1;
            break;
        case 3 :
            aA = 1;
            aB = 1;
            aC = 1;
            aD = 1;
            
            aE = 0;
            aF = 0;
            aG = 1;
            break;
        case 4 :
            aA = 0;
            aB = 1;
            aC = 1;
            aD = 0;
            
            aE = 0;
            aF = 1;
            aG = 1;
            break;
        case 5 :
            aA = 1;
            aB = 0;
            aC = 1;
            aD = 1;
            
            aE = 0;
            aF = 1;
            aG = 1;
            break;
        case 6 :
            aA = 1;
            aB = 0;
            aC = 1;
            aD = 1;
            
            aE = 1;
            aF = 1;
            aG = 1;
            break;
        case 7 :
            aA = 1;
            aB = 1;
            aC = 1;
            aD = 0;
            
            aE = 0;
            aF = 0;
            aG = 0;
            break;
        case 8 :
            aA = 1;
            aB = 1;
            aC = 1;
            aD = 1;
            
            aE = 1;
            aF = 1;
            aG = 1;
            break;
        case 9 :
            aA = 1;
            aB = 1;
            aC = 1;
            aD = 1;
            
            aE = 0;
            aF = 1;
            aG = 1;
            break;
        default:
            break;
    }
}

//7�Z�OLED�e���̎w�茅������Y���̌��̃J�\�[�h���A�N�e�B�uLOW�ɂ���B
void digitSelect(unsigned char d){
    switch (d){
        case 1:
            k1 = 0;
            k2 = 1;
            k3 = 1;
            k4 = 1;
            break;
        case 2:
            k1 = 1;
            k2 = 0;
            k3 = 1;
            k4 = 1;
            break;
        case 3:
            k1 = 1;
            k2 = 1;
            k3 = 0;
            k4 = 1;
            break;
        case 4:
            k1 = 1;
            k2 = 1;
            k3 = 1;
            k4 = 0;
            break;
        default:
            break;
    }
}


//I2C�f�[�^��M���ɂ������Ă΂�āA��M�f�[�^���l������
void onI2CReceiveCallback(unsigned char address, unsigned char data) {
    switch( address ){
        case 0x00:
            d1 = data;
            break;
        case 0x01:
            d2 = data;
            break;
        case 0x02:
            d3 = data;
            break;
        case 0x03:
            d4 = data;
            break;
        case 0x04:
            pt = data;
        default:
            break;
    }
}



void main(void){
    OSCCON = 0b01101010; // 4MHz
    //          1111 = 16 MHz
    //          1110 = 8 MHz
    //          1101 = 4 MHz
    //          1100 = 2 MHz
    //          1011 = 1 MHz
    OPTION_REGbits.nWPUEN = 0;
    
    TRISC = 0b00000000; // set RC0 to RC7 as Output
    TRISA = 0b00000;     // set RA0 to RA4 as OUTPUT
    
    ANSELB = 0; // set Port B for digital
    TRISBbits.TRISB4 = 1; //set RB4 as Input for SDA
    TRISBbits.TRISB6 = 1;// set RB6 as Input for SCL
    WPUBbits.WPUB4 = 1; // Weak Pull-up for SDA port
    WPUBbits.WPUB6 = 1; // Weak Pull-up for SCL port

    //Timer-1�̐ݒ�(7�Z�OLED�̃_�C�i�~�b�N�\���Ɏg�p)
    T1CONbits.T1CKPS1 = 0b00; // Prescale = 1:1
    //    11 = 1:8 Prescale value
    //    10 = 1:4 Prescale value
    //    01 = 1:2 Prescale value
    //    00 = 1:1 Prescale value
    T1CONbits.TMR1ON = 1;// Enables Timer1
    TMR1IF = 0;
    
    //���荞�݋���
    TMR1IE = 1;
    PEIE = 1;
    GIE = 1;

    
    // Start as I2C slave
    setupI2CSlave(I2C_ADDRESS);

    
    while(1);//main�͖������[�v�Ƃ��A���荞�ݏ����ɂ��ׂĔC����
}

void __interrupt() isr(void){
    
  // �^�C�}���荞�ݔ����̏ꍇ�̏���(7�Z�OLED�̃_�C�i�~�b�N�\��)
  if( TMR1IF ){
    TMR1IF = 0;
    TMR1ON = 0;
    TMR1H = 0xEC;//�_�C�i�~�b�N�\���̃��t���b�V���^�C�}�ݒ�i���l���傫���قǎ����͒Z���Ȃ�j
    TMR1L = 0xFF;
    TMR1ON = 1;
    
    digit++;
    digit %= 4;
    
    switch(digit)
    {
        case 0:
            digitSelect(1);
            bcd(d1);
            break;
        case 1:
            digitSelect(2);
            bcd(d2);
            break;
        case 2:
            digitSelect(3);
            bcd(d3);
            break;
        case 3:
            digitSelect(4);
            bcd(d4);
            break;
        default:
            break;
    }
    
    colon = pt > 0? 1 : 0; // pt���[���ȊO�̂Ƃ��� colon ��_��������B
  }
    
    //I2C���荞�ݔ������̏���
  if (SSP1IF) {
        SSP1IF = 0; // Clear interrupt flag
        checkStateAndManageI2c();
  }
}
        
        

