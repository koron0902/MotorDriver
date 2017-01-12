#pragma once
#include <swm_data.hpp>
/** @defgroup SWM_15XX CHIP: LPC15xx Switch Matrix Driver
 * @ingroup CHIP_15XX_Drivers
 * @{
 */

/**
 * @brief	Initialize the SWM module
 * @return	Nothing
 * @note	This function only enables the SWM clock.
 */
STATIC INLINE void Chip_SWM_Init(void)
{
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
}

/**
 * @brief	Deinitialise the SWM module
 * @return	Nothing
 * @note	This function only disables the SWM clock.
 */
STATIC INLINE void Chip_SWM_Deinit(void)
{
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

/**
 * @brief	Assign movable pin function to physical pin in Switch Matrix
 * @param	movable	: Movable pin function
 * @param	assign	: Physical pin to be assigned
 * @return	Nothing
 */
void Chip_SWM_MovablePinAssign(CHIP_SWM_PIN_MOVABLE_T movable, uint8_t assign);

/**
 * @brief	Assign movable pin function to port and pin in the Switch Matrix
 * @param	movable	: Movable pin function
 * @param	port	: Port number
 * @param	pin		: Pin number
 * @return	Nothing
 * @note	This function does the same thing as Chip_SWM_MovablePinAssign()
 *			except works with a port and pin number instead of a physical
 *			pin number.
 */
STATIC INLINE void Chip_SWM_MovablePortPinAssign(CHIP_SWM_PIN_MOVABLE_T movable, uint8_t port, uint8_t pin)
{
	Chip_SWM_MovablePinAssign(movable, ((port * 32) + pin));
}

/**
 * @brief	Enables a fixed function pin in the Switch Matrix
 * @param	pin	: Pin to be enabled
 * @return	Nothing
 */
void Chip_SWM_EnableFixedPin(CHIP_SWM_PIN_FIXED_T pin);

/**
 * @brief	Disables a fixed function pin in the Switch Matrix
 * @param	pin	: Pin to be disabled
 * @return	Nothing
 */
void Chip_SWM_DisableFixedPin(CHIP_SWM_PIN_FIXED_T pin);

/**
 * @brief	Enables or disables a fixed function pin in the Switch Matrix
 * @param	pin		: Pin to be enabled or disabled
 * @param	enable	: True to enable the pin, False to disable the pin
 * @return	Nothing
 */
void Chip_SWM_FixedPinEnable(CHIP_SWM_PIN_FIXED_T pin, bool enable);

/**
 * @brief	Tests whether a fixed function pin is enabled or disabled in the Switch Matrix
 * @param	pin	: The pin to test whether it is enabled or disabled
 * @return	True if the pin is enabled, False if disabled
 */
bool Chip_SWM_IsFixedPinEnabled(CHIP_SWM_PIN_FIXED_T pin);

/**
 * @}
 */


