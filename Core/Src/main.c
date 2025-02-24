/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>  // Adicionado para snprintf()

/* Private define ------------------------------------------------------------*/
#define ADC_MAX_VALUE 4095  // Máximo valor do ADC (12 bits)

/* Private variables ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;

uint32_t adc_value = 0;   // Armazena a leitura do ADC
uint8_t display_mode = 0; // Alterna os modos do OLED
char display_buffer[20];  // Buffer para exibir texto no OLED

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_TIM1_Init(void);
void MX_I2C1_Init(void);
void Update_Display(void); // Função para atualizar o OLED
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin); // Callback da interrupção do botão

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();

  HAL_ADC_Start_DMA(&hadc1, &adc_value, 1); // Inicia o ADC no modo DMA
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Inicia o PWM no canal 1 do TIM1

  ssd1306_Init(); // Inicializa o display OLED
  Update_Display(); // Atualiza a tela do display

  while (1)
  {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, adc_value * 1000 / ADC_MAX_VALUE);
    Update_Display();
    HAL_Delay(500);
  }
}

/**
  * @brief Atualiza o display OLED com informações do ADC e PWM.
  */
void Update_Display(void)
{
    ssd1306_Fill(Black); // Limpa a tela

    if (display_mode == 0)
    {
        snprintf(display_buffer, sizeof(display_buffer), "ADC: %lu", adc_value);
        ssd1306_SetCursor(10, 10);
        ssd1306_WriteString(display_buffer, Font_11x18, White);
    }
    else
    {
        snprintf(display_buffer, sizeof(display_buffer), "PWM: %lu", adc_value * 1000 / ADC_MAX_VALUE);
        ssd1306_SetCursor(10, 10);
        ssd1306_WriteString(display_buffer, Font_11x18, White);
    }

    ssd1306_UpdateScreen();
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief Callback da interrupção do botão (PA1 - EXTI).
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_1)
    {
        display_mode = !display_mode; // Alterna entre ADC e PWM
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  NVIC_SystemReset(); // Reseta o microcontrolador em caso de erro grave
}
