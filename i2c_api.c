#include "i2c_api.h"
void I2C_BufferRead(I2C_TypeDef* I2C,uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    /* While the bus is busy */
    while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2C, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C, 0xEE, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the EEPROM's internal address to read from: MSB of the address first */
    I2C_SendData(I2C, ReadAddr);

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send START condition a second time */
    I2C_GenerateSTART(I2C, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C, 0xEF, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while(NumByteToRead)
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C, ENABLE);
        }

    /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2C);

            /* Point to the next location where the byte read will be saved */
            pBuffer++;

            /* Decrement the read bytes counter */
            NumByteToRead--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C, ENABLE);
}
void I2C_WriteByte(I2C_TypeDef* I2C,uint8_t address, uint8_t byte)
{
   /* While the bus is busy */
	    while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));

	    /* Send START condition */
	    I2C_GenerateSTART(I2C, ENABLE);

	    /* Test on EV5 and clear it */
	    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

	    /* Send EEPROM address for write */
	    I2C_Send7bitAddress(I2C, 0xEE, I2C_Direction_Transmitter);

	    /* Test on EV6 and clear it */
	    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	    /* Send the EEPROM's internal address to write for: MSB of the address first */
	    I2C_SendData(I2C, address);

	    /* Test on EV8 and clear it */
	    while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	    /* Send the EEPROM's internal address to write for: MSB of the address first */
	    I2C_SendData(I2C, byte);

	    /* Test on EV8 and clear it */
	     while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

         /* Send STOP Condition */
         I2C_GenerateSTOP(I2C, ENABLE);

}
