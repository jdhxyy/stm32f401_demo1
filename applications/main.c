// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// bootloader入口文件

#include "framework.h"

#define VERSION_SOFT 4

// 单片机内部RAM
#define RAM_INTERNAL 0

// 给TZMALLOC使用的空间
// 使用的内部存储大小.单位:kbytes
#define RAM_INTERNAL_SIZE 20

// tzmalloc用户模块最大数
#define TZMALLOC_USER_NUM_MAX 20

// 串口1参数
#define UART1_MAX_FRAME_SIZE 1024
#define UART1_BAUD_RATE 115200
#define UART1_TX_PIN STM32F4_PA9
#define UART1_RX_PIN STM32F4_PA10

// 串口6参数
#define UART6_MAX_FRAME_SIZE 1024
#define UART6_BAUD_RATE 115200
#define UART6_TX_PIN STM32F4_PA11
#define UART6_RX_PIN STM32F4_PA12

static int mid = -1;
static uint64_t pipe = 0;

// DComCallbackFunc 注册DCOM服务回调函数
// 遵循谁调用谁释放原则,resp需要由回调函数使用TZMalloc开辟空间,DCom负责释放空间
// 返回值为0表示回调成功,否则是错误码
static int service1(uint64_t pipe, uint64_t srcIA, uint8_t* req, int reqLen, uint8_t** resp, int* respLen);

static void initDrivers(void);
static void dealUart1Rx(uint8_t* bytes, int size);
static void dealUart6Rx(uint8_t* bytes, int size);
static LaganTime getLaganTime(void);
static int case1(void);
static void dealEspATRxCallback(uint8_t* data, int size, uint8_t* ip, uint16_t port);

static void corePipeSend(uint8_t* data, int size, uint8_t* dstIP, uint16_t dstPort);
static int case2(void);

int main(void)
{
    DrvClockInit();
    ClockLoad();
    TZTimeLoad(GetLocalTimeUs);

//    LaganLoad(TZStm32f4Uart1Tx, getLaganTime, NULL);
    LaganLoad(TZStm32f4Uart1Tx, NULL, TZTimeGet);
    
    __nop();
    void* addr = malloc((size_t)RAM_INTERNAL_SIZE * 1024);
    if (addr != NULL) {
        TZMallocLoad(RAM_INTERNAL, TZMALLOC_USER_NUM_MAX, RAM_INTERNAL_SIZE * 1024, addr);
    }
    
    mid = TZMallocRegister(RAM_INTERNAL, "test", 1024);
    
    initDrivers();
    TZStm32f4Uart1RegisterCallback(dealUart1Rx);
    TZStm32f4Uart6RegisterCallback(dealUart6Rx);
    TZStm32f4Uart1Tx("jdh", 3);
    
    LE("abc", "123%d", 456);


    EspATLoadParam param;
    param.BaudRate = 115200;
    strcpy(param.WifiSsid, "JDHOME_MASTER");
    strcpy(param.WifiPwd, "QWERASDFZXCV");
    param.WriteResetIO = NULL;
    param.SetBaudRate = TZStm32f4Uart1EditBaudRate;
    param.Send = TZStm32f4Uart6Tx;
    param.IsAllowSend = TZStm32f4Uart6IsBusy;
    EspATLoad(&param);
    
    EspATRegisterObserver(dealEspATRxCallback);
    
    TZIotLoad(0x2141000000010010, "123");
    pipe = TZIotBindPipeCore(corePipeSend, EspATIsConnectWifi);
    
    TZIotRegister(1, service1);
    
    //AsyncStart(case1, 1 * ASYNC_SECOND);
    AsyncStart(case2, 10 * ASYNC_SECOND);
    
    
// 	drv_flash_init();
//    TZBaseAdapterLoad();
//	TZBootRun();
	for (;;) {
        AsyncRun();
    }
}

static void initDrivers(void) {
    TZStm32f4UartParam param;

    param.MaxFrameSize = UART1_MAX_FRAME_SIZE;
    param.BaudRate = UART1_BAUD_RATE;
    param.Priority = TZ_IRQ_PRIORITY_MIDDLE;
    param.TxPin = UART1_TX_PIN;
    param.RxPin = UART1_RX_PIN;
    param.TxCtrlPin = STM32F4_INVALID_PIN;
    param.RxCtrlPin = STM32F4_INVALID_PIN;
    TZStm32f4Uart1Init(param);

    param.MaxFrameSize = UART6_MAX_FRAME_SIZE;
    param.BaudRate = UART6_BAUD_RATE;
    param.Priority = TZ_IRQ_PRIORITY_MIDDLE;
    param.TxPin = UART6_TX_PIN;
    param.RxPin = UART6_RX_PIN;
    param.TxCtrlPin = STM32F4_INVALID_PIN;
    param.RxCtrlPin = STM32F4_INVALID_PIN;
    TZStm32f4Uart6Init(param);
}

