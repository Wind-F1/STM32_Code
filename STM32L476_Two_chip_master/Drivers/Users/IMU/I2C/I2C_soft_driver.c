
#include "I2C_soft_driver.h"
#include "main.h"

/*software driver of I2C*/

#define SDAIN 0
#define SDAOUT 1


#define IIC_CLK_SET() HAL_GPIO_WritePin(I2C2_SCL_soft_GPIO_Port, I2C2_SCL_soft_Pin, GPIO_PIN_SET)
#define IIC_CLK_RESET() HAL_GPIO_WritePin(I2C2_SCL_soft_GPIO_Port, I2C2_SCL_soft_Pin, GPIO_PIN_RESET)

#define IIC_SDA_SET() HAL_GPIO_WritePin(I2C2_SDA_soft_GPIO_Port, I2C2_SDA_soft_Pin, GPIO_PIN_SET)
#define IIC_SDA_RESET() HAL_GPIO_WritePin(I2C2_SDA_soft_GPIO_Port, I2C2_SDA_soft_Pin, GPIO_PIN_RESET)

#define IIC_SDA_READ() HAL_GPIO_ReadPin(I2C2_SDA_soft_GPIO_Port, I2C2_SDA_soft_Pin)


short CharToShort(uint8_t cData[]);
uint8_t IICWriteData(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IICReadData(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
static void IIC_SDA_Dir(uint8_t n);
static void IIC_Delay(uint32_t count);
static void IIC_Start(void);
static void IIC_Stop(void);
static int IIC_WaitACK(void);
static void IIC_ACK(void);
static void IIC_NACK(void);
void IIC_WriteByte(uint8_t byte);
uint8_t IIC_ReadByte(uint8_t ACK);



short CharToShort(uint8_t cData[])
{
	return ((short)cData[1]<<8)|cData[0];
}

uint8_t IICWriteData(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	uint8_t count=0;
	IIC_Start();
	IIC_WriteByte(dev<<1);
	IIC_WaitACK();
	IIC_WriteByte(reg);
	IIC_WaitACK();
	for(count=0;count<length;count++)
	{
		IIC_WriteByte(data[count]);
		IIC_WaitACK();
	}
	IIC_Stop();
	return 1;
}


uint8_t IICReadData(uint8_t dev, uint8_t reg, uint8_t readLength, uint8_t *tempBuf)
{

	IIC_Start();
	IIC_WriteByte(dev<<1);
	IIC_WaitACK();
	
	
	IIC_WriteByte(reg);
	IIC_WaitACK();
	
	
	IIC_Start();
	IIC_WriteByte((dev<<1)+1);
	IIC_WaitACK();
	

	for(int count=0;count<readLength;count++)
	{
		if(count!=(readLength-1))
		{
			tempBuf[count]=IIC_ReadByte(1);
		}
		else
		{
			tempBuf[count]=IIC_ReadByte(0);
		}
	}
	IIC_Stop();
	return 0;
}




static void IIC_SDA_Dir(uint8_t n)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin=I2C2_SDA_soft_Pin;
	
	if (n)
	{
		GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull=GPIO_NOPULL;
		
	}
	else
	{
		GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull=GPIO_PULLDOWN;
	}
	HAL_GPIO_Init(I2C2_SDA_soft_GPIO_Port,&GPIO_InitStruct);
}

static void IIC_Delay(uint32_t count)
{
	count=count*2;
	while (count--);
}

static void IIC_Start()
{
	IIC_SDA_Dir(SDAOUT);
	IIC_CLK_SET();
	IIC_SDA_SET();
	
	IIC_Delay(5);
	IIC_SDA_RESET();
	IIC_Delay(5);
	IIC_CLK_RESET();
	
}

static void IIC_Stop()
{
	IIC_SDA_Dir(SDAOUT);
	IIC_SDA_RESET();
	IIC_CLK_RESET();
	IIC_Delay(5);
	IIC_CLK_SET();
	IIC_SDA_SET();
	IIC_Delay(5);
}


static int IIC_WaitACK(void)
{
	uint8_t timeout=0;
	IIC_SDA_Dir(SDAIN);
	IIC_SDA_SET();
	//IIC_CLK_RESET();
	//IIC_Delay(5);
	//IIC_CLK_SET();
	IIC_Delay(5);
	while(IIC_SDA_READ())
	{
		
		timeout++;
		if (timeout > 50)
		{
			IIC_SDA_Dir(SDAOUT);
			IIC_Stop();
			return -1;
		}
		IIC_Delay(5);
	}
	IIC_CLK_SET();
	IIC_Delay(5);
	IIC_CLK_RESET();
	//IIC_SDA_Dir(SDAOUT);	
	return 0;
}

static void IIC_ACK()
{
	IIC_CLK_RESET();
	IIC_SDA_Dir(SDAOUT);
	IIC_SDA_RESET();
	IIC_Delay(5);
	IIC_CLK_SET();
	IIC_Delay(5);
	IIC_CLK_RESET();
}


static void IIC_NACK()
{
	IIC_CLK_RESET();
	IIC_SDA_Dir(SDAOUT);
	IIC_SDA_SET();
	IIC_Delay(5);
	IIC_CLK_SET();
	IIC_Delay(5);
	IIC_CLK_RESET();
}


static void IIC_WriteByte(uint8_t txd)
{
	
	IIC_SDA_Dir(SDAOUT);
	IIC_CLK_RESET();
	for(int i=0;i<8;i++)
	{
		if (txd & 0x80)
		{
			IIC_SDA_SET();
		}
		else
		{
			IIC_SDA_RESET();
		}
		txd <<= 1;
		IIC_Delay(2);
		IIC_CLK_SET();
		IIC_Delay(5);
		IIC_CLK_RESET();
		IIC_Delay(3);
	}
}


uint8_t IIC_ReadByte(uint8_t ACK)
{
	uint8_t tmp=0;
	IIC_SDA_Dir(SDAIN);

	for (int i=0;i<8;i++)
	{
		IIC_CLK_RESET();
		IIC_Delay(5);
		IIC_CLK_SET();
		tmp<<=1;
		if (IIC_SDA_READ()) tmp ++;
		IIC_Delay(5);
	}
	
	if (ACK) IIC_ACK();
	else IIC_NACK();
	
	return tmp;
}



