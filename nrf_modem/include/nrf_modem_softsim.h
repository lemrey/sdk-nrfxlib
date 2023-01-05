/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_softsim.h
 *
 * @defgroup nrf_modem_softsim SoftSIM API
 * @{
 * @brief API to mediate SoftSIM exchanges between Modem and application.
 */
#ifndef NRF_MODEM_SOFTSIM_H__
#define NRF_MODEM_SOFTSIM_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Max allowed Le.
 * 
 * @details Maximum number of bytes expected in the data part of the response APDU.
 */
#define NRF_MODEM_SOFTSIM_MAX_LE 512

/** @brief Callback procedure that shall be called when handling a SoftSIM reset. */
void nrf_modem_softsim_reset_handler(void);

/** @brief SoftSIM request command type. */
enum nrf_modem_softsim_cmd {
	NRF_MODEM_SOFTSIM_INIT = 1,
	NRF_MODEM_SOFTSIM_APDU = 3,
	NRF_MODEM_SOFTSIM_DEINIT = 4,
	NRF_MODEM_SOFTSIM_RESET = 5
};

/** @brief Sends a SoftSIM request.
 *
 * @details This function sends a SoftSIM request which shall be handled asynchronously.
 *          The application shall then call @ref nrf_modem_softsim_res_send with the response data
 *          requested.
 * 
 * @note This function is called within an interrupt context so it shall return immediately.
 * 
 * @param cmd SoftSIM request command.
 * @param[in] data Pointer to the data of the SoftSIM request.
 * @param data_len Length of the data of the SoftSIM request.
 * 
 * @retval 0 on success.
 * @retval -NRF_EINVAL If input data is invalid.
 * @retval -NRF_ENOMEM If allocation failed.
 */
int nrf_modem_softsim_req_send(enum nrf_modem_softsim_cmd cmd, const uint8_t *data,
			       uint16_t data_len);

/**
 * @brief Sends the SoftSIM response to the last request.
 * 
 * @details This function is used to respond to the modem with the data requested.
 *
 * @param[in] data Pointer to the data of the SoftSIM response.
 * @param data_len Length of the data of the SoftSIM response.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL If input data is invalid.
 * @retval -NRF_ENOMEM If memory allocation failed.
 */
int nrf_modem_softsim_res_send(const uint8_t *data, uint16_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_SOFTSIM_H__ */

/** @} */