static void dealUart1Rx(uint8_t* bytes, int size) {
    __nop();
}

static LaganTime getLaganTime(void) {
    static LaganTime laganTime;
    uint64_t time = TZTimeGet();
    laganTime.Hour = time / 3600000000;
    time %= 3600000000;
    laganTime.Minute = time / 60000000;
    time %= 60000000;
    laganTime.Second = time / 1000000;
    time %= 1000000;
    laganTime.Us = time;
    return laganTime;
}

static void dealUart6Rx(uint8_t* bytes, int size) {
    EspATReceive(bytes, size);
}

// DComCallbackFunc 注册DCOM服务回调函数
// 遵循谁调用谁释放原则,resp需要由回调函数使用TZMalloc开辟空间,DCom负责释放空间
// 返回值为0表示回调成功,否则是错误码
static int service1(uint64_t pipe, uint64_t srcIA, uint8_t* req, int reqLen, uint8_t** resp, int* respLen) {
    LI("service1", "pipe:%d srcIA:0x%x reqLen:%d\n", pipe, srcIA, reqLen);

    LaganPrintHex("service1", LAGAN_LEVEL_INFO, req, reqLen);

    uint8_t* arr = TZMalloc(mid, 260);
    if (arr == NULL) {
        return 0;
    }
    for (int i = 0; i < 260; i++) {
        arr[i] = i;
    }
    *resp = arr;
    *respLen = 260;
    return 0;
}

static int case1(void) {
    static struct pt pt = {0};
    static int i = 0;
    char str[32] = {0};
    
    PT_BEGIN(&pt);
    
//    if (i > 2) {
//        PT_EXIT(&pt);
//    }
    
    PT_WAIT_UNTIL(&pt, EspATIsConnectWifi());
    sprintf(str, "jdh%d", i++);
    EspATSend((uint8_t*)str, strlen(str), "192.168.1.119", 12100);
    
    PT_END(&pt);
}

static void dealEspATRxCallback(uint8_t* data, int size, uint8_t* ip, uint16_t port) {
    LI("esprx:", "--------------------->ip:%d.%d.%d.%d port:%d", ip[0], ip[1], ip[2], ip[3], port);
    LaganPrintHex("esprxhex:", LAGAN_LEVEL_INFO, data, size);
    TZIotPipeCoreReceive(data, size, ip, port);
}

static void corePipeSend(uint8_t* data, int size, uint8_t* dstIP, uint16_t dstPort) {
    //printf("------------------------->pipeSend:0x%x %p %d\n", (uint32_t)dstIA, (void*)bytes, size);
//    for (int i = 0; i < size; i++) {
//        printf("%02x ", bytes[i]);
//    }
//    printf("\n");

//    DComReceive(protocol, pipe, dstIA, bytes, size);
    EspATSend(data, size, dstIP, dstPort);
}

static int case2(void) {
    static struct pt pt;
    static uint8_t arr[3] = {1,2,3};
    static int result = 0;
    static uint8_t* resp = NULL;
    static int respLen = 0;
    static intptr_t handle;

    PT_BEGIN(&pt);

    PT_WAIT_UNTIL(&pt, TZIotIsConn());

    handle = TZIotCallCreateHandle(pipe, 0x2141000000000004, 1, 5000, NULL, 0, &resp, &respLen, &result);
    PT_WAIT_THREAD(&pt, TZIotCall(handle));

    if (result == 0 && resp != NULL) {
        LI("case2", "rx:%d %s", respLen, (char*)resp);
        //LaganPrintHex("case2", LAGAN_LEVEL_INFO, resp, respLen);
//        for (int i = 0; i < respLen; i++) {
//            printf("0x%02x\t", resp[i]);
//        }
//        printf("\n");
    } else {
        //printf("call failed:0x%x\n", result);
        LW("case2", "call failed:0x%x\n", result);
    }
    
    if (resp != NULL) {
        TZFree(resp);
        resp = NULL;
    }

    PT_END(&pt);
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
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
