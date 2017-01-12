#pragma once

/** @defgroup SCT_PWM_15XX CHIP: LPC15XX State Configurable Timer PWM driver
 *
 * For more information on how to use the driver please visit the FAQ page at
 * <a href="http://www.lpcware.com/content/faq/how-use-sct-standard-pwm-using-lpcopen">
 * www.lpcware.com</a>
 *
 * @ingroup CHIP_15XX_Drivers
 * @{
 */

/**
 * @brief	Get number of ticks per PWM cycle
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @return	Number ot ticks that will be counted per cycle
 * @note	Return value of this function will be vaild only
 *          after calling Chip_SCTPWM_SetRate()
 */
STATIC INLINE uint32_t Chip_SCTPWM_GetTicksPerCycle(LPC_SCT_T *pSCT)
{
	return pSCT->MATCHREL[0].U;
}

/**
 * @brief	Converts a percentage to ticks
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	percent	: Percentage to convert (0 - 100)
 * @return	Number ot ticks corresponding to given percentage
 * @note	Do not use this function when using very low
 *          pwm rate (like 100Hz or less), on a chip that has
 *          very high frequency as the calculation might
 *          cause integer overflow
 */
STATIC INLINE uint32_t Chip_SCTPWM_PercentageToTicks(LPC_SCT_T *pSCT, uint8_t percent)
{
	return (Chip_SCTPWM_GetTicksPerCycle(pSCT) * percent) / 100;
}

/**
 * @brief	Get number of ticks on per PWM cycle
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	index	: Index of the PWM 1 to N (see notes)
 * @return	Number ot ticks for which the output will be ON per cycle
 * @note	@a index will be 1 to N where N is the "Number of
 *          match registers available in the SCT - 1" or
 *          "Number of OUTPUT pins available in the SCT" whichever
 *          is minimum.
 */
STATIC INLINE uint32_t Chip_SCTPWM_GetDutyCycle(LPC_SCT_T *pSCT, uint8_t index)
{
	return pSCT->MATCHREL[index].U;
}

/**
 * @brief	Get number of ticks on per PWM cycle
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	index	: Index of the PWM 1 to N (see notes)
 * @param	ticks	: Number of ticks the output should say ON
 * @return	None
 * @note	@a index will be 1 to N where N is the "Number of
 *          match registers available in the SCT - 1" or
 *          "Number of OUTPUT pins available in the SCT" whichever
 *          is minimum. The new duty cycle will be effective only
 *          after completion of current PWM cycle.
 */
STATIC INLINE void Chip_SCTPWM_SetDutyCycle(LPC_SCT_T *pSCT, uint8_t index, uint32_t ticks)
{
	Chip_SCT_SetMatchReload(pSCT, (CHIP_SCT_MATCH_REG_T)index, ticks);
}

/**
 * @brief	Initialize the SCT/PWM clock and reset
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @return	None
 */
STATIC INLINE void Chip_SCTPWM_Init(LPC_SCT_T *pSCT)
{
	Chip_SCT_Init(pSCT);
}

/**
 * @brief	Start the SCT PWM
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @return	None
 * @note	This function must be called after all the
 * 			configuration is completed. Do not call Chip_SCTPWM_SetRate()
 * 			or Chip_SCTPWM_SetOutPin() after the SCT/PWM is started. Use
 * 			Chip_SCTPWM_Stop() to stop the SCT/PWM before reconfiguring,
 * 			Chip_SCTPWM_SetDutyCycle() can be called when the SCT/PWM is
 * 			running to change the DutyCycle.
 */
STATIC INLINE void Chip_SCTPWM_Start(LPC_SCT_T *pSCT)
{
	Chip_SCT_ClearControl(pSCT, SCT_CTRL_HALT_L | SCT_CTRL_HALT_H);
}

/**
 * @brief	Stop the SCT PWM
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @return	None
 */
STATIC INLINE void Chip_SCTPWM_Stop(LPC_SCT_T *pSCT)
{
	/* Stop SCT */
	Chip_SCT_SetControl(pSCT, SCT_CTRL_HALT_L | SCT_CTRL_HALT_H);

	/* Clear the counter */
	Chip_SCT_SetControl(pSCT, SCT_CTRL_CLRCTR_L | SCT_CTRL_CLRCTR_H);
}

/**
 * @brief	Sets the frequency of the generated PWM wave
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	freq	: Frequency in Hz
 * @return	None
 */
void Chip_SCTPWM_SetRate(LPC_SCT_T *pSCT, uint32_t freq);

/**
 * @brief	Setup the OUTPUT pin and associate it with an index
 * @param	pSCT	: The base of the SCT peripheral on the chip
 * @param	index	: Index of PWM 1 to N (see notes)
 * @param	pin		: COUT pin to be associated with the index
 * @return	None
 * @note	@a index will be 1 to N where N is the "Number of
 *          match registers available in the SCT - 1" or
 *          "Number of OUTPUT pins available in the SCT" whichever
 *          is minimum.
 */
void Chip_SCTPWM_SetOutPin(LPC_SCT_T *pSCT, uint8_t index, uint8_t pin);

/**
 * @}
 */

