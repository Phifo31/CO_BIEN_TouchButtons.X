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

#include "leds.h"
#include "systick.h"
#include "i2c_slave.h"


void test_unitaires(void);

volatile union {
    uint8_t I2C_clientDataArray[10];

    struct {
        uint8_t ident;
        uint8_t userLedState;
        LEDS_color_t ledsColor;
        LEDS_mode_t ledsMode;
        uint8_t touchBoutonState;
        uint8_t proximityState;
        uint8_t touchBoutonRawValue;
        uint8_t proximityRawValue;
    } I2C_clientDataStruct;
} IC2_data;

volatile uint8_t * I2C_clientDataArray = IC2_data.I2C_clientDataArray;

// Private variable
//volatile static uint8_t CLIENT_DATA[I2C_CLIENT_LOCATION_SIZE] = {
//    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09
//};

void DATA_initialize(void)
{
    IC2_data.I2C_clientDataStruct.ident = 0xb3;
    IC2_data.I2C_clientDataStruct.userLedState = ON;
    IC2_data.I2C_clientDataStruct.ledsMode = FADING_BLINK;
    IC2_data.I2C_clientDataStruct.ledsColor.blue = 10;
    IC2_data.I2C_clientDataStruct.ledsColor.red = 20;
    IC2_data.I2C_clientDataStruct.ledsColor.green = 40;

    IC2_data.I2C_clientDataStruct.touchBoutonState = 0;
    IC2_data.I2C_clientDataStruct.proximityState = 0;
    IC2_data.I2C_clientDataStruct.touchBoutonRawValue = 0;
    IC2_data.I2C_clientDataStruct.proximityRawValue = 0;
}

/**
 * 
 */
void DATA_service_Mainloop(void)
{
    if (I2C_stopReceived) {
        I2C_stopReceived = false;
        //= IC2_data.I2C_clientDataStruct.userLedState;

        //LEDS_configure(LEDS_color_t color, uint8_t intensity, LEDS_mode_t mode, uint16_t interval);

        leds_state.mode = IC2_data.I2C_clientDataStruct.ledsMode = FADING_BLINK;
        leds_state.color.blue = IC2_data.I2C_clientDataStruct.ledsColor.blue;
        leds_state.color.red = IC2_data.I2C_clientDataStruct.ledsColor.red;
        leds_state.color.green = IC2_data.I2C_clientDataStruct.ledsColor.green;
    }
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

//void processProximityActivate(enum mtouch_proximity_names prox)
//{
//    __nop();
//}
// 
//void processProximityNotActivate(enum mtouch_proximity_names prox)
//{
//    __nop();
//}



//bool I2C_interruptHandler (i2c_client_transfer_event_t clientEvent);
//void I2C_InterruptHandler ()

int main(void)
{
    test_unitaires();

    SYSTEM_Initialize();
    LEDS_initialize();
    SYSTICK_initialize();
    MTOUCH_Initialize();
//    I2C_client_initialize();

    DATA_initialize();

    MTOUCH_Button_SetPressedCallback(processButtonTouch);
    MTOUCH_Button_SetNotPressedCallback(processButtonRelease);
    //  MTOUCH_Proximity_SetActivatedCallback (processProximityActivate);
    //  MTOUCH_Proximity_SetNotActivatedCallback (processProximityNotActivate);

    //  I2C1_CallbackRegister (I2C_interruptHandler);

    // void I2C1_CallbackRegister(bool(*callback)(i2c_client_transfer_event_t clientEvent))

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 
    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 

    while (1) {
        //MTOUCH_Service_Mainloop();
        LEDS_serviceMainLoop();
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
