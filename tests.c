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
 *          Leds adressables                        ==> ?
 *          Affichage avec data streamer            ==> ?     
 *          Boutons tactiles séparés                ==> ?
 *          Boutons tactiles regroupés              ==> ?
 *          Bouton tactile proximité                ==> ?
 *          Bus I2C esclave                         ==> ?
 * 
 * Tests unitaires
 *          ?                                       ==> ?
 *  *  
 * Tests intégrations
 *          Commande leds par message I2C           ==> ?
 *          Lecture états boutons tactiles          ==> ?
 *          Configuration boutons tactiles          ==> ?
 * 
 */

//----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/touch/mtouch.h"

#include "leds.h"
#include "systick.h"

/*!
 * 
 */
void testHardware_ledDebug(void)
{
    SYSTEM_Initialize();
    TRISCbits.TRISC5 = 0;

    while (1) {
        PORTCbits.RC5 = 0;
        __delay_ms(50);
        PORTCbits.RC5 = 1;
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
        PORTCbits.RC5 = 0;
        update += 50;
        while (SYSTICK_isOverrun(update));

        PORTCbits.RC5 = 1;
        update += 950;
        while (SYSTICK_isOverrun(update));
    }
}


extern leds_state_t leds_state;

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
 * première leds pour passer de 3v3 à 5V avec un collecteur ouvert 
 *              
 */
void testHardware_ledsAdressables(void)
{
    SYSTEM_Initialize();
    SYSTICK_initialize();
    LEDS_initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while (1) {
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


mtouch_sensor_sample_t MTOUCH_Sensor_RawSample_Get(enum mtouch_sensor_names name) ;

void print_sensor_value (void) {
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

void test_unitaires(void)
{
    //testHardware_ledDebug ();
    //testHardware_sysTick ();
    //testHardware_ledsAdressables ();
    testDriver_ledsAdressables();
    //testHardware_uart();
    //testHardware_buttons();
}
// End of file 


