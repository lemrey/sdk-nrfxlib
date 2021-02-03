Full Modem firmware update
##########################

The modem library supports updating the full modem firmware when initialized in Full DFU mode via the ``nrf_modem_full_dfu.h`` interface.
When the library is initialized in Full DFU mode, the only operation that can be carried out is a full modem firmware update.
In particular, sockets are not available to the application.

It is possible to switch between Full DFU and Normal operation modes by re-initializing the library.

Memory Requirements
*******************

The total size of all shared memory regions must be at least 8220 bytes.
This is because programming requires 8 kB data buffer and some storage for
structures used in RPC communication.
@note:
When using NCS, library initialization is done via the glue, which will configure the library shared memory regions' size and position in RAM.
Use the Kconfig options belonging to the glue to ensure that the total size of the library shared memory region is as large as necessary.

Modem library initialization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to update the full modem firmware, the library must be initialized in `FULL_DFU` mode as shown below::

	/* Shutdown modem to prepare for DFU */
	nrf_modem_init(params, FULL_DFU_MODE);

If the library has already been initialized in `NORMAL_MODE` it must be shut down via `nrf_modem_shutdown` and re-initialized as shown below::

	nrf_modem_init(params, NORMAL_MODE);
	/* Shutdown modem to prepare for DFU */
	nrf_modem_shutdown();
	nrf_modem_init(params, FULL_DFU_MODE);

Considerations for corrupted modem firmware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When designing your application, you might also consider whether you must
tolerate interrupted DFU process, which leads to corrupted modem firmware.
There might be various reasons, for example out of power situation, that might
interrupt the update. Modem does not have back-up ROM and therefore interrupting
the DFU leads to situation where modem does not boot.

To workaround this situation, either application must tolerate the situation,
or another means of recovery is provided, for example re-flashing the modem with
PC tools.

And then initialize modem library manually::
	int main(void)
	{
		int rc = nrf_modem_init(params, NORMAL_MODE);

		if (rc < 0) {
			/* Recover the modem firmware from external flash */
			nrf_modem_shutdown();
			nrf_modem_init(params, FULL_DFU_MODE);
			/* Perform FULL modem firmware update. */
			nrf_modem_shutdown();
			nrf_modem_init(params, NORMAL_MODE);
		}
		/* Modem firmware updated, continue as normal */
	}


Full DFU API
************

A full modem firmware upgrade is done with the following steps:

1. Initialization
2. Programming the bootloader
3. Programming the modem firmware
4. Verification

Bootloader is always the first segment of firmware package. It must be programmed first.

When any failures happen, sequence should be started again from initialization phase.

Initialization
~~~~~~~~~~~~~~

To initialize the full modem firmware update process call::
	int nrf_modem_full_dfu_init(struct nrf_modem_full_dfu_digest *digest_buffer);

Programming the bootloader
~~~~~~~~~~~~~~~~~~~~

To program a bootloader, call following function:
::
	int nrf_modem_full_dfu_bl_write(uint32_t len, void *src)

The bootloader may be written in smaller chuncks, which the library internally
appends together. When all pieces are written, call::
	int nrf_modem_full_dfu_apply(void)

After a succesfull call, the modem is in DFU mode, and you may follow with writing
firmware segments or issue any other DFU commands, like verify.

Programming the modem firmware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Firmware segments are written with::
	int nrf_modem_full_dfu_fw_write(uint32_t addr, uint32_t len, void *src)

The modem library internally appends small chuncks together and does full page
programming. Sequantial writes may be written as small chuncks as preferred,
but must not come in random order. When given address is not contigious,
previously buffered data is programmed to a modem.

When all segments have been written, you must call::
	int nrf_modem_full_dfu_apply(void)

Verification
~~~~~~~~~~~~

To verify the content of the modem flash, use::
	nrf_modem_full_dfu_digest(uint32_t addr, uint32_t size,
			     struct nrf_modem_full_dfu_digest *digest_buffer);

That function calculates SHA-256 hash over given FLASH area. Hash can
then be compared to precalculated value, received with the modem firmware
package, to ensure that the image is programmed succesfully.
