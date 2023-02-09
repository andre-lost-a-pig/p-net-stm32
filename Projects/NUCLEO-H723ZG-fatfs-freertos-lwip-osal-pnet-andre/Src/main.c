/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComPolling/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          polling transfer. 
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "httpserver_socket.h"
#include "app_ethernet.h"
#include "sys/osal_cc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD disk logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD disk logical drive path */

UART_HandleTypeDef UartHandle;

struct netif gnetif; /* network interface structure */
osThreadAttr_t attr;
osThreadId StartHandle;
osThreadId LinkHandle;
osThreadId DHCPHandle;
osThreadId defaultTaskHandle;
  
int SDFatFSMounted = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void MX_USART1_UART_Init(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void StartThread(void const *argument);
static void StartThread_lwip(void const * argument);
static void Netif_Config(void);
static void PNetThread(void const *argument);
extern int _main (void);

/* USER CODE BEGIN PFP */
#ifdef Debug
#ifdef __GCC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GCC__ */
#endif /* Debug */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  //MX_USART1_UART_Init();
  
  /* Configure LED2 and LED3 */
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  printf("Start tasks\r\n");
  /* Init thread */
  osKernelInitialize();
  
  /* Lwip test */
  // attr.name = "Start";
  // attr.stack_size = 4 * configMINIMAL_STACK_SIZE;
  // attr.priority = osPriorityNormal;
  // StartHandle = osThreadNew(StartThread_lwip, NULL, &attr);

  /* Definitions for p-net */
  const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 2048 * 4,
    .priority = (osPriority_t) osPriorityNormal,
  };
  defaultTaskHandle = osThreadNew(PNetThread, NULL, &defaultTask_attributes);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  pvParameters not used
  * @retval None
  */
static void StartThread(void const *argument)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */

  // FATFS test
	printf("FATFS test\r\n");

  /*##-1- Link the SD disk I/O driver ########################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    printf("1-FATFS_LinkDriver ok\r\n");
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      BSP_LED_On(LED_RED);
      Error_Handler();
    }
    else
    {
      printf("2-f_mount ok\r\n");
      /*##-3- Create a FAT file system (format) on the logical drive #########*/
//      if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
//      {
//        BSP_LED_On(LED_RED);
//        Error_Handler();
//      }
//      else
      {
        printf("3-f_mkfs ok\r\n");
        /*##-4- Create and Open a new text file object with write access #####*/
        if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        {
          /* 'STM32.TXT' file Open for write Error */
          BSP_LED_On(LED_RED);
          Error_Handler();
        }
        else
        {
          printf("4-f_open ok\r\n");
          /*##-5- Write data to the text file ################################*/
          res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

          if((byteswritten == 0) || (res != FR_OK))
          {
            /* 'STM32.TXT' file Write or EOF Error */
            BSP_LED_On(LED_RED);
            Error_Handler();
          }
          else
          {
            printf("5-f_write ok\r\n");
            /*##-6- Close the open text file #################################*/
            f_close(&MyFile);
            printf("6-f_close ok\r\n");
            /*##-7- Open the text file object with read access ###############*/
            if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
            {
              /* 'STM32.TXT' file Open for read Error */
              BSP_LED_On(LED_RED);
              Error_Handler();
            }
            else
            {
              printf("7-f_open ok\r\n");
              /*##-8- Read data from the text file ###########################*/
              res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);

              if((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
              {
                /* 'STM32.TXT' file Read or EOF Error */
                BSP_LED_On(LED_RED);
                Error_Handler();
              }
              else
              {
                printf("8-f_read ok\r\n");
                /*##-9- Close the open text file #############################*/
                f_close(&MyFile);
                printf("9-f_close ok\r\n");
                /*##-10- Compare read data with the expected data ############*/
                if ((bytesread != byteswritten))
                {
                  /* Read data is different from the expected data */
                  BSP_LED_On(LED_RED);
                  Error_Handler();
                }
                else
                {
                  printf("10-Compare ok\r\n");
                  /* Success of the demo: no error occurrence */
                  BSP_LED_On(LED_GREEN);
                }
              }
            }
          }
        }
      }
    }
  }

  /*##-11- Unlink the SD disk I/O driver ####################################*/
  FATFS_UnLinkDriver(SDPath);

  /* Infinite loop */
  for( ;; )
  {
    /* Delete the Init Thread */ 
    osThreadTerminate(NULL);
  }
}

/**
  * @brief  Start Thread 
  * @param  argument not used
  * @retval None
  */
