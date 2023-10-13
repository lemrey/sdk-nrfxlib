#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_MODEM_LINK_BINARY)

  string(REGEX REPLACE "_[a-zA-Z][a-zA-Z][a-zA-Z][a-zA-Z]$" "" arch_soc_dir ${CONFIG_SOC})

  if(NOT arch_soc_dir MATCHES "nRF9160")
  string(REGEX REPLACE "nRF91[0-9]*" "nRF9120" arch_soc_dir ${arch_soc_dir})
  endif()

  if(CONFIG_FPU)
    if(CONFIG_FP_HARDABI)
      set(float_dir hard-float)
    elseif(CONFIG_FP_SOFTABI)
      set(float_dir soft-float)
    else()
      assert(0 "Unreachable code")
    endif()
  else()
    set(float_dir soft-float)
  endif()

  set(NRF_MODEM_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/${arch_soc_dir}/${float_dir}")

  if(CONFIG_NRF_MODEM_DEFAULT)
    zephyr_library_import(modem ${NRF_MODEM_LIB_DIR}/libmodem.a)
  elseif(CONFIG_NRF_MODEM_BINARY_LOG)
    zephyr_library_import(modem ${NRF_MODEM_LIB_DIR}/libmodem_log.a)
  else()
    assert(0 "Unreachable code")
  endif()

  zephyr_include_directories(include)

endif()
