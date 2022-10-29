/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "libjpeg.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "TFT_ILI.h"


#include "GFX_Color.h"
#include "fonts/fonts.h"
//#include "logo.h"
//#include "tecza.h"
#include "GFX_EnhancedFonts.h"
#include "EnhancedFonts/arialBlack_20ptFontInfo.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

///////////////////

extern ApplicationTypeDef Appli_state; 	//ZMIENNA APLIKACJI W JAKIM JEST STANIE O ODPOWIEDNIM TYPIE////////////////////////////////////////////////////////////

extern uint8_t retSD;    /* Return value for SD */
extern char SDPath[4];   /* SD logical drive path */
extern FATFS SDFatFS;    /* File system object for SD logical drive */
extern FIL SDFile;       /* File object for SD */

extern uint8_t retUSBH;    /* Return value for USBH */
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

uint8_t Bytes;
char Data[128];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USB_HOST_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_FSMC_Init();
  MX_LIBJPEG_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_RTC_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  ILI_Init();

  ILI_ClearDisplay(ILI_YELLOW);

//  uint8_t Timer = HAL_GetTick();

//  uint8_t OneTime = 0;
  uint32_t Timer = HAL_GetTick();
  uint32_t OneTime=1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {



	  switch(Appli_state)
	  	  {
	  		case APPLICATION_IDLE:


	  			break;

	  		case APPLICATION_START:
	  			  retUSBH = f_mount(&USBHFatFS, (const TCHAR*)USBHPath, 0);
	  			  if(retUSBH != FR_OK)
	  			  {
	  			  	  Bytes = sprintf(Data, "FatFS mount error.\n\r");
	  			  	  HAL_UART_Transmit(&huart3, (uint8_t*)Data, Bytes, 1000);
	  			  }
	  			  else
	  			  {
	  				  Bytes = sprintf(Data, "FatFS mounted.\n\r");
	  			  	  HAL_UART_Transmit(&huart3, (uint8_t*)Data, Bytes, 1000);
	  			  }

	  			break;

	  		case APPLICATION_READY:

	  			if((retUSBH == FR_OK) && ((HAL_GetTick() - Timer) > 4000))


	  			{
	  				  	Timer = HAL_GetTick();

	  				  	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);

	  				  	DIR dj;
	  				  	FILINFO fno;

	  				  	retUSBH = f_chdrive(USBHPath);

	  				  	retUSBH = f_findfirst(&dj, &fno, "", "*.jpg");  /* Start to search for photo files */

	  				  	while (retUSBH == FR_OK && fno.fname[0] && OneTime)
	  				  	{
	  				  		ILI_DrawJPEG(0, 0, fno.fname);
	  				  		retUSBH = f_findnext(&dj, &fno);
	  				  		OneTime = 0;
	  				  	}

	  				  	f_closedir(&dj);
		  			}

		  			break;

	  		case APPLICATION_DISCONNECT:
	  			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);
	  			break;

	  		default:
	  			break;
	  	}







    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* SDIO_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SDIO_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
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
