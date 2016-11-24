#pragma once
/** @defgroup PWRROM_15XX CHIP: LPC15xx Power ROM API declarations and functions
 * @ingroup ROMAPI_15XX
 * @{
 */

/**
 * @brief LPC15XX Power ROM APIs - set_pll mode options
 */
#define CPU_FREQ_EQU    0
#define CPU_FREQ_LTE    1
#define CPU_FREQ_GTE    2
#define CPU_FREQ_APPROX 3

/**
 * @brief LPC15XX Power ROM APIs - set_pll response0 options
 */
#define PLL_CMD_SUCCESS    0
#define PLL_INVALID_FREQ   1
#define PLL_INVALID_MODE   2
#define PLL_FREQ_NOT_FOUND 3
#define PLL_NOT_LOCKED     4

/**
 * @brief LPC15XX Power ROM APIs - set_power mode options
 */
#define PWR_DEFAULT         0
#define PWR_CPU_PERFORMANCE 1
#define PWR_EFFICIENCY      2
#define PWR_LOW_CURRENT     3

/**
 * @brief LPC15XX Power ROM APIs - set_power response0 options
 */
#define PWR_CMD_SUCCESS  0
#define PWR_INVALID_FREQ 1
#define PWR_INVALID_MODE 2

/**
 * @brief LPC15XX Power ROM APIs - power_mode_configure mode options
 */
#define PMU_SLEEP           0
#define PMU_DEEP_SLEEP      1
#define PMU_POWERDOWN       2
#define PMU_DEEP_POWERDOWN  3

/**
 * @brief LPC15XX Power ROM APIs - power_mode_configure peripheral control bits
 */
#define PMU_PD_WDOSC         (1 << 0)
#define PMU_PD_BOD           (1 << 1)
#define PMU_PD_ACMP0         (1 << 2)
#define PMU_PD_ACMP1         (1 << 3)
#define PMU_PD_ACMP2         (1 << 4)
#define PMU_PD_ACMP3         (1 << 5)
#define PMU_PD_IREF          (1 << 6)
#define PMU_PD_TS            (1 << 7)

/**
 * @brief LPC15xx Power ROM API structure
 * The power profile API provides functions to configure the system clock and optimize the
 * system setting for lowest power consumption.
 */
typedef struct PWRD_API {
	void (*set_pll)(uint32_t cmd[], uint32_t resp[]);	/*!< Set PLL function */
	void (*set_power)(uint32_t cmd[], uint32_t resp[]);	/*!< Set power function */
	void (*power_mode_configure)(uint32_t power_mode, uint32_t peripheral_ctrl);/*!< Sets the chip is low power modes */
	void (*set_aclkgate)(uint32_t aclkgate);
	uint32_t (*get_aclkgate)(void);
} PWRD_API_T;

/**
 * @}
 */

