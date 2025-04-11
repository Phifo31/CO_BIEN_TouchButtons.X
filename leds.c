/*
 * Leds Driver File
 * 
 * @file leds.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the driver for adressable leds.
 *
 * @version LEDS Driver Version 1.0
 *
 * Ce fichier repecte les préconisation de Microchip AN1606
 * on retrouve les mêmes éléments dans la vidéo : https://www.youtube.com/watch?v=hHLMnJs87VE
 * 
 * Le problème : le PIC choisi n'est pas compatible avec la version MCC d'origine (celui de la vidéo)
 * Le MCC actuel ne permet pas de configurer correctement le SPI
 * 
 * Ce driver reprend les configs et simplifie le MCC en n'utilisant que le nécessaire.
 * 
 * Par ailleurs, les fonctionnalités réalisée par Dorian LB S12024/2025 sont reprises ici.
 * 
 * 
 */

#include <xc.h>
#include <stdbool.h>

#include "systick.h"
#include "leds.h"

LEDS_color_t leds [NUM_LEDS];
leds_state_t leds_state;

leds_state_t userLed;

/*! 
 * 
 * Configurations des périphériques  d'après AN1606 :
 *      TMR ==> TMR2    
 *      SPI ==> MSSP2
 *      PWM => PWM3
 *      CLC ==> CLC1
 */
void LEDS_initialize(void)
{
    // Initialization of TMR2 (copy from MCC generated code)
    T2CLKCON = (1 << _T2CLKCON_T2CS_POSN); // T2CS FOSC/4

    T2HLT = (0 << _T2HLT_T2MODE_POSN) // T2MODE Software control
        | (0 << _T2HLT_CSYNC_POSN) // CSYNC Not Synchronized
        | (0 << _T2HLT_CPOL_POSN) // CPOL Rising Edge
        | (0 << _T2HLT_T2PSYNC_POSN); // T2PSYNC Not Synchronized

    T2RST = (0 << _T2RST_T2RSEL_POSN); // T2RSEL T2CKIPPS pin
    T2PR = 0x4; // Period 625e-9s; Frequency 8000000Hz (32MHz, FOSC/4; Count 4)
    T2TMR = 0x0; // Timer Counter Register <= 0
    PIR1bits.TMR2IF = 0; // Reset interrupt flag
    T2CON = (0 << _T2CON_T2CKPS_POSN) // T2CKPS 1:1
        | (1 << _T2CON_TMR2ON_POSN) // TMR2ON on
        | (0 << _T2CON_T2OUTPS_POSN); // T2OUTPS 1:1

    // Initialization of MSSP2 
    SSP2STAT = (0 << _SSP2STAT_CKE_POSN) // Transmit occurs on the transition from Idle to Active clock state
        | (0 << _SSP2STAT_SMP_POSN); // Input data are sampled at the middle of data output time

    SSP2CON1 = (3 << _SSP2CON1_SSPM_POSN) // SPI Host mode: Clock = TMR2 output/2
        | (0 << _SSP2CON1_CKP_POSN) // Idle state for the clock is a low level
        | (1 << _SSP2CON1_SSPEN_POSN); // Enables the serial port 

    // Initialization of PWM3 (copy from MCC generated code)
    PWM3CON = 0x80; // PWMPOL active_hi; PWMEN enabled;
    PWM3DCH = 0x1; // PWMDCH 1; 
    PWM3DCL = 0xC0; // PWMDCL 3;
    PWMTMRS0bits.P3TSEL = 0x1; // PWM Timers Selection (PWM3 use TMR2)

    // Initialization de CLC1 (copy from MCC generated code)
    // (SPI_SCK & nSPI_SDO & PWM3) | (SPI_SCK & SPI_SDO)
    // Erreur possible entre SCK et nSDO (à vérifier)
    CLCSELECT = 0x0; // SLCT 0x0; 
    CLCnPOL = 0x1; // LCG1POL inverted; LCG2POL not_inverted; LCG3POL not_inverted; LCG4POL not_inverted; LCPOL not_inverted; 
    CLCnSEL0 = 0x15; // LCD1S PWM3_OUT; 
    CLCnSEL1 = 0x25; // LCD2S SPI2 SCK OUT;
    CLCnSEL2 = 0x24; // LCD3S SPI2 SDO OUT;
    CLCnSEL3 = 0x0; // LCD4S CLCIN0 (CLCIN0PPS);

    CLCnGLS0 = 0x5; // LCG1D1N enabled; LCG1D2N enabled ; all other disabled
    CLCnGLS1 = 0x10; // LCG2D3N enabled ;
    CLCnGLS2 = 0x8; // LCG3D2T enabled ;  all other disabled
    CLCnGLS3 = 0x20; // LCG4D3T enabled ;
    CLCDATA = 0x0; // LCOUT 0x00; 
    CLCnCON = 0x80; // LCMODE AND-OR; LCINTN disabled; LCINTP disabled; LCEN enabled;

    // Clear the CLC interrupt flag
    PIR2bits.CLC1IF = 0; // Reset interrupt flag

    // PPS and GPIO configuration
    RC4PPS = 0x01; // output CLC1 on RC4
    TRISCbits.TRISC4 = 0;
    ODCONCbits.ODCC4 = 1; // Open drain 

    // Data initialisation
    leds_state.size = 9;
    //        LEDS_color_t color; // Current color
    leds_state.intensity = UINT8_MAX; // Maximum brightness
    //        LEDS_mode_t mode; // Current mode (blinking, on, off, fading blink)
    leds_state.toggle = false; // ON/OFF state
    leds_state.togglePeriod = 5;
    leds_state.toggleCmpt = 1;
    leds_state.lastUpdate = 0; // Time of the last update
    leds_state.interval = 50; // Update interval in milliseconds
    leds_state.fadeLevel = 4; // Current fade level for fading blink
    leds_state.fadeDirection = 1; // Direction of fading (1 for up, -1 for down)  


    // Set the PWM5 to the options selected in the User Interface
    PWM5CON = 0x80; // PWMPOL active_hi; PWMEN enabled; 
    PWM5DCH = 0x7C; // PWMDCH 124
    PWM5DCL = 0xC0; // PWMDCL 3
    PWMTMRS0bits.P5TSEL = 0x3;
    RC5PPS = 0x0D; //RC5->PWM5:PWM5;
    TRISCbits.TRISC5 = 0;

    userLed.mode = FADING_BLINK;
    userLed.intensity = 250;
    userLed.color.red = 255;
    userLed.toggle = true;
    userLed.togglePeriod = 5;
    userLed.toggleCmpt = 1;
    userLed.lastUpdate = 0;
    userLed.interval = 50;
    userLed.fadeDirection = 1;
}

