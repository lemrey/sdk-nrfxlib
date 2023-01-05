.. _nrf_modem_softsim:

SoftSIM interface
#################

.. contents::
   :local:
   :depth: 2

.. _SIM card: https://en.wikipedia.org/wiki/SIM_card

SoftSIM is a form of `SIM card`_ that is implemented in software and programmed into a device.
The SoftSIM interface in the Modem library is used to mediate between the Modem and the SoftSIM implementation.
When the Modem makes a SoftSIM request, the SoftSIM interface handles it and forwards it to the SoftSIM Implementation. The SoftSIM implementation then generates a response and via the SoftSIM interface responds to the Modem.

Handling SoftSIM requests
*************************

To handle requests from the SoftSIM interface, the application needs to implement the :c:func:`nrf_modem_softsim_req_send` function.
This function is then called by the interface whenever there is a new SoftSIM request and possibly associated data available.
Once the request has been handled, the apllication needs to call the :c:func:`nrf_modem_softsim_res_send` function with the response data if available.
For every SoftSIM request, it is expected a response, even in case of failure.

.. code-block:: c

   int nrf_modem_softsim_req_send(enum nrf_modem_softsim_cmd cmd, const uint8_t *data, uint16_t data_len)
   {
       uint8_t *out = NULL;
       uint16_t out_len = 0;

       switch (cmd)
       {
       case NRF_MODEM_SOFTSIM_INIT:
           /* Handle INIT, update `out` and `out_len`. */
           ...
           nrf_modem_softsim_res_send(out, out_len);
           break;
       case NRF_MODEM_SOFTSIM_APDU:
           /* Handle APDU, update `out` and `out_len`. */
           ...
           nrf_modem_softsim_res_send(out, out_len);
           break;
       case NRF_MODEM_SOFTSIM_DEINIT:
           /* Handle DEINIT. */
           ...
           nrf_modem_softsim_res_send(NULL, 0);
           break;
       case NRF_MODEM_SOFTSIM_RESET:
           /* Handle RESET. */
           ...
           nrf_modem_softsim_res_send(NULL, 0);
           break;
       }

       return 0;
   }

.. note::

   The :c:func:`nrf_modem_softsim_req_send` function is called in an interrupt context so it is recommended to have an asynchronous implementation that returns immediately. This example shows a synchronous implementation for demonstration purposes only.

If errors happen while handling a SoftSIM request, it is recommended to handle them gracefully by calling the :c:func:`nrf_modem_softsim_res_send` with no data.

.. code-block:: c

   ...
   case NRF_MODEM_SOFTSIM_APDU:
       /* Handle APDU, update `out` and `out_len`. */
       ...
       /* APDU failure. */
       if (err) {
           nrf_modem_softsim_res_send(NULL, 0);
           break;
       }

       nrf_modem_softsim_res_send(out, out_len);
       break;
   ...

.. note::

   The type of failure is not important to the Modem so we don't need to provide any data when responding.
