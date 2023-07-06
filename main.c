/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ESPDataLogger.h"
#include <stdio.h>
#include <stdbool.h>
#include <ssd1306.h>
#include <fonts.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SoundSpd 0.0343
#define set 1
#define reset 0
#define r 0
#define g 1
#define y 2
#define sensor_0 0
#define sensor_1 1
#define sensor_2 2
#define sensor_3 3

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_2

#define LIGHTSENSOR_PORT GPIOB
#define LIGHTSENSOR_PIN GPIO_PIN_6

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

I2C_HandleTypeDef hi2c3;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart3;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

//Traffic_1

//Green light
GPIO_TypeDef * T1GPort = GPIOD;
uint16_t T1GPin = GPIO_PIN_14;

//Yellow light
GPIO_TypeDef * T1YPort = GPIOD;
uint16_t T1YPin = GPIO_PIN_15;

//Red light
GPIO_TypeDef * T1RPort = GPIOF;
uint16_t T1RPin = GPIO_PIN_12;

//Traffic_2

//Green light
GPIO_TypeDef * T2GPort = GPIOF;
uint16_t T2GPin = GPIO_PIN_13;

//Yellow light
GPIO_TypeDef * T2YPort = GPIOE;
uint16_t T2YPin = GPIO_PIN_9;

//Red light
GPIO_TypeDef * T2RPort = GPIOE;
uint16_t T2RPin = GPIO_PIN_11;

//Traffic_3

//Green light
GPIO_TypeDef * T3GPort = GPIOF;
uint16_t T3GPin = GPIO_PIN_14;

//Yellow light
GPIO_TypeDef * T3YPort = GPIOE;
uint16_t T3YPin = GPIO_PIN_13;

//Red light
GPIO_TypeDef * T3RPort = GPIOF;
uint16_t T3RPin = GPIO_PIN_15;

//Traffic_4

//Green light
GPIO_TypeDef * T4GPort = GPIOD;
uint16_t T4GPin = GPIO_PIN_8;

//Yellow light
GPIO_TypeDef * T4YPort = GPIOD;
uint16_t T4YPin = GPIO_PIN_9;

