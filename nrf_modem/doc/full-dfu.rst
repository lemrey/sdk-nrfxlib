Full Modem firmware update
##########################

NRF libraries support updating the full modem firmware, as opposed
of delta update, which is the normally used. In following sections the full
update is referred as Full DFU or just DFU in short. The API is introduced in
``nrf_modem_full_dfu.h``

Requirements
************

In order to use the full DFU, few requirements must be fulfilled
- Application build must allocate 8220 bytes for shared memory segment.
- Modem library must be shut down and initialized in ``FULL_DFU`` mode.


Build configuration
~~~~~~~~~~~~~~~~~~~

NRF library exposes following Kconfig variables that affect the size of
shared memory area reserved for modem.

* ``CONFIG_NRF_MODEM_LIB_SHMEM_CTRL_SIZE``
* ``CONFIG_NRF_MODEM_LIB_SHMEM_TX_SIZE``
* ``CONFIG_NRF_MODEM_LIB_SHMEM_RX_SIZE``
* ``CONFIG_NRF_MODEM_LIB_SHMEM_TRACE_SIZE``

Control segment size is statically set to 1256 bytes, and trace segment is mostly not used,
so effectively application should share the remaining 6964 bytes to TX and RX
segments. For example, following values in ``prj.conf`` is sufficient.::

	CONFIG_NRF_MODEM_LIB_SHMEM_TX_SIZE=4000
	CONFIG_NRF_MODEM_LIB_SHMEM_RX_SIZE=4000

Note that size of SHM region may be bigger. 8220 bytes is just a minimum
required for DFU to work.


Modem library initialization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to use the DFU mode, we must first shut down all modem library
operations and re-initialize with DFU mode.::

	/* Shutdown modem to prepare for DFU */
	nrf_modem_lib_shutdown();
	nrf_modem_lib_init(FULL_DFU_MODE);


Considerations for corrupted modem firmware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When designing your application, you might also consider whether you must
tolerate interrupted DFU process, which leads to corrupted modem firmware.
There might be various reasons, for example out of power situation, that might
interrupt the update. Modem does not have back-up ROM and therefore interrupting
the DFU leads to situation where modem does not boot.

When developing application, where modem library is initialized when Zephyr
boots, application cannot boot if modem firmware is corrupted. This is because
``nrf_modem_lib_init()`` cannot complete, and therefore we never enter
``main()`` function.

To workaround this situation, either application must tolerate the situation,
or another means of recovery is provided, for example re-flashing the modem with
PC tools.

To develop application that tolerate corrupted modem firmware, you must
handle the modem library initialization manually. Set following Kconfig::

	CONFIG_NRF_MODEM_LIB_SYS_INIT=n

And then initialize modem library manually::
	int main(void)
	{
		int rc = nrf_modem_lib_init(NORMAL_MODE);

		if (rc) {
			// Failed, I must recover the modem firmware from external flash
			nrf_modem_lib_shutdown();
			nrf_modem_lib_init(FULL_DFU_MODE);
			// continue to DFU recovery
			...
		}
		// Modem works OK, continue to normal application flow.
	}


Full DFU API
************

Firmware upgrade is done using following cycle

1. Initialize
1. Program a bootloader
1. Program FW segments
1. Verify

Bootloader is always the first segment of firmware package. It must be programmed first.

When any failures happen, sequence should be started again from initialization phase.

Initialization
~~~~~~~~~~~~~~

To initialize the Full DFU library call::
	int nrf_modem_full_dfu_init(struct nrf_modem_full_dfu_digest *digest_buffer);


Program a bootloader
~~~~~~~~~~~~~~~~~~~~

To program a bootloader, call following function:
::
	int nrf_modem_full_dfu_bl_write(uint32_t len, void *src)

Bootloader may be written in smaller chuncks, which DFU library internally
appends together. When all pieces are written, call::
	int nrf_modem_full_dfu_apply(void)

After succesfull call, the modem is in DFU mode, and you may follow with writing
firmware segments or issue any other DFU commands, like verify.

Write firmware segments
~~~~~~~~~~~~~~~~~~~~~~~

Firmware segments are written with::
	int nrf_modem_full_dfu_fw_write(uint32_t addr, uint32_t len, void *src)

DFU library internally appends small chuncks together and does full page
programming. Sequantial writes may be written as small chuncks as preferred,
but must not come in random order. When given address is not contigious,
previously buffered data is programmed to a modem.

When all segments have been written, you must call::
	int nrf_modem_full_dfu_apply(void)

Verify
~~~~~~
TODO.