/**
 * 
 * @param i
 * @param j
 * @return 
 */
static inline uint8_t qsub8(uint8_t i, uint8_t j)
{
    if (i > j)
        return i - j;
    else
        return 0;
}

static inline uint8_t fadeToBlackBy(uint8_t v, uint8_t scale)
{
    uint16_t tmp = (uint16_t) v;
    tmp *= (uint16_t) scale;
    return(uint8_t) (tmp >> 8);
}

/*!
 * 
 * @param r
 * @param g
 * @param b
 */
static void LEDS_Write(uint8_t r, uint8_t g, uint8_t b)
{
    SSP2BUF = g; //SPI2TXB = g;
    while (PIR4bits.SSP2IF == 0); // while (!SPI2SPI2STATUSbits.TXBE);
    PIR4bits.SSP2IF = 0;

    SSP2BUF = r;
    while (PIR4bits.SSP2IF == 0);
    PIR4bits.SSP2IF = 0;

    SSP2BUF = b;
    while (PIR4bits.SSP2IF == 0);
    PIR4bits.SSP2IF = 0;
}

/*!
 * 
 * 
 */
static void LEDS_Show(void)
{
    for (uint8_t i = 0; i < leds_state.size; i++) {
        LEDS_Write(leds[i].red, leds[i].green, leds[i].blue);
    }
}

/*!
 * 
 * @param color
 * @param intensity
 * @param mode
 * @param interval
 */
void LEDS_configure(LEDS_color_t color, uint8_t intensity, LEDS_mode_t mode, uint16_t interval)
{
}