//Red light
GPIO_TypeDef * T4RPort = GPIOE;
uint16_t T4RPin = GPIO_PIN_8;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
float DistSensor(int SenNumb);
void TrfcSwch(bool flag);
void TrfcSet(uint8_t TrafficNumber, uint8_t color, bool state);
void Delay_us(uint16_t time);
void DHT11(void);
void DHT11_start (void);
uint8_t DHT_Chck_Resp (void);
uint8_t DHT11_Read (void);
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void update_screenT_H(void);
void TrafficLightSystem(void);
void pollution_sensor(void);
void light_sensor(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC3_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t Value_Buf [4] = {1,2,3,4};
int value1;
int value2;
int value3;

float Distance_0 = 0;
float Distance_1 = 0;
float Distance_2 = 0;
float Distance_3 = 0;

uint32_t TimeT1 = 0;
uint32_t TimeT2 = 0;
uint32_t RealTime1 = 0;
uint32_t RealTime2 = 0;
uint32_t GCntTotalDiff = 0;
uint32_t GCntTotal_1stLane = 0;
uint32_t GCntTotal_2ndLane = 0;

float Temperature;
float Humidity;
uint8_t Presence = 0;

uint32_t pollution;
double voltage;

bool light = false;
int state;
int Light_val;
const int initial_delay = 8500;
const int yellow_delay = 5000;
const int distance_delay = 12000;
uint8_t light_source[4];


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  SSD1306_Init();
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);



  //Initialising traffic lights

   TrfcSet(1, r, reset);
   TrfcSet(1, y, reset);
   TrfcSet(1, g, set);

   TrfcSet(2, r, set);
   TrfcSet(2, y, reset);
   TrfcSet(2, g, reset);

   TrfcSet(3, r, reset);
   TrfcSet(3, y, reset);
   TrfcSet(3, g, set);

   TrfcSet(4, r, set);
   TrfcSet(4, y, reset);
   TrfcSet(4, g, reset);

   TimeT1 = 30000 - initial_delay - yellow_delay;
   TimeT2 = 30000 - yellow_delay;
   RealTime1 = 30000;
   RealTime2 = 30000;

   //HAL_GPIO_WritePin(LIGHTSENSOR_PORT, LIGHTSENSOR_PIN, GPIO_PIN_RESET);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  ESP_Init("FBI SURVEILLANCE VAN 2.4G","parolanetwork");

	  light_sensor();
	  HAL_Delay(100);

	  DHT11();
	  HAL_Delay(100);

	  pollution_sensor();
	  HAL_Delay(50);

	  TrafficLightSystem();

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void TrfcSet(uint8_t TrafficNumber, uint8_t color, bool state)
{
	switch(TrafficNumber)
	{
	case 1:

		if(color == 0)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T1RPort, T1RPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T1RPort, T1RPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 1)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T1GPort, T1GPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T1GPort, T1GPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 2)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T1YPort, T1YPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T1YPort, T1YPin, GPIO_PIN_RESET);
			}
		}

		break;

	case 2:

		if(color == 0)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T2RPort, T2RPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T2RPort, T2RPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 1)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T2GPort, T2GPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T2GPort, T2GPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 2)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T2YPort, T2YPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T2YPort, T2YPin, GPIO_PIN_RESET);
			}
		}

		break;

	case 3:

		if(color == 0)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T3RPort, T3RPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T3RPort, T3RPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 1)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T3GPort, T3GPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T3GPort, T3GPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 2)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T3YPort, T3YPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T3YPort, T3YPin, GPIO_PIN_RESET);
			}
		}

		break;

	case 4:

		if(color == 0)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T4RPort, T4RPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T4RPort, T4RPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 1)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T4GPort, T4GPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T4GPort, T4GPin, GPIO_PIN_RESET);
			}
		}
		else if (color == 2)
		{
			if(state)
			{
				HAL_GPIO_WritePin(T4YPort, T4YPin, GPIO_PIN_SET);
			}
			else if (!state)
			{
				HAL_GPIO_WritePin(T4YPort, T4YPin, GPIO_PIN_RESET);
			}
		}

		break;

	}
}

