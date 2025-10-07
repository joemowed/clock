#required for cross-compiling to device with no OS
set(CMAKE_SYSTEM_NAME Generic)
#required for cross-compiling to device with no OS
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_CXX_COMPILER_FORCED TRUE)
#probably not required
set(CMAKE_CXX_COMPILER_ID GNU)

# Some default GCC settings
# arm-none-eabi- must be part of path environment

set(TOOLCHAIN_PREFIX                arm-none-eabi-)
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER              ${CMAKE_CXX_COMPILER})
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

#compile things as static and not as executables
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#set(TARGET_FLAGS "-mcpu=cortex-m4 fpu=fpv5-sp-d16 -mfloat-abi=hard ")
set(TARGET_FLAGS "-mcpu=cortex-m4")

set(CMAKE_CXX_FLAGS "${TARGET_FLAGS}")
#enable extra warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -Wpedantic ")
#enable more memory sectioning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g0")
endif()
#unknown -MMD,-MP, -xassembler-with-cpp enables c header symbols when assembling .asm files
set(CMAKE_ASM_FLAGS "${CMAKE_CXX_FLAGS} -xassembler-with-cpp -MMD -MP")
#no run-time-type-info, no exeptions, no atomic statics
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

#processor specific flags
set(CMAKE_CXX_LINK_FLAGS "${TARGET_FLAGS}")
#set linker file location
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/samd51j20a_flash.ld\"")
#link against the newlib-nosys lib
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --specs=nosys.specs")
#get rid of unused vars, make the map
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")
