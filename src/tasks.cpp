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
    auto res = osThreadFlagsSet(defaultTaskThreadId, 1);
    if (res != 1) {
        printf("Unlock status %lu\n", res);
    }
}

void StartDefaultTask([[maybe_unused]] void *argument)
{
    auto &dev = app::Device::instance();
    dev.registerRxCallback(unlockRxTask);
    printf("GUI initialized\n");
    osThreadFlagsClear(1);
    bool displayUpdated{false};
    for (;;)
    {
        osThreadFlagsWait(1, osFlagsWaitAny, 100);
        
        while(dev.displayRxBuf.size()) {
            maxRxBufUsage = std::max(dev.displayRxBuf.size(), maxRxBufUsage);
            auto tmp = dev.displayRxBuf.pop();
            dispStateMachine.process(reinterpret_cast<uint8_t*>(&tmp)[0], reinterpret_cast<uint8_t*>(&tmp)[1]);
            displayUpdated = true;
        }
        if(displayUpdated) {
            osThreadFlagsSet(displayTaskThreadId, 1);
            displayUpdated = false;
        }
    }
}

void StartDisplayTask([[maybe_unused]] void *argument)
{
    auto& dev = app::Device::instance();
    for(;;) {
        auto res = osThreadFlagsWait(1, osFlagsWaitAny, 100);
        if (res == 1) {
            dev.display.refresh(dispStateMachine.buffer);
        }
        if(dev.button.read()) {     
            printf("Max RX buf usage: %u\n", maxRxBufUsage);       
            dispStateMachine.display();
        }     
        dev.led.toggle();
    }
}