float DistSensor(int SenNumb)
{
	uint32_t pMillis;
	uint32_t Value1 = 0;
	uint32_t Value2 = 0;

	GPIO_TypeDef * TrigPort = 0;
	uint16_t TrigPin = 0;
	GPIO_TypeDef * EchoPort = 0;
	uint16_t EchoPin = 0;

	float RetlVal = 0;

	switch (SenNumb)
	{

	case 0:
		TrigPort = GPIOA;
		TrigPin = GPIO_PIN_3;
		EchoPort = GPIOD;
		EchoPin = GPIO_PIN_7;
		break;

	case 1:
		TrigPort = GPIOC;
		TrigPin = GPIO_PIN_0;
		EchoPort = GPIOD;
		EchoPin = GPIO_PIN_6;
		break;

	case 2:
		TrigPort = GPIOC;
		TrigPin = GPIO_PIN_3;
		EchoPort = GPIOD;
		EchoPin = GPIO_PIN_5;
		break;

	case 3:
		TrigPort = GPIOC;
		TrigPin = GPIO_PIN_1;
		EchoPort = GPIOD;
		EchoPin = GPIO_PIN_4;
		break;

	}

	HAL_GPIO_WritePin(TrigPort, TrigPin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
			__HAL_TIM_SET_COUNTER(&htim2, 0);
			    while (__HAL_TIM_GET_COUNTER (&htim2) < 10)  // wait for 10 us
			    {
			    	//does nothing
			    }
					   HAL_GPIO_WritePin(TrigPort, TrigPin, GPIO_PIN_RESET);  // pull the TRIG pin low

					   pMillis = HAL_GetTick(); // used this to avoid infinite while loop  (for timeout)
					   // wait for the echo pin to go high
					   while (!(HAL_GPIO_ReadPin (EchoPort, EchoPin)) && pMillis + 10 >  HAL_GetTick());
					   Value1 = __HAL_TIM_GET_COUNTER (&htim2);

					   pMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
					      // wait for the echo pin to go low
					   while ((HAL_GPIO_ReadPin (EchoPort, EchoPin)) && pMillis + 50 > HAL_GetTick());
					   Value2 = __HAL_TIM_GET_COUNTER (&htim2);

					   RetlVal = (float)(Value2-Value1)* SoundSpd/2;
					   HAL_Delay(50);

 return RetlVal;

}

void TrfcSwch(bool flag)
{

		if(!flag) // 1/3 become green
		{


			HAL_GPIO_WritePin(T1YPort, T1YPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T3YPort, T3YPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T2YPort, T2YPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T4YPort, T4YPin, GPIO_PIN_SET);

			HAL_Delay(5000);

			HAL_GPIO_WritePin(T1YPort, T1YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T3YPort, T3YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T2YPort, T2YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T4YPort, T4YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T1RPort, T1RPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T3RPort, T3RPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T2GPort, T2GPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T4GPort, T4GPin, GPIO_PIN_RESET);


			HAL_GPIO_WritePin(T1GPort, T1GPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T3GPort, T3GPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T2RPort, T2RPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T4RPort, T4RPin, GPIO_PIN_SET);
		}
		else if(flag)  //2/4 become green
		{

			HAL_GPIO_WritePin(T1YPort, T1YPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T3YPort, T3YPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T2YPort, T2YPin, GPIO_PIN_SET);

			HAL_GPIO_WritePin(T4YPort, T4YPin, GPIO_PIN_SET);

			HAL_Delay(5000);

			HAL_GPIO_WritePin(T1YPort, T1YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T3YPort, T3YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T2YPort, T2YPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T4YPort, T4YPin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(T1GPort, T1GPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T3GPort, T3GPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T2RPort, T2RPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(T4RPort, T4RPin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(T1RPort, T1RPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T3RPort, T3RPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T2GPort, T2GPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(T4GPort, T4GPin, GPIO_PIN_SET);

		}

}

void Delay_us(uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim3))<time);
}

void DHT11(void)
{
	uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
	uint16_t SUM, RH, TEMP;

	DHT11_start();

	Presence = DHT_Chck_Resp();

	Rh_byte1 = DHT11_Read();
	Rh_byte2 = DHT11_Read();

	Temp_byte1 = DHT11_Read();
	Temp_byte2 = DHT11_Read();

	SUM = DHT11_Read();

	TEMP = Temp_byte1;
	RH = Rh_byte1;

	Temperature = (float) TEMP;
	Humidity = (float) RH;

	update_screenT_H();

	ESP_Send_Data("GET /post-data.php?api_key=%s&humidity=%f&temperature=%f HTTP/1.1\r\nHost: proven-gaps.000webhostapp.com\r\nConnection: close\r\n\r\n",
	                    "%2354321", Temperature, Humidity);


}

void DHT11_start (void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);     //Initialize with Data pin HIGH
	HAL_Delay(300);
	Set_Pin_Output (DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
	Delay_us (18000);   // wait for 18ms
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
	Delay_us (20);   // wait for 20us
	Set_Pin_Input(DHT11_PORT, DHT11_PIN);    // set as input
}

uint8_t DHT_Chck_Resp (void)
{
	uint8_t Response = 0;
	Delay_us (40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		Delay_us (80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
		else Response = -1;
	}
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go low

	return Response;
}

uint8_t DHT11_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go high
		Delay_us (40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for the pin to go low
	}
	return i;
}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void pollution_sensor(void)
{
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	  pollution = HAL_ADC_GetValue(&hadc1);
	  voltage = (double)(pollution*5)/4095;
	  ESP_Send_Data("GET /post-data.php?api_key=%s&pollution=%d HTTP/1.1\r\nHost: proven-gaps.000webhostapp.com\r\nConnection: close\r\n\r\n",
	  	                    "%2354321", pollution);
}

void light_sensor(void)
{
    	  HAL_ADC_Start(&hadc3);
    	  HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
    	  Light_val = HAL_ADC_GetValue(&hadc3);
    	  HAL_Delay(200);


		if (Light_val < 2900)
		{
			//state = HAL_GPIO_ReadPin(LIGHTSENSOR_PORT, LIGHTSENSOR_PIN);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 1);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, 1);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
			light_source[0] = 1;
			light_source[1] = 1;
			light_source[2] = 1;
			light_source[3] = 1;
		}
		else
		{
			//state = HAL_GPIO_ReadPin(LIGHTSENSOR_PORT, LIGHTSENSOR_PIN);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 0);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, 0);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);
			light_source[0] = 0;
			light_source[1] = 0;
			light_source[2] = 0;
			light_source[3] = 0;
		}
		ESP_Send_Data("GET /post-data.php?api_key=%s&light=%d&light_sensor_1=%d&light_sensor_2=%d&light_sensor_3=%d&light_sensor_4=%d HTTP/1.1\r\nHost: proven-gaps.000webhostapp.com\r\nConnection: close\r\n\r\n",
		                    "%2354321", Light_val, light_source[0], light_source[1], light_source[2], light_source[3]);
}

void update_screenT_H(void)
{
	  char str1[20] = {0};
	  char str2[20] = {0};

	  sprintf(str1, "Humidity: %.2f %%", Humidity);
	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts (str1, &Font_7x10, 1);

	  sprintf(str2, "Temperature: %d C", (int)Temperature);
	  SSD1306_GotoXY (0, 13);
	  SSD1306_Puts (str2, &Font_7x10, 1);
	  SSD1306_UpdateScreen(); //display
}

void TrafficLightSystem(void)
{
	ESP_Send_Data("GET /post-data.php?api_key=%s&lane1_green=%d&lane1_red=%d&lane2_green=%d&lane2_red=%d HTTP/1.1\r\nHost: proven-gaps.000webhostapp.com\r\nConnection: close\r\n\r\n",
		  	 	  		                    "%2354321", RealTime1/1000, RealTime2/1000, RealTime2/1000, RealTime1/1000);
		 // HAL_Delay(50);

		  Distance_0 = DistSensor(sensor_0);
		  Distance_1 = DistSensor(sensor_1);
		  Distance_2 = DistSensor(sensor_2);
		  Distance_3 = DistSensor(sensor_3);

		  printf("Distance_0 = %f\n", Distance_0);
		  printf("Distance_1 = %f\n", Distance_1);
		  printf("Distance_2 = %f\n", Distance_2);
		  printf("Distance_3 = %f\n", Distance_3);


		 	       //Traffic light system

		 	        HAL_Delay(TimeT1);
		 	        TrfcSwch(0);
		 	        HAL_Delay(TimeT2);
		 	        TrfcSwch(1);

			 	  	uint8_t counter = 10;

			 	  	uint8_t Cnt0On = 0;
					uint8_t Cnt0Off = 0;

			 	  	uint8_t Cnt1On = 0;
					uint8_t Cnt1Off = 0;

			 	  	uint8_t Cnt2On = 0;
					uint8_t Cnt2Off = 0;

			 	  	uint8_t Cnt3On = 0;
					uint8_t Cnt3Off = 0;


			 	  	while( counter != 0)
			 	  	{
			 	  		Distance_0 = DistSensor(sensor_0);
			 	  		if(Distance_0 < 16 && Distance_0 > 7.5)
			 	  		{
			 	  			Cnt0On++;
			 	  		}
			 	  		else if (Distance_0 > 16)
			 	  		{
			 	  			Cnt0Off++;
			 	  		}
			 	  		Distance_1 = DistSensor(sensor_1);
			 	  		if(Distance_1 < 16 && Distance_1 > 7.5)
			 	  		{
			 	  			Cnt1On++;
			 	  		}
			 	  		else if (Distance_1 > 16)
			 	  		{
			 	  		Cnt1Off++;
			 	  		}

			 	  		Distance_2 = DistSensor(sensor_2);
			 	  		if(Distance_2 < 16 && Distance_2 > 7.5)
			 	  		{
			 	  			Cnt2On++;
			 	  		}
			 	  		else if (Distance_2 > 16)
			 	  		{
			 	  			Cnt2Off++;
			 	  		}
			 	  		Distance_3 = DistSensor(sensor_3);
			 	  		if(Distance_3 < 16 && Distance_3 > 7.5)
			 	  		{
			 	  			Cnt3On++;
			 	  		}
			 	  		else if (Distance_3 > 16)
			 	  		{
			 	  			Cnt3Off++;
			 	  		}

			 	  		HAL_Delay(1000);
			 	  		counter--;
			 	  	}

			 	  	int CntTotal_1stLane = 0;
			 	  	int CntTotal_2ndLane = 0;
			 	  	int CntTotalDiff = 0;

			 	  	CntTotal_1stLane = Cnt0On + Cnt2On;
			 	  	CntTotal_2ndLane = Cnt1On + Cnt3On;

			 	  	if(CntTotal_1stLane > CntTotal_2ndLane)
			 	  	{
			 	  		CntTotalDiff = CntTotal_1stLane - CntTotal_2ndLane;
			 	  		if(CntTotalDiff > 12)
			 	  		{
				 	  		TimeT1 = 50000 - initial_delay - yellow_delay - distance_delay;
				 	  		TimeT2 = 10000 - yellow_delay;
			 	  		}
			 	  		else if(CntTotalDiff > 9)
			 	  		{
				 	  		TimeT1 = 45000 - initial_delay - yellow_delay - distance_delay;
				 	  		TimeT2 = 15000 - yellow_delay;
			 	  		}
			 	  		else if(CntTotalDiff > 6)
			 	  		{
				 	  		TimeT1 = 40000 - initial_delay - yellow_delay - distance_delay;
				 	  		TimeT2 = 20000 - yellow_delay;
			 	  		}
			 	  		else if(CntTotalDiff > 3)
			 	  		{
				 	  		TimeT1 = 30000 - initial_delay - yellow_delay  - distance_delay;
				 	  		TimeT2 = 25000 - yellow_delay;
			 	  		}
			 	  	}
			 	  	else if(CntTotal_2ndLane > CntTotal_1stLane)
			 	  	{
			 	  		CntTotalDiff = CntTotal_2ndLane - CntTotal_1stLane;
			 	  		if(CntTotalDiff > 12)
			 	  		{
				 	  		TimeT1 = 50000 - initial_delay - yellow_delay  - distance_delay;
				 	  		TimeT2 = 10000 - yellow_delay;
			 	  		}
			 	  		else if(CntTotalDiff > 9)
			 	  		{
				 	  		TimeT1 = 45000 - initial_delay - yellow_delay  - distance_delay;
				 	  		TimeT2 = 15000 - yellow_delay;
			 	  		}
			 	  		else if(CntTotalDiff > 6)
			 	  		{
				 	  		TimeT1 = 40000 - initial_delay - yellow_delay  - distance_delay;
				 	  		TimeT2 = 20000 - yellow_delay;
			 	  		}
			 	  		else if(CntTotalDiff > 3)
			 	  		{
				 	  		TimeT1 = 30000 - initial_delay - yellow_delay  - distance_delay;
				 	  		TimeT2 = 25000 - yellow_delay;
			 	  		}
			 	  	}
			 	  	else if(CntTotal_1stLane == CntTotal_2ndLane)
					{
			 	  		TimeT1 = 30000 - initial_delay - yellow_delay  - distance_delay;
			 	  		TimeT2 = 30000 - yellow_delay;
			 	  	}
			 	  	RealTime1 = TimeT1 + initial_delay + yellow_delay  + distance_delay;
			 	  	RealTime2 = TimeT2 + yellow_delay;

			 	  	GCntTotalDiff = CntTotalDiff;
			 	  	GCntTotal_1stLane = CntTotal_1stLane;
			 	  	GCntTotal_2ndLane = CntTotal_2ndLane;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 36;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00301739;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 209700;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_7B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xffff-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xffff-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.battery_charging_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0 PF1 PF2 PF12
                           PF13 PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_2|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE9 PE11 PE13 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD14 PD15
                           PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
