/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file nrf_modem_os.h
 * @brief OS specific definitions.
 *
 * @defgroup nrf_modem_os Modem library OS glue
 * @{
 */
#ifndef NRF_MODEM_OS_H__
#define NRF_MODEM_OS_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Zero time-out. */
#define NRF_MODEM_OS_NO_WAIT 0
/** Infinite time-out. */
#define NRF_MODEM_OS_FOREVER -1

/**
 * @brief Initialize the glue layer.
 */
void nrf_modem_os_init(void);

/**
 * @brief Allocate a buffer on the TX area of shared memory.
 *
 * @param bytes Buffer size.
 * @return void*
 */
void *nrf_modem_os_shm_tx_alloc(size_t bytes);

/**
 * @brief Free a shared memory buffer in the TX area.
 *
 * @param mem Buffer to free.
 */
void nrf_modem_os_shm_tx_free(void *mem);

/**
 * @brief Allocate a buffer on the library heap.
 *
 * @param bytes Buffer size.
 * @return void*
 */
void *nrf_modem_os_alloc(size_t bytes);

/**
 * @brief Free a memory buffer in the library heap.
 *
 * @param mem Buffer to free.
 */
void nrf_modem_os_free(void *mem);

/* @brief Put a thread to a sleep for a specific time or until an event occurs.
 *
 * @param[in]      context   A unique identifier assigned by the library to identify the context.
 * @param[in, out] timeout   A pointer to the time-out value, in milliseconds. -1 for infinite
 *                           time-out. Contains the time-out value as input, remainig time to sleep
 *                           as output.
 *
 * @retval 0             If the procedure succeeded - it was interrupted by the RPC.
 * @retval NRF_ETIMEDOUT If a time-out condition occured.
 * @retval Other         Some other, OS specific error took place. The error code shall
 *                       belong to the nrf_errno error space.
 */
int32_t nrf_modem_os_timedwait(uint32_t context, int32_t *timeout);

/**
 * @brief Set errno.
 *
 * @param errno_val errno to set.
 */
void nrf_modem_os_errno_set(int errno_val);

/**
 * @brief Set the application IRQ, @c NRF_MODEM_APPLICATION_IRQ.
 */
void nrf_modem_os_application_irq_set(void);

/**
 * @brief Clear the application IRQ, @c NRF_MODEM_APPLICATION_IRQ.
 */
void nrf_modem_os_application_irq_clear(void);

/**
 * @brief Set the Trace IRQ.
 */
void nrf_modem_os_trace_irq_set(void);

/**
 * @brief Clear the Trace IRQ.
 */
void nrf_modem_os_trace_irq_clear(void);

/**
 * @brief Output Trace data from the trace buffer.
 *
 * @param data Memory buffer containing the output trace data.
 * @param len  Memory buffer length.
 * @return int32_t 0 on success, an errno otherwise.
 */
int32_t nrf_modem_os_trace_put(const uint8_t *data, uint32_t len);

/**
 * @brief Application IRQ handler in the modem library.
 *
 * Call this function when handling the Application IRQ.
 */
void nrf_modem_os_application_irq_handler(void);

/**
 * @brief Trace IRQ handler in the modem library.
 *
 * Call this function when handling the Trace IRQ.
 */
void nrf_modem_os_trace_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_OS_H__ */

/** @} */
