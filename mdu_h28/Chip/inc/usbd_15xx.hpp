#pragma once
/** @defgroup USBD_15XX CHIP: LPC15xx USB Device driver
 * @ingroup CHIP_15XX_Drivers
 * @{
 */

/**
 * @brief USB device register block structure
 */
typedef struct {				/*!< USB Structure */
	__IO uint32_t DEVCMDSTAT;	/*!< USB Device Command/Status register */
	__IO uint32_t INFO;			/*!< USB Info register */
	__IO uint32_t EPLISTSTART;	/*!< USB EP Command/Status List start address */
	__IO uint32_t DATABUFSTART;	/*!< USB Data buffer start address */
	__IO uint32_t LPM;			/*!< Link Power Management register */
	__IO uint32_t EPSKIP;		/*!< USB Endpoint skip */
	__IO uint32_t EPINUSE;		/*!< USB Endpoint Buffer in use */
	__IO uint32_t EPBUFCFG;		/*!< USB Endpoint Buffer Configuration register */
	__IO uint32_t INTSTAT;		/*!< USB interrupt status register */
	__IO uint32_t INTEN;		/*!< USB interrupt enable register */
	__IO uint32_t INTSETSTAT;	/*!< USB set interrupt status register */
	__IO uint32_t INTROUTING;	/*!< USB interrupt routing register */
	__I  uint32_t RESERVED0[1];
	__I  uint32_t EPTOGGLE;		/*!< USB Endpoint toggle register */
} LPC_USB_T;

/**
 * @}
 */
