/**
 * MSSP1 Generated Driver File
 *
 * @file mssp1.c
 *
 * @ingroup I2C_client
 *
 * @brief This file contains the API implementation for the I2C1 driver.
 *
 * @version I2C1 Driver Version 2.1.3
 * 
 * @version I2C1 Package Version 7.0.3
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

#include <xc.h>
#include "../mssp1.h"

/**
  Section: Driver Interface
 */
const i2c_client_interface_t I2C1_Client = {
    .Initialize = I2C1_Initialize,
    .Deinitialize = I2C1_Deinitialize,
    .WriteByte = I2C1_WriteByte,
    .ReadByte = I2C1_ReadByte,
    .TransferDirGet = I2C1_TransferDirGet,
    .LastByteAckStatusGet = I2C1_LastByteAckStatusGet,
    .ErrorGet = I2C1_ErrorGet,
    .CallbackRegister = I2C1_CallbackRegister,
    .Tasks = NULL
};

/**
 Section: Private functions declaration
 */
static void I2C1_EventHandler(void);
static void I2C1_ErrorEventHandler(void);

/**
 Section: Private Variable Definitions
 */
static volatile uint16_t i2c1Addr;
static volatile i2c_client_error_t i2c1ErrorState;
static bool(*I2C1_InterruptHandler)(i2c_client_transfer_event_t clientEvent);

/**
 Section: Public functions
 */
void I2C1_Initialize(void)
{
    /* CKE disabled; SMP Standard Speed;  */
    SSP1STAT = 0x80;
    /* SSPM 7 Bit Polling; CKP disabled; SSPEN disabled; SSPOV no_overflow; WCOL no_collision;  */
    SSP1CON1 |= 0x6;
    /* SEN enabled; RSEN disabled; PEN disabled; RCEN disabled; ACKEN disabled; ACKDT acknowledge; GCEN disabled;  */
    SSP1CON2 = 0x1;
    /* DHEN disabled; AHEN disabled; SBCDE disabled; SDAHT 100ns; BOEN disabled; SCIE disabled; PCIE disabled;  */
    SSP1CON3 = 0x0;
    /* SSPADD 112;  */
    SSP1ADD = 0x48;
    /* SSPMSK 254;  */
    SSP1MSK = 0xFE;
    /* Enable Interrupts */
    PIE3bits.SSP1IE = 1;
    PIE3bits.BCL1IE = 1;
    I2C1_InterruptHandler = NULL;
    SSP1CON1bits.SSPEN = 1;
    SSP1CON3bits.PCIE = 1;
    SSP1CON3bits.AHEN = 1;
    SSP1CON3bits.DHEN = 1;

}

void I2C1_Deinitialize(void)
{
    SSP1STAT = 0x00;
    SSP1CON1 |= 0x00;
    SSP1CON2 = 0x00;
    SSP1CON3 = 0x00;
    SSP1ADD = 0x08;
    SSP1MSK = 0xFE;
    /* Disable Interrupts */
    PIE3bits.SSP1IE = 0;
    PIE3bits.BCL1IE = 0;
    SSP1CON3bits.PCIE = 0;
}

void I2C1_WriteByte(uint8_t data)
{
    SSP1BUF = data;
}

uint8_t I2C1_ReadByte(void)
{
    return SSP1BUF;
}

uint16_t I2C1_ReadAddr(void)
{
    return(i2c1Addr >> 1);
}

i2c_client_error_t I2C1_ErrorGet(void)
{
    i2c_client_error_t error;
    error = i2c1ErrorState;
    i2c1ErrorState = I2C_CLIENT_ERROR_NONE;
    return error;
}

i2c_client_transfer_dir_t I2C1_TransferDirGet(void)
{
    return(SSP1STATbits.R_nW ? I2C_CLIENT_TRANSFER_DIR_READ : I2C_CLIENT_TRANSFER_DIR_WRITE);
}

i2c_client_ack_status_t I2C1_LastByteAckStatusGet(void)
{
    return(SSP1CON2bits.ACKDT ? I2C_CLIENT_ACK_STATUS_RECEIVED_NACK : I2C_CLIENT_ACK_STATUS_RECEIVED_ACK);
}

void I2C1_CallbackRegister(bool(*callback)(i2c_client_transfer_event_t clientEvent))
{
    if (callback != NULL) {
        I2C1_InterruptHandler = callback;
    }
}

void I2C1_ISR(void)
{
    I2C1_EventHandler();
}

void I2C1_ERROR_ISR(void)
{
    I2C1_ErrorEventHandler();
}

/**
 Section: Private functions
 */
static void I2C1_EventHandler(void)
{
    PIR3bits.SSP1IF = 0;
    if (0U != SSP1STATbits.P) {
        /* Notify that a stop event has occurred */
        I2C1_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_STOP_BIT_RECEIVED);
    } else if ((!SSP1STATbits.D_nA) && (0U != SSP1CON3bits.ACKTIM)) {
        /* Received I2C address must be read out */
        i2c1Addr = I2C1_ReadByte();
        /* Clear Software Error State */
        i2c1ErrorState = I2C_CLIENT_ERROR_NONE;
        /* Notify that a address match event has occurred */
        if (I2C1_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH) == true) {
            /* Send ACK */
            SSP1CON2bits.ACKDT = 0;
            SSP1CON2bits.ACKEN = 1;
        } else {
            /* Send NACK */
            SSP1CON2bits.ACKDT = 1;
            SSP1CON2bits.ACKEN = 1;
        }
    } else {
        /* Host reads from client, client transmits */
        if (0U != SSP1STATbits.R_nW) {
            if ((!SSP1STATbits.BF) && (!SSP1CON2bits.ACKSTAT)) {
                /* I2C host wants to read. In the callback, client must write to transmit register */
                if (I2C1_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_TX_READY) == false) {
                    /* Send NACK */
                    SSP1CON2bits.ACKDT = 1;
                    SSP1CON2bits.ACKEN = 1;
                } else {
                    /* Send ACK */
                    SSP1CON2bits.ACKDT = 0;
                    SSP1CON2bits.ACKEN = 1;
                }
            }
        } else {
            if (0U != SSP1STATbits.BF) {
                /* I2C host wants to write. In the callback, client must read data by calling I2Cx_ReadByte()  */
                if (I2C1_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_RX_READY) == true) {
                    /* Send ACK */
                    SSP1CON2bits.ACKDT = 0;
                    SSP1CON2bits.ACKEN = 1;
                } else {
                    /* Send NACK */
                    SSP1CON2bits.ACKDT = 1;
                    SSP1CON2bits.ACKEN = 1;
                }
            }
        }
    }

    /* Data written by the application; release the clock stretch */
    SSP1CON1bits.CKP = 1;
}

static void I2C1_ErrorEventHandler(void)
{
    if (0U != PIR3bits.BCL1IF) {
        i2c1ErrorState = I2C_CLIENT_ERROR_BUS_COLLISION;
        I2C1_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        PIR3bits.BCL1IF = 0; /* Clear the bus collision */
    } else if (0U != SSP1CON1bits.WCOL) {
        i2c1ErrorState = I2C_CLIENT_ERROR_WRITE_COLLISION;
        I2C1_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        SSP1CON1bits.WCOL = 0; /* Clear the Write collision */
    } else {
        i2c1ErrorState = I2C_CLIENT_ERROR_NONE;
    }

    /*Clear SSPOV bit if set*/
    if (0U != SSP1CON1bits.SSPOV) {
        SSP1CON1bits.SSPOV = 0;
    }
    /* Data written by the application; release the clock stretch */
    SSP1CON1bits.CKP = 1;
}
