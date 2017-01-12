#pragma once

#include "cmsis.h"

/** @defgroup Stop_Watch CHIP: Stopwatch primitives.
 * @ingroup CHIP_Common
 * @{
 */

/**
 * @brief	Initialize stopwatch
 * @return	Nothing
 */
void StopWatch_Init(void);

/**
 * @brief	Start a stopwatch
 * @return	Current cycle count
 */
uint32_t StopWatch_Start(void);

/**
 * @brief      Returns number of ticks elapsed since stopwatch was started
 * @param      startTime	: Time returned by StopWatch_Start().
 * @return     Number of ticks elapsed since stopwatch was started
 */
STATIC INLINE uint32_t StopWatch_Elapsed(uint32_t startTime)
{
	return StopWatch_Start() - startTime;
}

/**
 * @brief	Returns number of ticks per second of the stopwatch timer
 * @return	Number of ticks per second of the stopwatch timer
 */
uint32_t StopWatch_TicksPerSecond(void);

/**
 * @brief	Converts from stopwatch ticks to mS.
 * @param	ticks	: Duration in ticks to convert to mS.
 * @return	Number of mS in given number of ticks
 */
uint32_t StopWatch_TicksToMs(uint32_t ticks);

/**
 * @brief	Converts from stopwatch ticks to uS.
 * @param	ticks	: Duration in ticks to convert to uS.
 * @return	Number of uS in given number of ticks
 */
uint32_t StopWatch_TicksToUs(uint32_t ticks);

/**
 * @brief	Converts from mS to stopwatch ticks.
 * @param	mS	: Duration in mS to convert to ticks.
 * @return	Number of ticks in given number of mS
 */
uint32_t StopWatch_MsToTicks(uint32_t mS);

/**
 * @brief	Converts from uS to stopwatch ticks.
 * @param	uS	: Duration in uS to convert to ticks.
 * @return	Number of ticks in given number of uS
 */
uint32_t StopWatch_UsToTicks(uint32_t uS);

/**
 * @brief	Delays the given number of ticks using stopwatch primitives
 * @param	ticks	: Number of ticks to delay
 * @return	Nothing
 */
STATIC INLINE void StopWatch_DelayTicks(uint32_t ticks)
{
	uint32_t startTime = StopWatch_Start();
	while (StopWatch_Elapsed(startTime) < ticks) {}
}

/**
 * @brief	Delays the given number of mS using stopwatch primitives
 * @param	mS	: Number of mS to delay
 * @return	Nothing
 */
STATIC INLINE void StopWatch_DelayMs(uint32_t mS)
{
	uint32_t ticks = StopWatch_MsToTicks(mS);
	uint32_t startTime = StopWatch_Start();
	while (StopWatch_Elapsed(startTime) < ticks) {}
}

/**
 * @brief	Delays the given number of uS using stopwatch primitives
 * @param	uS	: Number of uS to delay
 * @return	Nothing
 */
STATIC INLINE void StopWatch_DelayUs(uint32_t uS)
{
	uint32_t ticks = StopWatch_UsToTicks(uS);
	uint32_t startTime = StopWatch_Start();
	while (StopWatch_Elapsed(startTime) < ticks) {}
}

/**
 * @}
 */

