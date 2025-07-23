/* 
 * Tests unitaires et tests d'intégrations
 * 
 * File:   tests.c
 * Author: phifo
 *
 * Created on 18 mars 2025, 09:28
 * 
 * Tests Hardwares
 *          Led debug                               ==> ok 
 *          Timer, base de temps à 1 milliseconde   ==> ok
 *          Leds adressables                        ==> ok
 *          Affichage avec data streamer            ==> ok en partie seulement (seul les valeurs configurées sur MCC)     
 *          Boutons tactiles séparés                ==> ok
 *          Boutons tactiles regroupés              ==> ?
 *          Bouton tactile proximité                ==> ok
 *          Bus I2C esclave                         ==> ?
 * 
 * Tests unitaires
 *          ?                                       ==> ?
 *  *  
 * Tests intégrations
 *          Commande leds par messages I2C          ==> ?
 *          Lecture états boutons tactiles          ==> ?
 *          Configuration boutons tactiles          ==> ?
 * 
 */

//----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/touch/mtouch.h"

#include "../Common/common_data.h"
#include "leds.h"
#include "systick.h"
#include "i2c_slave.h"


extern volatile I2C_data_t I2C_data;

/*!
 * 
 */
void testHardware_ledDebug(void)
{
    SYSTEM_Initialize();
    TRISCbits.TRISC5 = 0;

    while (1) {
        PORTCbits.RC5 = 0; // = led debug on
        __delay_ms(50);
        PORTCbits.RC5 = 1; // = led debug off
        __delay_ms(450);
    }
}

void testHardware_sysTick(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    TRISCbits.TRISC5 = 0;

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    uint32_t update = SYSTICK_read();
    while (1) {
        PORTCbits.RC5 = 0; // = led debug on
        update += 50;
        while (!SYSTICK_isOverrun(update));

        PORTCbits.RC5 = 1; // = led debug off
        update += 950;
        while (!SYSTICK_isOverrun(update));
    }
}


//extern leds_state_t leds_state;

/**
 * Test du 18/03/2025
 * 
 *          Test des sous-ensembles
 *              CLC1            ==> Ok (mais penser à la porte "Et" en entrée 
 *              PPS RC4         ==> Ok
 *              PWM3 sur CLC1   ==> Ok (et indirectement TIMER2 ==> ok)
 *
 *              SCK & SDO       ==> Ok
 * En final, les configs fonctionnement (mais il faut ajouter une pull-up sur la 
 * première leds pour passer de 3v3 à 5V avec un collecteur ouvert)
 * 
 * retour du 22/05 : demontage par erreur de la resistance (avec 1K ça ne fonctionne pas, avec 120R non plus)
 * Quelle était la valeur ?
 *  
 *              
 */
void testHardware_ledsAdressables(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    LEDS_initialize();
    TRISCbits.TRISC5 = 0;

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while (1) {
        PORTCbits.RC5 = 1; // = led debug off
        SSP2BUF = 0x98; //SPI2TXB = g;
        while (PIR4bits.SSP2IF == 0); // while (!SPI2SPI2STATUSbits.TXBE);
        PIR4bits.SSP2IF = 0;
        SSP2BUF = 0x0;
        while (PIR4bits.SSP2IF == 0);
        PIR4bits.SSP2IF = 0;
        SSP2BUF = 0x0;
        while (PIR4bits.SSP2IF == 0);
        PIR4bits.SSP2IF = 0;
        __delay_ms(100);

        PORTCbits.RC5 = 0; // = led debug on
        SSP2BUF = 0x0; //SPI2TXB = g;
        while (PIR4bits.SSP2IF == 0); // while (!SPI2SPI2STATUSbits.TXBE);
        PIR4bits.SSP2IF = 0;
        SSP2BUF = 0x55;
        while (PIR4bits.SSP2IF == 0);
        PIR4bits.SSP2IF = 0;
        SSP2BUF = 0x0;
        while (PIR4bits.SSP2IF == 0);
        PIR4bits.SSP2IF = 0;
        __delay_ms(100);
    }
}

