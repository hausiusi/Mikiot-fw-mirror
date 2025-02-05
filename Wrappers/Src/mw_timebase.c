/*
 * mw_timebase.c
 *
 *  Created on: Apr 24, 2020
 *      Author: Zviad
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#include "mw_timebase.h"
#include "defines.h"

#define BASETIME_TIMER TIM9

static uint32_t basetime_ticks;
static lockable_uint64_t basetime_ticks_us;
static TIM_HandleTypeDef base_tim;

void mw_timebase_init(int frequency_hz) {
    basetime_ticks = 0;
    base_tim.Instance = BASETIME_TIMER;
    uint32_t tim_clock = SystemCoreClock / 4;
    base_tim.Init.Prescaler = 3;
    base_tim.Init.Period = (uint32_t) (tim_clock / (frequency_hz)) - 1;
    base_tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    // Period = (PSK + 1) * (ARR+1) / TimerClockFreq
    __HAL_RCC_TIM9_CLK_ENABLE();
    HAL_TIM_Base_Init(&base_tim);
    HAL_TIM_Base_Start_IT(&base_tim);
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 4, 1);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
}

uint32_t mw_timebase_ticks_get() {
    return basetime_ticks;
}

uint64_t mw_timebase_ticks_get_us() {
    uint32_t cnt = base_tim.Instance->CNT;
    while (basetime_ticks_us.locked) {

    }

    return basetime_ticks_us.value + (cnt / 25);
}

void TIM1_BRK_TIM9_IRQHandler() {
    basetime_ticks++;
    basetime_ticks_us.locked = true;
    basetime_ticks_us.value += 1000;
    basetime_ticks_us.locked = false;
    HAL_TIM_IRQHandler(&base_tim);
}

uint32_t HAL_GetTick(void) {
    return mw_timebase_ticks_get();
}
