//===========================================================================		
/*
	* Change Logs :
	* Date           Author       Notes
	* 2015-03-23     Cris	      the first version
*/
//===========================================================================		
#include	"voice_test.h"

//===========================================================================		
/*
  @brief		register definition.                  
*/
//===========================================================================
bzuuzer_t	buzzer;

//===========================================================================		
/*
  @brief		buzzer_scan_freq.  		
  
  @details		�w�q���ﾹ1cnt���ɶ�.
  
  @note			�`�N�ҩ񪺮ɧ�.
*/
//===========================================================================		
void	buzzer_scan_freq(void)
{
	buzzer.bz_scan_time_base++;
	
	if(buzzer.bz_scan_time_base >= BZ_SACN_FREQ)	
	{
		buzzer.bz_scan_time_base = 0;	
		
		buzzer_tmd();
	}	
}

//===========================================================================		
/*
  @brief		buzzer.  		
  
  @details		buzzer time division multiplexing.
  
  @note			�`�N�ҩ񪺮ɧ�.
*/
//===========================================================================		
void	buzzer_tmd(void)
{
	if((buzzer.bz_cnt & 0x02) == 0x02)
	{
		_BUZZER =~ _BUZZER;	
	}
	
	if(buzzer.bz_cnt == 0)
	{
		buzzer_pwm_off();		
	}
}


//===========================================================================		
/*
  @brief		Enalbe buzzer.  		
*/
//===========================================================================		
void	buzzer_pwm_on(void)
{
	_BUZZER = 1;	
}

//===========================================================================		
/*
  @brief		Disable buzzer.  		
*/
//===========================================================================		
void	buzzer_pwm_off(void)
{	     
	_BUZZER = 0;	
}

//===========================================================================		
/*
  @brief		buzzer_count_dec 	
  
  @details		bz_cnt != 0�ɡA�C�h�[bz_cnt��1.
  
  @note			�`�N�ҩ񪺮ɧ�.	
*/
//===========================================================================		
void	buzzer_count_dec(void)
{
	if(buzzer.bz_cnt != 0)	
	{
		buzzer.bz_cnt--;	
	}
}

//===========================================================================		
/*
  @brief		
  
  @details		
  
  @note				
*/
//===========================================================================		
void	buzzer_count_set(uint8_t  bz_count)
{
	buzzer.bz_cnt = bz_count;		
}

//===========================================================================		
/*
  @brief		
  
  @details		
  
  @note				
*/
//===========================================================================		
void	buzzer_count_time(void)
{
	buzzer.bz_count_dec_time++;
	
	if(buzzer.bz_count_dec_time >= BZ_COUNT_FREQ)	
	{
		buzzer.bz_count_dec_time = 0;
		
		buzzer_count_dec();		
	}
}

