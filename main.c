/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
 */

/*
� [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"

#include "../Common/common_data.h"
#include "leds.h"
#include "systick.h"
#include "i2c_slave.h"

//#include "i2c_data.h"

void test_unitaires(void);

volatile I2C_data_t I2C_data;
volatile uint8_t * I2C_clientDataArray = I2C_data.I2C_clientDataArray;
volatile bool proximityButtonPressed = false;

// Private variable
//volatile static uint8_t CLIENT_DATA[I2C_CLIENT_LOCATION_SIZE] = {
//    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09
//};

void I2C_DATA_initialize(void)
{
    I2C_data.I2C_clientDataStruct.ident = 0xb3; // Magic number = Whoami
    I2C_data.I2C_clientDataStruct.debugLedState = ON;
    I2C_data.I2C_clientDataStruct.rgbLedsMode = FADING_BLINK;
    I2C_data.I2C_clientDataStruct.rgbLedsColor.blue = 10;
    I2C_data.I2C_clientDataStruct.rgbLedsColor.red = 20;
    I2C_data.I2C_clientDataStruct.rgbLedsColor.green = 40;
    I2C_data.I2C_clientDataStruct.rgbLedIntensity = 100;
    I2C_data.I2C_clientDataStruct.rgbLedsBlinkTempo = 100;

    I2C_data.I2C_clientDataStruct.proximityState = 0;
    I2C_data.I2C_clientDataStruct.proximityRawValue = 0;
}

/**
 * 
 */
void I2C_DATA_service_Mainloop(void)
{
    // Modification uniquement si reception d'un message I2C
    if (I2C_stopReceived) {
        I2C_stopReceived = false;

        LEDS_configure(I2C_data.I2C_clientDataStruct.rgbLedsColor,
            I2C_data.I2C_clientDataStruct.rgbLedIntensity,
            I2C_data.I2C_clientDataStruct.rgbLedsMode,
            I2C_data.I2C_clientDataStruct.rgbLedsBlinkTempo);

        userLed_configure(I2C_data.I2C_clientDataStruct.debugLedState);
    }

    I2C_data.I2C_clientDataStruct.proximityState = proximityButtonPressed;

}

void processButtonTouch(enum mtouch_button_names button)
{
    __nop();
}

void processButtonRelease(enum mtouch_button_names button)
{
    __nop();
}

// Dans touchTuneInit
//touchUartRxComplete
// touchUartTxComplete

void processProximityActivate(enum mtouch_proximity_names prox)
{
    proximityButtonPressed = true;
}

void processProximityNotActivate(enum mtouch_proximity_names prox)
{
    proximityButtonPressed = false;
}

void processProximityState(void)
{
    // If prox is detected before, wait for all deactivation
    if (proximityButtonPressed) {
        for (uint8_t i = 0; i < MTOUCH_PROXIMITY; i++) {
            if (MTOUCH_Proximity_State_Get(i) == true)
                break;
        }
        proximityButtonPressed = false;
    } else { // else detect the first proximity sensor
        for (uint8_t i = 0; i < MTOUCH_PROXIMITY; i++) {
            if (MTOUCH_Proximity_State_Get(i) == true) {
                proximityButtonPressed = true;
                break;
            }
        }
    }
}





//bool I2C_interruptHandler (i2c_client_transfer_event_t clientEvent);
//void I2C_InterruptHandler ()

int main(void)
{
    //test_unitaires();

    SYSTEM_Initialize();
    SYSTICK_initialize();
    LEDS_initialize();
    MTOUCH_Initialize();
    I2C_client_initialize(I2C_data.I2C_clientDataArray, sizeof(I2C_data.I2C_clientDataArray));

    MTOUCH_Button_SetPressedCallback(processButtonTouch);
    MTOUCH_Button_SetNotPressedCallback(processButtonRelease);
    MTOUCH_Proximity_SetActivatedCallback(processProximityActivate);
    MTOUCH_Proximity_SetNotActivatedCallback(processProximityNotActivate);

    //  I2C1_CallbackRegister (I2C_interruptHandler);

    // void I2C1_CallbackRegister(bool(*callback)(i2c_client_transfer_event_t clientEvent))

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable();

    uint32_t time = SYSTICK_read();
    while (1) {
        if (SYSTICK_isOverrun(time)) {
            time += 20;

            LEDS_serviceMainLoop();
            I2C_DATA_service_Mainloop();
        }

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
    }
}

// end of file
