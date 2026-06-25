// (c) Nikolai Baschinski 22.06.2026

#include "LPC55S06.h"

void init_GPIO();
void init_TIMER();

uint32_t cntr = 0;

int main(void)
{
  init_GPIO();
  init_TIMER();
  while(1);
  return 0;
}

void CTIMER0_IRQHandler(void)
{
  CTIMER0->IR = CTIMER_IR_MR0INT_MASK;

  if(cntr%100 == 0) {
    GPIO->NOT[0] = (1UL << 22);
  }
  cntr++;
  GPIO->NOT[0] = (1UL << 9);
}

void init_GPIO()
{
  SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_GPIO0_MASK | SYSCON_AHBCLKCTRL0_IOCON_MASK;

  // GPIO Port 0, Pin 22 (LED D4)
  IOCON->PIO[0][22] = 0;
  GPIO->DIRSET[0] = (1UL << 22);

  // GPIO Port 0, Pin 9
  IOCON->PIO[0][9] = 0;
  GPIO->DIRSET[0] = (1UL << 9);
  IOCON->PIO[0][9] = IOCON_PIO_DIGIMODE(1);

  // UM: Once the pins are configured, the IOCON clock can be disabled in order to conserve power.
  SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL0_IOCON_MASK;
}

void init_TIMER()
{
  SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_TIMER0_MASK; // enable clock for timer 0
  SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK; // reset peripheral
  SYSCON->CTIMERCLKSELX[0] = 3; // select FRO 96 MHz as source for timer 0
  CTIMER0->PR = 95; // prescaler
  CTIMER0->MR[0] = 10000; // set match value for 10 ms
  CTIMER0->MCR = (1 << 0) | (1 << 1); // interrupt is generated when MR0 matches the value in the TC
  NVIC->ISER[0] |= (1UL << (uint32_t)CTIMER0_IRQn); // Enable CTIMER0 interrupt
  CTIMER0->TC = 0; // set timer counter to 0
  CTIMER0->PC = 0; // set prescale counter to 0
  CTIMER0->TCR = 1; // enable timer 0
}