static void StartThread_lwip(void const * argument)
{ 
  /* Initialize LCD and LEDs */
//  BSP_Config();
  
  /* Create tcp_ip stack thread */
  tcpip_init(NULL, NULL);
  
  /* Initialize the LwIP stack */
  Netif_Config();
  
  /* Initialize webserver demo */
  http_server_socket_init();
  
  /* Notify user about the network interface config */
  //User_notification(&gnetif);
  
#ifdef USE_DHCP
  /* Start DHCPClient */
  osThreadDef(DHCP, DHCP_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(DHCP), &gnetif);
#endif
  
  for( ;; )
  {
    /* Delete the Init Thread */ 
    //osThreadTerminate(NULL);
  }
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
 
#if LWIP_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  
  /* add the network interface */    
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&gnetif);

  ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);

  attr.name = "EthLink";
  attr.stack_size = 4 * configMINIMAL_STACK_SIZE;
  attr.priority = osPriorityNormal;
  LinkHandle = osThreadNew(ethernet_link_thread, &gnetif, &attr);
#endif

#if LWIP_DHCP
  /* Start DHCPClient */
  attr.name = "DHCP";
  attr.stack_size = 4 * configMINIMAL_STACK_SIZE;
  attr.priority = osPriorityBelowNormal;
  DHCPHandle = osThreadNew(DHCP_Thread, &gnetif, &attr);
#endif
}

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* Initialize the LwIP stack */
  Netif_Config();

//  /* Set the link callback function, this function is called on change of link status*/
//  netif_set_link_callback(&gnetif, ethernetif_update_config);

//  /* create a binary semaphore used for informing ethernetif of frame reception */
//  osSemaphoreDef(Netif_SEM);
//  Netif_LinkSemaphore = osSemaphoreCreate(osSemaphore(Netif_SEM) , 1 );

//  link_arg.netif = &gnetif;
//  link_arg.semaphore = Netif_LinkSemaphore;
//  /* Create the Ethernet link handler thread */
///* USER CODE BEGIN OS_THREAD_DEF_CREATE_CMSIS_RTOS_V1 */
//  osThreadDef(LinkThr, ethernetif_set_link, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
//  osThreadCreate (osThread(LinkThr), &link_arg);
///* USER CODE END OS_THREAD_DEF_CREATE_CMSIS_RTOS_V1 */

//  /* Start DHCP negotiation for a network interface (IPv4) */
//  netif_set_up(&gnetif);

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */
}

static void PNetThread(void const *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  /* USER CODE BEGIN 5 */

//  huart1_sem_handle = xSemaphoreCreateCountingStatic (1, 1, &huart1_sem);
//  HAL_UART_RegisterCallback(&huart1, HAL_UART_TX_COMPLETE_CB_ID, huart1_dma_complete);

  /*##-1- Link the SD disk I/O driver ########################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    printf("1-FATFS_LinkDriver ok\r\n");
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      BSP_LED_On(LED_RED);
      Error_Handler();
    }
    else
    {
      printf("2-f_mount ok\r\n");
      SDFatFSMounted = 1;
      // if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
      // {
      //   BSP_LED_On(LED_RED);
      //   Error_Handler();
      // }
      // else
      // {
      //   printf("3-f_mkfs ok\r\n");
      // }
    }
  }

  _main();

  /* Hang here if _main returns */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance            = USARTx;

  UartHandle.Init.BaudRate       = 9600;
  UartHandle.Init.WordLength     = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits       = UART_STOPBITS_1;
  UartHandle.Init.Parity         = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode           = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling   = UART_OVERSAMPLING_16;
  UartHandle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

static char uart_buffer[2][256];
static size_t uart_buffer_ix = 0;
static size_t uart_buffer_pos = 0;

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
#ifdef Debug
PUTCHAR_PROTOTYPE
{
  uart_buffer[uart_buffer_ix][uart_buffer_pos++] = ch;
  if (ch == '\n')
  {
    uart_buffer[uart_buffer_ix][uart_buffer_pos++] = '\r';
  }

  if (uart_buffer_pos >= sizeof(uart_buffer[0]) - 1 || ch == '\n')
  {
    //xSemaphoreTake (huart1_sem_handle, UINT32_MAX);
    HAL_UART_Transmit(&UartHandle, (uint8_t *)uart_buffer[uart_buffer_ix], uart_buffer_pos, 5000);
    uart_buffer_ix = (uart_buffer_ix + 1) & 0x01;
    uart_buffer_pos = 0;
  }

  return ch;
}
#endif

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 260
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable D2 domain SRAM1 Clock (0x30000000 AXI)*/
  __HAL_RCC_D2SRAM1_CLK_ENABLE();

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 260;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

/*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
*/


  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1);
}


/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as Device not cacheable
     for ETH DMA descriptors */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_1KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as Normal Non Cacheable
     for LwIP RAM heap which contains the Tx buffers */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x30004000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @}
  */

/**
  * @}
  */

