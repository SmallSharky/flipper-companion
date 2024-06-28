#include "FreeRTOS.h"
#include "task.h"

#include "main.h"
#include "cmsis_os.h"
#include <cstdio>

#include "Device.hpp"
#include "ST7567_StateMachine.hpp"

osThreadId_t defaultTaskThreadId;
osThreadId_t displayTaskThreadId;
// static TaskHandle_t defaultTaskHandle = NULL;
const UBaseType_t xArrayIndex = 1;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};

const osThreadAttr_t displayTask_attributes = {
    .name = "displayTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
app::ST7567_StateMachine dispStateMachine;
size_t maxRxBufUsage{0};
void StartDefaultTask(void *argument);
void StartDisplayTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void)
{
    defaultTaskThreadId = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
    displayTaskThreadId = osThreadNew(StartDisplayTask, NULL, &displayTask_attributes);
}

void unlockRxTask() {
    // printf("Unlock\n");
    // osSignalSet(defaultTaskThreadId, 0);
    auto res = osThreadFlagsSet(defaultTaskThreadId, 1);
    if (res != 1) {
        printf("Unlock status %lu\n", res);
    }
    // printf("Unlock status %lu\n", res);

    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // vTaskNotifyGiveFromISR( defaultTaskHandle, &xHigherPriorityTaskWoken );
}

void StartDefaultTask([[maybe_unused]] void *argument)
{
    // defaultTaskHandle = xTaskGetCurrentTaskHandle();
    auto &dev = app::Device::instance();
    dev.registerRxCallback(unlockRxTask);
    printf("GUI initialized\n");
    osThreadFlagsClear(1);
    // size_t i = 0;
    // bool dataSent{false};
    for (;;)
    {
        auto res = osThreadFlagsWait(1, osFlagsWaitAny, 100);

        // osSignalWait(0, 1000);
        // 
        // uint32_t res = 1;
        // uint32_t ulNotificationValue = ulTaskNotifyTake(pdFALSE, xMaxBlockTime);
        // printf("Unlocked\n");
        
        // if (ulNotificationValue == 1)
        // {
        
        while(!dev.displayRxBuf.empty()) {

            maxRxBufUsage = std::max(dev.displayRxBuf.size(), maxRxBufUsage);
            // dataSent = true;
            // if (i>=8) {
            //     // printf("\n");
            //     i = 0;
            // }

            auto tmp = dev.displayRxBuf.pop();
            // printf("%02x ", tmp.data);
            // ++i;
            dispStateMachine.process(reinterpret_cast<uint8_t*>(&tmp)[0], reinterpret_cast<uint8_t*>(&tmp)[1]);
            // if(tmp.isCommand) {
            //     printf("\nCMD[%02x] ", tmp.data);
            // } else {
            //     printf("%02x ", tmp.data);
            // }
            
        }
        // if(res != 1 && dataSent) {
        //     dataSent = false;
        //     printf("\n\n\n\n\n");
        // }
            /* The transmission ended as expected. */
        // }
        // else
        // {
        //     /* The call to ulTaskNotifyTake() timed out. */
        // }
        // dev.led.toggle();
        // osDelay(50);
    }
}

void StartDisplayTask([[maybe_unused]] void *argument)
{
    auto& dev = app::Device::instance();
    for(;;) {
        
        
        if(dispStateMachine.updated()) {
            dispStateMachine.clearUpdated();
            dev.display.refresh(dispStateMachine.buffer);            
        }
        if(dev.button.read()) {            
            dispStateMachine.display();
        }     
   // printf("Max RX buf usage: %u\n", maxRxBufUsage);
        dev.led.toggle();
        osDelay(100);
    }
}