void userLedUpdate(void)
{
    int16_t fade_level;

    switch (userLed.mode) {
    case FADING_BLINK:
        fade_level = (int16_t) userLed.fadeLevel;
        fade_level += (int16_t) userLed.fadeDirection * (int16_t) 15; // Adjust fading step 
        if (fade_level <= 0) {
            userLed.fadeDirection = 1; // Reverse fading direction
            userLed.fadeLevel = 0;
        } else if (fade_level >= UINT8_MAX) {
            userLed.fadeDirection = -1; // Reverse fading direction
            userLed.fadeLevel = UINT8_MAX;
        } else {
            userLed.fadeLevel = (uint8_t) fade_level;
        }

        // Set LEDs to the current fade level
        uint8_t tmp = qsub8(userLed.color.red, UINT8_MAX - userLed.intensity);
        PWM5DCH = USER_LED_MAXPWM - fadeToBlackBy(tmp, UINT8_MAX - userLed.fadeLevel);
        break;

    case BLINK:
        if (--userLed.toggleCmpt == 0) {
            userLed.toggleCmpt = userLed.togglePeriod;
            userLed.toggle = !userLed.toggle;

            //PORTCbits.RC5 = (userLed.toggle) ? 0 : 1; // Apply ON/OFF state to LEDs
            if (userLed.toggle) {
                PWM5DCH = USER_LED_MAXPWM; // PWMDCH 124
                PWM5DCL = 0xC0; // PWMDCL 3
            } else {
                PWM5DCH = 0x7C; // PWMDCH 124
                PWM5DCL = 0xC0; // PWMDCL 3
            }
        }
        break;

    case ON:
        PORTCbits.RC5 = 0;
        break;

    case OFF:
        PORTCbits.RC5 = 1;
    default:
        break;
    }
}

/*!
 * Led update
 * 
 */
void LEDS_serviceMainLoop(void)
{
    int16_t fade_level;

    if (SYSTICK_isOverrun(leds_state.lastUpdate)) {
        leds_state.lastUpdate += leds_state.interval;

        switch (leds_state.mode) {
        case FADING_BLINK: // Update fade level
            fade_level = (int16_t) leds_state.fadeLevel;
            fade_level += (int16_t) leds_state.fadeDirection * (int16_t) 15; // Adjust fading step 
            if (fade_level <= 0) {
                leds_state.fadeDirection = 1; // Reverse fading direction
                leds_state.fadeLevel = 0;
            } else if (fade_level >= UINT8_MAX) {
                leds_state.fadeDirection = -1; // Reverse fading direction
                leds_state.fadeLevel = UINT8_MAX;
            } else {
                leds_state.fadeLevel = (uint8_t) fade_level;
            }

            // Set LEDs to the current fade level
            for (uint8_t i = 0; i < leds_state.size; i++) {
                uint8_t tmp = qsub8(leds_state.color.blue, UINT8_MAX - leds_state.intensity);
                leds[i].blue = fadeToBlackBy(tmp, UINT8_MAX - leds_state.fadeLevel);
                tmp = qsub8(leds_state.color.red, UINT8_MAX - leds_state.intensity);
                leds[i].red = fadeToBlackBy(tmp, UINT8_MAX - leds_state.fadeLevel);
                tmp = qsub8(leds_state.color.green, UINT8_MAX - leds_state.intensity);
                leds[i].green = fadeToBlackBy(tmp, UINT8_MAX - leds_state.fadeLevel);
            }
            break;

        case BLINK: // Toggle ON/OFF state
            if (--leds_state.toggleCmpt == 0) {
                leds_state.toggleCmpt = leds_state.togglePeriod;
                leds_state.toggle = !leds_state.toggle;
                for (uint8_t i = 0; i < leds_state.size; i++) { // Apply ON/OFF state to LEDs
                    leds[i].blue = (!leds_state.toggle) ? 0 : qsub8(leds_state.color.blue, UINT8_MAX - leds_state.intensity);
                    leds[i].red = (!leds_state.toggle) ? 0 : qsub8(leds_state.color.red, UINT8_MAX - leds_state.intensity);
                    leds[i].green = (!leds_state.toggle) ? 0 : qsub8(leds_state.color.green, UINT8_MAX - leds_state.intensity);
                }
            }
            break;

        case ON: // Apply ON state to LEDs
            for (uint8_t i = 0; i < leds_state.size; i++) {
                leds[i].blue = qsub8(leds_state.color.blue, UINT8_MAX - leds_state.intensity);
                leds[i].red = qsub8(leds_state.color.red, UINT8_MAX - leds_state.intensity);
                leds[i].green = qsub8(leds_state.color.green, UINT8_MAX - leds_state.intensity);
            }
            break;

        case OFF: // Apply OFF state to LEDs
        default:
            for (uint8_t i = 0; i < leds_state.size; i++) {
                leds[i].blue = 0;
                leds[i].red = 0;
                leds[i].green = 0;
            }
            break;
        }

        LEDS_Show();
        userLedUpdate();
    }
}

// End of file 

