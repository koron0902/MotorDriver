#pragma once

/** @defgroup CHIP_I2CROM_15XX CHIP: LPC15xx I2C ROM API declarations and functions
 * @ingroup ROMAPI_15XX
 * @{
 */

/**
 * @brief LPC15XX I2C ROM driver handle structure
 */
typedef void *I2C_HANDLE_T;

/**
 * @brief LPC15XX I2C ROM driver callback function
 */
typedef void  (*I2C_CALLBK_T)(uint32_t err_code, uint32_t n);

/**
 * LPC15XX I2C ROM driver parameter structure
 */
typedef struct I2C_PARAM {
	uint32_t        num_bytes_send;		/*!< No. of bytes to send */
	uint32_t        num_bytes_rec;		/*!< No. of bytes to receive */
	uint8_t         *buffer_ptr_send;	/*!< Pointer to send buffer */
	uint8_t         *buffer_ptr_rec;	/*!< Pointer to receive buffer */
	I2C_CALLBK_T    func_pt;			/*!< Callback function */
	uint8_t         stop_flag;			/*!< Stop flag */
	uint8_t         dummy[3];
} I2C_PARAM_T;

/**
 * LPC15XX I2C ROM driver result structure
 */
typedef struct I2C_RESULT {
	uint32_t n_bytes_sent;	/*!< No. of bytes sent */
	uint32_t n_bytes_recd;	/*!< No. of bytes received */
} I2C_RESULT_T;

/**
 * LPC15XX I2C ROM driver modes enum
 */
typedef enum CHIP_I2C_MODE {
	IDLE,			/*!< IDLE state */
	MASTER_SEND,	/*!< Master send state */
	MASTER_RECEIVE,	/*!< Master Receive state */
	SLAVE_SEND,		/*!< Slave send state */
	SLAVE_RECEIVE	/*!< Slave receive state */
} CHIP_I2C_MODE_T;

/**
 * LPC15XX I2C ROM driver APIs structure
 */
struct I2CD_API {
	/*!< Interrupt Support Routine */
	void (*i2c_isr_handler)(I2C_HANDLE_T *handle);
	/*!< MASTER functions */
	ErrorCode_t (*i2c_master_transmit_poll)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_master_receive_poll)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_master_tx_rx_poll)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_master_transmit_intr)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_master_receive_intr)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_master_tx_rx_intr)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);

	/*!< SLAVE functions */
	ErrorCode_t (*i2c_slave_receive_poll)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_slave_transmit_poll)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_slave_receive_intr)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_slave_transmit_intr)(I2C_HANDLE_T *handle, I2C_PARAM_T *param, I2C_RESULT_T *result);
	ErrorCode_t (*i2c_set_slave_addr)(I2C_HANDLE_T *handle, uint32_t slave_addr_0_3, uint32_t slave_mask_0_3);

	/*!< OTHER support functions */
	uint32_t (*i2c_get_mem_size)(void);
	I2C_HANDLE_T *(*i2c_setup)(uint32_t  i2c_base_addr, uint32_t * start_of_ram);
	ErrorCode_t (*i2c_set_bitrate)(I2C_HANDLE_T *handle, uint32_t  p_clk_in_hz, uint32_t bitrate_in_bps);
	uint32_t (*i2c_get_firmware_version)(void);
	CHIP_I2C_MODE_T (*i2c_get_status)(I2C_HANDLE_T *handle);
	ErrorCode_t (*i2c_set_timeout)(I2C_HANDLE_T *handle, uint32_t timeout);
};
typedef struct I2CD_API I2CD_API_T;


/**
 * @}
 */

