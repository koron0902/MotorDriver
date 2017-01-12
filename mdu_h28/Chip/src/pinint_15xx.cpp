#include <pinint_15xx.hpp>
#include <chip.hpp>

void Chip_PININT_Init(LPC_PIN_INT_T *pPININT) {
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);
	Chip_SYSCTL_PeriphReset(RESET_PININT);
}

void Chip_PININT_DeInit(LPC_PIN_INT_T *pPININT) {
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_PININT);
}
