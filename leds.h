/* 
 * File:   leds.h
 * Author: phifo
 *
 * Created on 7 mars 2025, 15:25
 */

#ifndef LEDS_H
#define	LEDS_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum LEDS_MODE {
        ON,
        OFF,
        BLINK,
        FADING_BLINK
    } LEDS_mode_t;

    typedef struct LEDS_COLOR {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } LEDS_color_t;

    typedef struct LEDS_STRUCT {
        uint8_t *indices; // Indices of LEDs in the group
        uint8_t size; //<= Number of LEDs in the group
        //CRGB color; // Current color
        LEDS_color_t color; // Current color
        uint8_t intensity; // Maximum brightness

        LEDS_mode_t mode; // Current mode (blinking, on, off, fading blink)
        bool toggle; // ON/OFF state
        uint8_t togglePeriod;
        uint8_t toggleCmpt;
        uint32_t lastUpdate; // Time of the last update
        uint32_t interval; // Update interval in milliseconds
        uint8_t fadeLevel; // Current fade level for fading blink
        int8_t fadeDirection; // Direction of fading (1 for up, -1 for down)  
    } leds_state_t;


#define NUM_LEDS 9       // Nombre de leds
#define FADING_BLINK_SPEED 50

#define USER_LED_MAXPWM 0xF9

    extern LEDS_color_t leds [NUM_LEDS];
    extern leds_state_t leds_state;

    /*!
     * 
     */
    void LEDS_initialize(void);



    /*!
     * 
     * @param color
     * @param intensity
     * @param mode
     * @param interval
     */
    void LEDS_configure(LEDS_color_t color, uint8_t intensity, LEDS_mode_t mode, uint16_t interval);


    /*!
     * Led update
     * 
     */
    void LEDS_serviceMainLoop(void);


    /*!
     * 
     * 
     */
    //void LEDS_Show(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LEDS_H */

