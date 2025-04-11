/*
    MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:

    Subject to your compliance with the terms below and any license terms accompanying this software, you may use this software, and any derivatives created by any person or
    entity by or on your behalf, exclusively with Microchip's products.
    Microchip and its subsidiaries ("Microchip"), and its licensors, retain all
    ownership and intellectual property rights in the accompanying software and
    in all derivatives hereto.

    This software and any accompanying information is for suggestion only. It
    does not modify Microchip's standard warranty for its products.  You agree
    that you are solely responsible for testing the software and determining
    its suitability.  Microchip has no obligation to modify, test, certify, or
    support the software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S
    PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
    (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
    INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
    ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL
    LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED
    THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
    THESE TERMS.
*/
#ifndef MTOUCH_CONFIG_H
#define MTOUCH_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "mtouch_sensor.h"
#include "mtouch_button.h"


/*
 * =======================================================================
 * Application / Configuration Settings
 * =======================================================================
 */
    #define MTOUCH_SENSORS          9u
    #define MTOUCH_BUTTONS          9u

    #define MTOUCH_DEBUG_COMM_ENABLE      1u
#include "mtouch_datastreamer.h"

    #define MTOUCH_PROXIMITY_ENABLE 1u

    #define MTOUCH_PROXIMITY        9u

    /* 
     * =======================================================================
     * Sensor Parameters
     * =======================================================================
     */

    
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANC2              0x12
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANC2          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANC2        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANC2       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANC2      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANC7              0x17
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANC7          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANC7        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANC7       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANC7      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANC6              0x16
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANC6          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANC6        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANC6       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANC6      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANC3              0x13
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANC3          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANC3        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANC3       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANC3      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANA4              0x4
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANA4          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANA4        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANA4       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANA4      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANA5              0x5
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANA5          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANA5        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANA5       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANA5      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANC0              0x10
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANC0          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANC0        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANC0       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANC0      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANC1              0x11
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANC1          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANC1        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANC1       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANC1      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANA2              0x2
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANA2          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANA2        16u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANA2       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANA2      20u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ACTIVE_THRESHOLD               100u  

    /* 
     * =======================================================================
     * Button Parameters
     * =======================================================================
     */

    #define MTOUCH_BUTTON_READING_GAIN              ((uint8_t)2u)
    #define MTOUCH_BUTTON_BASELINE_GAIN             ((uint8_t)4u)
    #define MTOUCH_BUTTON_BASELINE_INIT             ((mtouch_button_baselinecounter_t)16u)
    #define MTOUCH_BUTTON_BASELINE_RATE             ((mtouch_button_baselinecounter_t)64u)
    #define MTOUCH_BUTTON_BASELINE_HOLD             ((mtouch_button_baselinecounter_t)1024u)
    #define MTOUCH_BUTTON_NEGATIVEDEVIATION         ((mtouch_button_statecounter_t)64u)
    #define MTOUCH_BUTTON_PRESSTIMEOUT              ((mtouch_button_statecounter_t)1000u)

    #define MTOUCH_BUTTON_SENSOR_Button0             Sensor_ANC2
    #define MTOUCH_BUTTON_SENSOR_Button1             Sensor_ANC7
    #define MTOUCH_BUTTON_SENSOR_Button2             Sensor_ANC6
    #define MTOUCH_BUTTON_SENSOR_Button3             Sensor_ANC3
    #define MTOUCH_BUTTON_SENSOR_Button4             Sensor_ANA4
    #define MTOUCH_BUTTON_SENSOR_Button5             Sensor_ANA5
    #define MTOUCH_BUTTON_SENSOR_Button6             Sensor_ANC0
    #define MTOUCH_BUTTON_SENSOR_Button7             Sensor_ANC1
    #define MTOUCH_BUTTON_SENSOR_Button8             Sensor_ANA2

    #define MTOUCH_BUTTON_THRESHOLD_Button0          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button1          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button2          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button3          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button4          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button5          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button6          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button7          50u
    #define MTOUCH_BUTTON_THRESHOLD_Button8          100u
    
    #define MTOUCH_BUTTON_SCALING_Button0            1u
    #define MTOUCH_BUTTON_SCALING_Button1            1u
    #define MTOUCH_BUTTON_SCALING_Button2            1u
    #define MTOUCH_BUTTON_SCALING_Button3            1u
    #define MTOUCH_BUTTON_SCALING_Button4            1u
    #define MTOUCH_BUTTON_SCALING_Button5            1u
    #define MTOUCH_BUTTON_SCALING_Button6            1u
    #define MTOUCH_BUTTON_SCALING_Button7            1u
    #define MTOUCH_BUTTON_SCALING_Button8            1u

    #define MTOUCH_BUTTON_COMMON_HYSTERESIS         HYST_50_PERCENT



 /* 
     * =======================================================================
     * Proximity Parameters
     * =======================================================================
     */

    #define MTOUCH_PROXIMITY_READING_GAIN           ((uint8_t)1u)
    #define MTOUCH_PROXIMITY_BASELINE_GAIN          ((uint8_t)4u)
    #define MTOUCH_PROXIMITY_BASELINE_INIT          ((mtouch_prox_baselinecounter_t)16u)
    #define MTOUCH_PROXIMITY_BASELINE_RATE          ((mtouch_prox_baselinecounter_t)128u)
    #define MTOUCH_PROXIMITY_BASELINE_HOLD          ((mtouch_prox_baselinecounter_t)1024u)
    #define MTOUCH_PROXIMITY_DEVIATION_GAIN         ((uint8_t)4u);
    #define MTOUCH_PROXIMITY_NEGATIVEDEVIATION      ((mtouch_prox_statecounter_t)32u)
    #define MTOUCH_PROXIMITY_ACTIVITYTIMEOUT        ((mtouch_prox_statecounter_t)1000u)

    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity0   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity1   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity2   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity3   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity4   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity5   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity6   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity7   100u
    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity8   100u
    
    #define MTOUCH_PROXIMITY_SCALING_Proximity0     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity1     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity2     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity3     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity4     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity5     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity6     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity7     0u
    #define MTOUCH_PROXIMITY_SCALING_Proximity8     0u
    
    #define MTOUCH_PROXIMITY_COMMON_HYSTERESIS       HYST_50_PERCENT


#endif // MTOUCH_CONFIG_H
/**
 End of File
*/
