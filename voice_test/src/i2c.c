//===========================================================================		
/*
	* Change Logs :
	* Date           Author       Notes
	* 2015-03-23     Cris	      the first version
	* 2015-09-15	 Cris		  �ק�I2C delay�ɶ��A�[�ֳt��.
*/
//===========================================================================		
#include	"voice_test.h"

//===========================================================================		
/*
  @brief		sda gpio config.
  
  @details		config sda pin hi or low
  
  @note			(1)�u��out low��input(�Q�Ψ�pull up)                
                
  @param[in]	pins_state			0 -> output low�A1 -> input hi
*/
//===========================================================================
void	sda_gpio_cfg(uint8_t  pins_state)
{
	if(pins_state == 0)	
	{
		_SDA_TRIS = 0;
		_SDA = 0;								
	}
	else
	{
		_SDA_TRIS = 1;
	}	
}

//===========================================================================		
/*
  @brief		scl gpio config.
  
  @details		config scl pin hi or low
  
  @note			(1)�u��out low��input(�Q�Ψ�pull up)
                                
  @param[in]	pins_state			0 -> output low�A1 -> input hi
*/
//===========================================================================
void	scl_gpio_cfg(uint8_t  pins_state)
{
	if(pins_state == 0)	
	{
		_SCL_TRIS = 0;
		_SCL = 0;								
	}
	else
	{
		_SCL_TRIS = 1;
	}	
}

//===========================================================================		
/*
  @brief		i2c start
  
  @details		This routine will send the I2C Start Bit.
  	                                
*/
//===========================================================================
void	i2c_start(void)
{
	sda_gpio_cfg(1);
	scl_gpio_cfg(1);
	delay_i2c_1();
	sda_gpio_cfg(0);
	delay_i2c_1();
	scl_gpio_cfg(0);
}

//===========================================================================		
/*
  @brief		i2c stop
  
  @details		This routine will send the I2C Stop Bit.
  	                                
*/
//===========================================================================
void	i2c_stop(void)
{
	sda_gpio_cfg(0);
	delay_i2c_1();
	scl_gpio_cfg(0);
	delay_i2c_1();
	scl_gpio_cfg(1);
	delay_i2c_1();
	sda_gpio_cfg(1);
}

//===========================================================================		
/*
  @brief		i2c clock
  
  @details		This routine will send the I2C clock pulse.
  	                                
*/
//===========================================================================
void	i2c_clock(void)
{
	delay_i2c_2();
	scl_gpio_cfg(1);
	delay_i2c_2();
	scl_gpio_cfg(0);
}

//===========================================================================		
/*
  @brief		write data
  
  @details		This routine will write a byte to the I2C device.

  @param[in]  	data		// ���g�J�����
  
  @return		ack			// ack = 0 -> �ǰe���ѡA ack = 1 -> �ǰe���\.                                
*/
//===========================================================================
uint8_t  i2c_byte_wirte(uint8_t  data)
{
	uint8_t		i,j;
	uint8_t		ack = 1;
	
	for(i = 0 ; i < 8 ; i++)	
	{
		j = data & 0x80;
		
		if(j == 0)	
		{
			sda_gpio_cfg(0);	
		}
		else
		{
			sda_gpio_cfg(1);
		}
		
		i2c_clock();
		data = data << 1;
	}
	
//===========================================================================
// Acknowledge
	sda_gpio_cfg(1);
	delay_i2c_2();	
	scl_gpio_cfg(1);
	delay_i2c_2();
	
	if(_SDA == 1)
	{
		ack = 0;			// �ǰe����.
	}

//===========================================================================
	i2c_check_scl_state();		
	scl_gpio_cfg(0);
	
	return	ack;
}

//===========================================================================		
/*
  @brief		read data
  
  @details		This routine will read a byte to the I2C device.
  
  @return		data			
*/
//===========================================================================
uint8_t		i2c_byte_read(void)
{
	uint8_t		i;
	uint8_t		data = 0;
	
	for(i = 0 ; i < 8 ; i++)
	{
		scl_gpio_cfg(1);
		delay_i2c_1();	
		
		data = data << 1;
		
		if(_SDA == 1)
		{
			data = data | 0x01;	
		}
		
		scl_gpio_cfg(0);
		delay_i2c_1();				
	}
	
	return	data;	
}

//===========================================================================		
/*
  @brief		i2c read ack
  
  @details		Ū��I2C��ƮɡA��Ack�i��Slave������.  			
*/
//===========================================================================
void	i2c_read_ack(void)
{
	sda_gpio_cfg(0);
	delay_i2c_1();	
	scl_gpio_cfg(1);
	delay_i2c_1();		
	i2c_check_scl_state();
	scl_gpio_cfg(0);
	delay_i2c_1();
	sda_gpio_cfg(1);
	delay_i2c_1();
}

//===========================================================================		
/*
  @brief		i2c read nack
  
  @details		Ū��I2C��ƮɡA��Ack�i��Slave�����ơA���̫�@�Ӭ�NACK.  			
  
  @note			�Ѿ\24LC256�W���.
*/
//===========================================================================
void	i2c_read_nack(void)
{
	sda_gpio_cfg(1);
	delay_i2c_1();
	scl_gpio_cfg(1);
	delay_i2c_1();
	i2c_check_scl_state();	
	scl_gpio_cfg(0);
	delay_i2c_1();
}

//===========================================================================		
/*
  @brief		i2c check scl state
  
  @details		(1)clock stretching.
  				(2)Slave�ݨӤ��Φ^���A�����u(SCL��Lo).
  
  @note			�����]�@���� 19 �ӫ��O�g�� : 19us @ 4MHz.
                                             9.5us @ 8MHz
   										     4.xx @ 16MHz            
*/
//===========================================================================
void	i2c_check_scl_state(void)
{
	uint8_t		dly = 0xff;
	
	while(dly)	
	{
		if(_SCL == 1)	
		{
			return;
		}
		
		dly--;
	}	
}

//===========================================================================		
/*
  @brief		delay_i2c_1.
  
  @details		delay 5us.
  
  @note			(1)�ݪ`�N���O�g��.
  				(2)Delay1KTCYx���p��覡: 1/( osc / 4 )  = 0.5 us .
                
*/
//===========================================================================
void	delay_i2c_1(void)
{
	Nop();
	Nop();	
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();	
}


//===========================================================================		
/*
  @brief		delay_i2c_2.
  
  @details		delay 2us.
  
  @note			(1)�ݪ`�N���O�g��.
  				(2)Delay1KTCYx���p��覡: 1/( osc / 4 ) = 0.5 us .
                
*/
//===========================================================================
void	delay_i2c_2(void)
{
	Nop();
	Nop();	
	Nop();
	Nop();
	
}