void testDriver_ledsAdressables(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    LEDS_initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    leds_state.color.blue = 200;
    leds_state.color.red = 0;
    leds_state.color.green = 0;
    leds_state.mode = ON;

    leds_state.mode = FADING_BLINK;
    uint32_t time = SYSTICK_read();
    while (1) {
        leds_state.color.blue = 200;
        leds_state.color.red = 0;
        leds_state.color.green = 0;
        while (!SYSTICK_isOverrun(time)) {
            LEDS_serviceMainLoop();
        }
        time += 5000;

        leds_state.color.blue = 0;
        leds_state.color.red = 200;
        leds_state.color.green = 0;
        while (!SYSTICK_isOverrun(time)) {
            LEDS_serviceMainLoop();
        }
        time += 5000;

        leds_state.color.blue = 0;
        leds_state.color.red = 0;
        leds_state.color.green = 200;
        while (!SYSTICK_isOverrun(time)) {
            LEDS_serviceMainLoop();
        }
        time += 5000;

        leds_state.color.blue = 200;
        leds_state.color.red = 200;
        leds_state.color.green = 200;
        while (!SYSTICK_isOverrun(time)) {
            LEDS_serviceMainLoop();
        }
        time += 5000;
    }
}

void testHardware_uart(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    uint32_t time = SYSTICK_read();
    int i = 0;
    while (1) {
        printf("Compteur = %d \n\r", i++);
        while (!SYSTICK_isOverrun(time));
        time += 1000;
    }
}

void print_buttons_states(void)
{
    printf("\n");
    for (enum mtouch_button_names name = 0; name < MTOUCH_BUTTONS; name++)
        if (MTOUCH_Button_isPressed(name))
            printf("\t 1");
        else
            printf("\t 0");
}


mtouch_sensor_sample_t MTOUCH_Sensor_RawSample_Get(enum mtouch_sensor_names name);

void print_sensor_value(void)
{
    mtouch_sensor_sample_t val;

    printf("\n");
    for (enum mtouch_button_names name = 0; name < MTOUCH_BUTTONS; name++) {
        val = MTOUCH_Sensor_RawSample_Get(name);
        printf("\t %05d", val);
    }
}

void processButtonTouch(enum mtouch_button_names button);
void processButtonRelease(enum mtouch_button_names button);

void testHardware_buttons(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    MTOUCH_Initialize();

    MTOUCH_Button_SetPressedCallback(processButtonTouch);
    MTOUCH_Button_SetNotPressedCallback(processButtonRelease);

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    uint32_t time = SYSTICK_read();
    int i = 0;

    while (1) {
        if (MTOUCH_Service_Mainloop()) {
            // If Touch Tuning  is enabled, touchTuneProcess() need to be called inside MTOUCH_Service_Mainloop branch statement

            //touchTuneProcess();
            /* Button API*/
            if (MTOUCH_Button_isPressed(0)) {
                
                /* process if button is pressed */
                /* LED_SetHigh();*/

            } else {
                /* process if button is not pressed */
                /* LED_SetLow();*/
            }
        }

        if (SYSTICK_isOverrun(time)) {
            time += 1000;
            //print_buttons_states();
            //   print_sensor_value ();
        }
    }
}


uint8_t test_i2c_data [10];

void testHardware_i2cSlave(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    TRISCbits.TRISC5 = 0;

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    uint32_t time = SYSTICK_read();

    I2C_client_initialize(test_i2c_data, 2);

    while (1) {
        if (SYSTICK_isOverrun(time)) {
            time += 20;
            if (test_i2c_data[0] == 0x01) {
                PORTCbits.RC5 = 0;
            } else {
                PORTCbits.RC5 = 1;
            }
        }
    }
}

void I2C_DATA_service_Mainloop(void);

void testDriver_i2cSlave(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    LEDS_initialize();
    //TRISCbits.TRISC5 = 0;
    MTOUCH_Initialize();
    I2C_client_initialize(I2C_data.I2C_clientDataArray, sizeof (I2C_data.I2C_clientDataArray));
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    uint32_t time = SYSTICK_read();

    while (1) {
        if (SYSTICK_isOverrun(time)) {
            time += 20;
            I2C_DATA_service_Mainloop();
            LEDS_serviceMainLoop();   
        }
    }
}

void test_unitaires(void)
{
    //testHardware_ledDebug (); // La led de debug doit clignoter toutes les 500ms (rapport cyclique 10% - sans le systick)
    //testHardware_sysTick (); // La led de debug doit clignoter toutes les 1s (rapport cyclique 5% - avec le systick et les interruptions)
    //testHardware_ledsAdressables ();
    //testDriver_ledsAdressables();
    //testHardware_uart();
    //testHardware_buttons();
    //testHardware_i2cSlave(); // reception et modification de la led de debug
    //testDriver_i2cSlave();

    while (1) {}    
}
// End of file 


