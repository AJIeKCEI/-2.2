
#include "stm32f7xx.h"

void init_RCC(void)
{
    RCC->AHB1RSTR = 0xFFFFFFFF;
    RCC->AHB1RSTR = 0x00000000;

    RCC->AHB2RSTR = 0xFFFFFFFF;
    RCC->AHB2RSTR = 0x00000000;

    RCC->APB1RSTR = 0xFFFFFFFF;
    RCC->APB1RSTR = 0x00000000;

    RCC->APB2RSTR = 0xFFFFFFFF;
    RCC->APB2RSTR = 0x00000000;

    // Обновление переменной с частотой тактирования

    SystemCoreClockUpdate();
    
   // return;

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    // включение внешенего тактирования
    //RCC->CR |= RCC_CR_HSIBYP;
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    //Выключение PLL  пред делитель
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY);

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1;

    //Настройка предделителя PLL
    uint32_t pllcfgr = 0;
    //пред делитель ноль это два
    pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE;
    pllcfgr |= 8 << RCC_PLLCFGR_PLLM_Pos;
    pllcfgr |= 64 << RCC_PLLCFGR_PLLN_Pos;
    pllcfgr |= 1 << RCC_PLLCFGR_PLLP_Pos;

    RCC->PLLCFGR = pllcfgr;

    //Настройка latency  на 1  для 32 МГц
    FLASH->ACR |= (1 << FLASH_ACR_LATENCY_Pos);

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    //Выбор PLL  как основного источника тактирования
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    //включение овердрайв нужен для максимальной частоте
   // PWR->CR1 |= (uint32_t)PWR_CR1_ODEN;
   // PWR->CR1 |= (uint32_t)PWR_CR1_ODSWEN;

    SystemCoreClockUpdate();
}
