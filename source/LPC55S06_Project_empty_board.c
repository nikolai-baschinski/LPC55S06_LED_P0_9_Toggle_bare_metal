// (c) Nikolai Baschinski

#include "LPC55S06.h"

void set_flash_wait_states_96MHz();
void set_power_profile_96MHz();

void init_CLOCK()
{
  set_flash_wait_states_96MHz();

  set_power_profile_96MHz();

  // Enable 96 MHz free running oscillator
  ANACTRL->FRO192M_CTRL |= ANACTRL_FRO192M_CTRL_ENA_96MHZCLK(1);

  SYSCON->AHBCLKDIV = 0;   // divide by one
  SYSCON->MAINCLKSELA = 3; // Set clock source to FRO 96 MHz
  SYSCON->MAINCLKSELB = 0; // Set clock source to main clock A
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
  CTIMER0->TCR = 1; // enable timer 0
}

uint32_t cntr = 0;

int main(void)
{
  init_CLOCK();
  init_GPIO();
  init_TIMER();
  while(1);
  return 0;
}

void CTIMER0_IRQHandler(void)
{
  CTIMER0->IR = 1;

  if(cntr%100 == 0) {
    GPIO->NOT[0] = (1UL << 22);
  }
  cntr++;
  GPIO->NOT[0] = (1UL << 9);
}

void set_power_profile_96MHz()
{
  // NXP recommended power profiles for 96 MHz

  // dcdc power profile
  #define FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_0_ADDRS (FLASH_NMPA_BASE + 0xE8U)
  #define FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_1_ADDRS (FLASH_NMPA_BASE + 0xECU)
  uint32_t dcdcTrimValue0 = (*((volatile unsigned int *)(FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_0_ADDRS)));
  uint32_t dcdcTrimValue1 = (*((volatile unsigned int *)(FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_1_ADDRS)));

  if (0UL != (dcdcTrimValue0 & 0x1UL)) {
    dcdcTrimValue0 = dcdcTrimValue0 >> 1;
    PMC->DCDC0 = dcdcTrimValue0;
    PMC->DCDC1 = dcdcTrimValue1;
  }

  // set voltage, values see user manual
  uint32_t lv_dcdc         = 5;
  uint32_t lv_ldo_ao       = 22;
  uint32_t lv_ldo_ao_boost = 27;

  // Set up LDO Always-On voltages
  PMC->LDOPMU = (PMC->LDOPMU & (~PMC_LDOPMU_VADJ_MASK) & (~PMC_LDOPMU_VADJ_BOOST_MASK)) | PMC_LDOPMU_VADJ(lv_ldo_ao) |
                PMC_LDOPMU_VADJ_BOOST(lv_ldo_ao_boost);

  // Set up DCDC voltage
  PMC->DCDC0 = (PMC->DCDC0 & (~PMC_DCDC0_VOUT_MASK)) | PMC_DCDC0_VOUT(lv_dcdc);
}

void set_flash_wait_states_96MHz(void)
{
  const uint32_t num_wait_states = 7U;

  // Save prefetch state
  uint32_t prefetch_enable_mask = SYSCON->FMCCR & SYSCON_FMCCR_PREFEN_MASK;

  // disable prefetch before flash commands
  SYSCON->FMCCR &= ~SYSCON_FMCCR_PREFEN_MASK;

  // Delete all status flags
  FLASH->INT_CLR_STATUS = 0x1FU;

  // Set new wait value
  FLASH->DATAW[0] = (FLASH->DATAW[0] & 0xFFFFFFF0UL) | (num_wait_states & 0xFU);

  // CMD_SET_READ_MODE
  FLASH->CMD = 0x2U;

  // wait
  while ((FLASH->INT_STATUS & FLASH_INT_STATUS_DONE_MASK) == 0U);

  // Set FMC
  SYSCON->FMCCR = (SYSCON->FMCCR & ~SYSCON_FMCCR_FLASHTIM_MASK) | ((num_wait_states << SYSCON_FMCCR_FLASHTIM_SHIFT) & SYSCON_FMCCR_FLASHTIM_MASK);

  // Restore prefetch state
  SYSCON->FMCCR |= prefetch_enable_mask;
}
