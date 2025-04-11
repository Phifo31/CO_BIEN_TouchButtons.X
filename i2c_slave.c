/**
 * 
 * I2C slave  
 * 
 * En lien avec le driver I2C_client de MCC
 * 
 * Recup de l'exemple MCC Melody API Reference ver. 23
 * https://onlinedocs.microchip.com/oxy/GUID-420E6AAC-9141-47BF-A4C7-A6EA17246D0D-en-US-23/GUID-6E279462-9E94-410C-9A48-7D719FFBC0AF.html
 * 
 * 
 * Passage de l'ensemble des données par un tableau mémoire 
 *  
 * 
 */

#include <xc.h>
#include "mcc_generated_files/i2c_client/mssp1.h"

#include "i2c_slave.h"


static uint8_t clientLocation = 0x00;
static bool isClientLocation = false;

bool I2C_stopReceived = false;

/**
 * Application 
 * 
 * @param event
 * @return 
 */
static bool I2C_client_application(i2c_client_transfer_event_t event) {
    switch (event) {
        case I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH: //Address Match Event
            if (I2C1_Client.TransferDirGet() == I2C_CLIENT_TRANSFER_DIR_WRITE) {
                isClientLocation = true;
            }
            break;

        case I2C_CLIENT_TRANSFER_EVENT_RX_READY: //Read the data sent by I2C Host
            if (isClientLocation) {
                clientLocation = I2C1_Client.ReadByte();
                isClientLocation = false;
                break;
            } else {
                I2C_CLIENT_DATA[clientLocation++] = I2C1_Client.ReadByte();
                if (clientLocation >= I2C_CLIENT_LOCATION_SIZE) {
                    clientLocation = 0x00;
                }
            }
            break;

        case I2C_CLIENT_TRANSFER_EVENT_TX_READY: //Provide the Client data requested by the I2C Host
            I2C1_Client.WriteByte(I2C_CLIENT_DATA[clientLocation++]);
            if (clientLocation >= I2C_CLIENT_LOCATION_SIZE) {
                clientLocation = 0x00;
            }
            break;

        case I2C_CLIENT_TRANSFER_EVENT_STOP_BIT_RECEIVED: //Stop Communication
            clientLocation = 0x00;
            I2C_stopReceived = true;
            break;

        case I2C_CLIENT_TRANSFER_EVENT_ERROR: //Error Event Handler
            clientLocation = 0x00;
            i2c_client_error_t errorState = I2C1_Client.ErrorGet();
            if (errorState == I2C_CLIENT_ERROR_BUS_COLLISION) {
                // Bus Collision Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_WRITE_COLLISION) {
                // Write Collision Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_RECEIVE_OVERFLOW) {
                // Receive Overflow Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_TRANSMIT_UNDERFLOW) {
                // Transmit Underflow Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_READ_UNDERFLOW) {
                // Read Underflow Error Handling
            }
            break;

        default:
            break;
    }
    return true;
}

void I2C_client_initialize (void) {
    I2C1_Client.CallbackRegister(I2C_client_application);
}


// End of file
