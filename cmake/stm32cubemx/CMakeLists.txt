cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# STM32CubeMX generated symbols (macros)
set(MX_Defines_Syms 
	MBEDTLS_CONFIG_FILE=<mbedtls_config.h> 
	USE_PWR_LDO_SUPPLY 
	USE_HAL_DRIVER 
	STM32H743xx
    $<$<CONFIG:Debug>:DEBUG>
)

# STM32CubeMX generated include paths
set(MX_Include_Dirs
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/MBEDTLS/App
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/include/mbedtls
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
)

# STM32CubeMX generated application sources
set(MX_Application_Src
    ${CMAKE_SOURCE_DIR}/MBEDTLS/App/mbedtls.c
    ${CMAKE_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_SOURCE_DIR}/Core/Src/gpio.c
    ${CMAKE_SOURCE_DIR}/Core/Src/freertos.c
    ${CMAKE_SOURCE_DIR}/Core/Src/memorymap.c
    ${CMAKE_SOURCE_DIR}/Core/Src/usart.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32h7xx_it.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32h7xx_hal_msp.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32h7xx_hal_timebase_tim.c
    ${CMAKE_SOURCE_DIR}/Core/Src/sysmem.c
    ${CMAKE_SOURCE_DIR}/Core/Src/syscalls.c
    ${CMAKE_SOURCE_DIR}/startup_stm32h743xx.s
)

# STM32 HAL/LL Drivers
set(STM32_Drivers_Src
    ${CMAKE_SOURCE_DIR}/Core/Src/system_stm32h7xx.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
)

# Drivers Midllewares


set(FreeRTOS_Src
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)
set(mbedTLS_Src
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/aes.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/aesni.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/arc4.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/asn1parse.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/asn1write.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/base64.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/bignum.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/blowfish.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/camellia.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ccm.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/certs.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/chacha20.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/chachapoly.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/cipher.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/cipher_wrap.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ctr_drbg.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/des.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/dhm.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ecdh.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ecdsa.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ecp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ecp_curves.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/entropy.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/entropy_poll.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/error.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/gcm.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/hkdf.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/hmac_drbg.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/md.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/md5.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/md_wrap.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/memory_buffer_alloc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/oid.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/padlock.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pem.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pk.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pkcs12.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pkcs5.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pkparse.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pkwrite.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/pk_wrap.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/platform.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/platform_util.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/poly1305.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ripemd160.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/rsa.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/rsa_internal.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/sha1.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/sha256.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/sha512.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/ssl_ciphersuites.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/version.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/version_features.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509write_crt.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509write_csr.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509_create.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509_crl.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509_crt.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/x509_csr.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/mbedTLS/library/xtea.c
)

# Link directories setup
set(MX_LINK_DIRS

)
# Project static libraries
set(MX_LINK_LIBS 
    STM32_Drivers
    FreeRTOS	mbedTLS	
)
# Interface library for includes and symbols
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${MX_Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${MX_Defines_Syms})

# Create STM32_Drivers static library
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32cubemx)


# Create FreeRTOS static library
add_library(FreeRTOS OBJECT)
target_sources(FreeRTOS PRIVATE ${FreeRTOS_Src})
target_link_libraries(FreeRTOS PUBLIC stm32cubemx)

# Create mbedTLS static library
add_library(mbedTLS OBJECT)
target_sources(mbedTLS PRIVATE ${mbedTLS_Src})
target_link_libraries(mbedTLS PUBLIC stm32cubemx)

# Add STM32CubeMX generated application sources to the project
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MX_Application_Src})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${MX_LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()
