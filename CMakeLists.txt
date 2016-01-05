# Copyright (c) 2014, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

SET(PROJECT smartDeviceLinkCore)

#compiler set
#MESSAGE(STATUS ${CMAKE_SYSTEM_NAME})
IF(SYSTEM_NAME STREQUAL "Android")
SET(CMAKE_SYSTEM_NAME Android)
ENDIF()

#when build on android plat,you should set -DCOMPILER_PATH=your compiler root path
IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    IF(NOT COMPILER_PATH)
    SET(COMPILER_PATH "/opt/android-9-toolchain")
    ENDIF()
    MESSAGE(STATUS "Current cross compiler path is ${COMPILER_PATH}")
    SET(CMAKE_CXX_COMPILER "${COMPILER_PATH}/bin/arm-linux-androideabi-g++")
    SET(CMAKE_C_COMPILER "${COMPILER_PATH}/bin/arm-linux-androideabi-gcc")
ENDIF()

MESSAGE(STATUS "Current build system name is ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "Current CMAKE_BUIDL_TYPE is ${CMAKE_BUIDL_TYPE}")

#Jenkins integration section
#dont modify this section if you don't know details about integration with Jenkins!!!
SET(HMI "web" CACHE STRING "HMI type")
option(HMI2 "Use Qt HMI" OFF)
option(EXTENDED_MEDIA_MODE "Turn on and off extended Madia Manager features relates to PulseAudio A2DP and GStreamer" OFF)
option(BUILD_SHARED_LIBS "Build all libraries as shared (if ON) or static (if OFF)" OFF)
option(BUILD_BT_SUPPORT "Bluetooth support" ON)
option(BUILD_USB_SUPPORT "libusb support" ON)
option(BUILD_AVAHI_SUPPORT "libavahi support" ON)
option(BUILD_RWLOCK_SUPPORT "rwlocks support" OFF)
option(BUILD_BACKTRACE_SUPPORT "backtrace support" OFF)
option(BUILD_TESTS "Possibility to build and run tests" OFF)
option(TIME_TESTER "Enable profiling time test util" OFF)
option(BUILD_TESTS_WITH_HMI "Possibility to build and run HMI tests" OFF)
option(ENABLE_LOG "Logging feature" ON)
option(ENABLE_GCOV "gcov code coverage feature" OFF)
option(ENABLE_SANITIZE "Sanitize tool" OFF)
option(ENABLE_SECURITY "Security Ford protocol protection" ON)
option(MODIFY_FUNCTION_FLAGS "modify function" ON)
IF(MODIFY_FUNCTION_FLAGS)
option(BUILD_MANAGESDL_SUPPORT "manage sdl support" OFF)
ENDIF()
IF(MODIFY_FUNCTION_FLAGS)
option(BUILD_TARGET_LIBRARY    "build target as library" OFF)
ENDIF()
option(ENABLE_HMI_PTU_DECRYPTION "Policy table update parsed by hmi" ON)

SET(OS_TYPE_OPTION     "$ENV{OS_TYPE}")
SET(DEBUG_OPTION     "$ENV{DEBUG}")
SET(HMI_TYPE_OPTION     "$ENV{HMI_TYPE}")
SET(TARGET_OPTION     "$ENV{TARGET}")
SET(MEDIA_MODE_OPTION     "$ENV{MEDIA_MODE}")
SET(HMI_ADAPTER_OPTION     "$ENV{HMI_ADAPTER}")
SET(ENABLE_LOG_OPTION     "$ENV{ENABLE_LOG}")
SET(ARCH_TYPE_OPTION    "$ENV{ARCH_TYPE}")
SET(POLICY_OPTION     "$ENV{POLICY_TYPE}")
SET(SECURITY_OPTION     "$ENV{SECURITY_MODE}")
SET(COMPONENTS_DIR ${CMAKE_SOURCE_DIR}/src/components)
SET(SNAPSHOT_TAG     "$ENV{SNAPSHOT_TAG}")



IF(ARCH_TYPE_OPTION)
    IF(NOT (${ARCH_TYPE_OPTION} STREQUAL "x86") AND NOT (${ARCH_TYPE_OPTION} STREQUAL "armv7"))
    MESSAGE(AUTHOR_WARNING "HW architecture is not defined, using x86. Allowed values are x86/armv7 (case sensitive)")
    SET(ARCH_TYPE_OPTION    "x86")
    ENDIF()
ELSE ()
    SET(ARCH_TYPE_OPTION    "x86")
ENDIF()

SET(objcopy "objcopy")
IF(OS_TYPE_OPTION)
    IF(${OS_TYPE_OPTION} STREQUAL "QNX")
    MESSAGE(STATUS "Jenkins integration: set build process for QNX")
    #do not use include after project() command.
    #Such usecase results in infinite cycle of reinitialization of compiler and other variables
    INCLUDE("./qnx_6.5.0_linux_x86.cmake")
    SET(objcopy "nto${ARCH_TYPE_OPTION}-objcopy")
    #tests are not supported yet for QNX build
    SET(BUILD_TESTS OFF)
    ENDIF()
ENDIF()

IF(HMI_TYPE_OPTION)
    IF(${HMI_TYPE_OPTION} STREQUAL "HTML5")
    MESSAGE(STATUS "Jenkins integration: select HTML5 HMI")
    SET(HMI "web")
    ELSEIF(${HMI_TYPE_OPTION} STREQUAL "NONE")
    MESSAGE(STATUS "Jenkins integration: select HMI none")
    SET(HMI "no")
    ELSE ()
    MESSAGE(STATUS "Jenkins integration: select QML HMI none")
    SET(HMI "qt")
    ENDIF()
ENDIF()

IF(MEDIA_MODE_OPTION)
    IF(${MEDIA_MODE_OPTION} STREQUAL "EXTENDED_MEDIA")
    MESSAGE(STATUS "Jenkins integration: select extended media mode")
    SET(EXTENDED_MEDIA_MODE ON)
    ENDIF()
ENDIF()

IF(DEBUG_OPTION)
    IF(${DEBUG_OPTION} STREQUAL "DBG_OFF")
    MESSAGE(STATUS "Jenkins integration: build release version")
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    ENDIF()
ENDIF()

IF(HMI_ADAPTER_OPTION)
    IF(${HMI_ADAPTER_OPTION} STREQUAL "MESSAGEBROKER")
    MESSAGE(STATUS "Jenkins integration: selected HMI adapter MESSAGEBROKER")
    SET(HMIADAPTER "messagebroker")
    ELSEIF(${HMI_ADAPTER_OPTION} STREQUAL "DBUS")
    MESSAGE(STATUS "Jenkins integration: selected HMI adapter DBUS")
    SET(HMIADAPTER "dbus")
    ELSEIF(${HMI_ADAPTER_OPTION} STREQUAL "MQUEUE")
    MESSAGE(STATUS "Jenkins integration: selected HMI adapter MQUEUE")
    SET(HMIADAPTER "mqueue")
    ENDIF()
ENDIF()

IF(ENABLE_LOG_OPTION)
    IF(${ENABLE_LOG_OPTION} STREQUAL "LOG_OFF")
    MESSAGE(STATUS "Jenkins integration: Log is turned off")
    SET(ENABLE_LOG OFF)
    ENDIF()
ENDIF()


IF(SECURITY_OPTION)
    IF(${SECURITY_OPTION} STREQUAL "SEC_OFF")
    MESSAGE(STATUS "Jenkins integration: Security is turned OFF")
    SET(ENABLE_SECURITY OFF)
    ENDIF()
ENDIF()

IF(NOT CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
#Jenkins integration section end

ADD_CUSTOM_TARGET(pasa-tarball
    COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} pasa
    COMMAND tar -cz -C /tmp/PASA -f ${CMAKE_BINARY_DIR}/pasa.tar.gz .
    DEPENDS HMI_API MOBILE_API v4_protocol_v1_2_no_extra
)

ADD_CUSTOM_TARGET(ford-tarball
    COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ford
    COMMAND tar -cz -C /tmp/FORD -f ${CMAKE_BINARY_DIR}/ford.tar.gz .
    DEPENDS HMI_API MOBILE_API v4_protocol_v1_2_no_extra
)

ADD_CUSTOM_TARGET(genivi-tarball
    COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} genivi
    COMMAND tar -cz -C /tmp/GENIVI -f ${CMAKE_BINARY_DIR}/genivi.tar.gz .
)

ENDIF()

project (${PROJECT})

#ADD_DEPENDENCIES(${PROJECT} Policy)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Please do not change compiler/linker flags if You do not know how particular
# flag is handled by CMake
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
SET(ARCHIVE_OUTPUT_DIRECTORY ./bin)
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL    "Android")
    SET(CMAKE_CXX_FLAGS "-fPIC -std=gnu++0x -Wall -Werror -Wuninitialized -Wvla")
    IF(ENABLE_SANITIZE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    ENDIF()
    IF(ENABLE_GCOV)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    ADD_DEFINITIONS(-DGCOV_ENABLED)
    ENDIF()
    SET(CMAKE_CXX_FLAGS_RELEASE "-fPIC -s -O2") #It will be appended to CMAKE_CXX_FLAGS in release
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    SET(CMAKE_CXX_FLAGS "-g3 -ggdb3 -std=gnu++0x -Werror=return-type -Wuninitialized --coverage -std=c++0x") # -stdlib=libc++ -m32
    SET(CMAKE_CXX_FLAGS_RELEASE "-s -O2") #It will be appended to CMAKE_CXX_FLAGS in release
    #SET(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(CMAKE_CXX_FLAGS "/wd\"4819\" /wd\"4244\" /wd\"4390\" /wd\"4430\" /EHsc /bigobj")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG") #It will be appended to CMAKE_CXX_FLAGS in release
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    SET(CMAKE_CXX_FLAGS "/wd\"4819\" /wd\"4244\" /wd\"4390\" /wd\"4996\" /EHsc /bigobj /Z7 /GF")
 SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG") #It will be appended to CMAKE_CXX_FLAGS in release
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    ADD_DEFINITIONS(-DOS_LINUX)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    ADD_DEFINITIONS(-DOS_QNX)
    SET(BUILD_BT_SUPPORT OFF)
    SET(BUILD_AVAHI_SUPPORT OFF)
    SET(BUILD_BACKTRACE_SUPPORT OFF)
    SET(EXTENDED_MEDIA_MODE OFF)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    ADD_DEFINITIONS(-DOS_WIN32)
    ADD_DEFINITIONS(-DWIN32)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
    ADD_DEFINITIONS(-DASR_HAS_VR)
    SET(BUILD_AVAHI_SUPPORT OFF)
    SET(BUILD_USB_SUPPORT ON)
    SET(BUILD_BT_SUPPORT OFF)
    SET(EXTENDED_MEDIA_MODE OFF)
    SET(ENABLE_LOG ON)
    SET(ENABLE_SECURITY OFF)
    ADD_DEFINITIONS(-DLOG4CXX_STATIC)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    ADD_DEFINITIONS(-D_WIN32_WCE=$(CEVER))
    ADD_DEFINITIONS(-D_WINDOWS)
    ADD_DEFINITIONS(-DUNDER_CE)
    ADD_DEFINITIONS(-D$(ARCHFAM))
    ADD_DEFINITIONS(-D$(_ARCHFAM_))
    ADD_DEFINITIONS(-D$(PLATFORMDEFINES))
    ADD_DEFINITIONS(-DOS_WINCE)
    ADD_DEFINITIONS(-DHAVE_PTW32_CONFIG_H)
    ADD_DEFINITIONS(-D_UNICODE)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-DWINCE)
    ADD_DEFINITIONS(-D_WIN32)
    ADD_DEFINITIONS(-DLOG4CXX_STATIC)
    ADD_DEFINITIONS(-DAPR_DECLARE_STATIC)
    SET(ENABLE_LOG OFF)
    SET(ENABLE_SECURITY OFF)
    SET(BUILD_BT_SUPPORT OFF)
    SET(BUILD_USB_SUPPORT OFF)
    SET(BUILD_AVAHI_SUPPORT OFF)
    SET(BUILD_BACKTRACE_SUPPORT OFF)
    SET(EXTENDED_MEDIA_MODE OFF)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    ADD_DEFINITIONS(-DOS_MAC)
    SET(BUILD_BT_SUPPORT OFF)
    SET(BUILD_USB_SUPPORT OFF)
    SET(BUILD_AVAHI_SUPPORT OFF)
    SET(BUILD_BACKTRACE_SUPPORT OFF)
    SET(EXTENDED_MEDIA_MODE OFF)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL    "Android")
    ADD_DEFINITIONS(-DOS_LINUX)
    ADD_DEFINITIONS(-DOS_ANDROID)
    SET(BUILD_BT_SUPPORT OFF)
    SET(BUILD_USB_SUPPORT ON)
    SET(BUILD_AVAHI_SUPPORT OFF)
    SET(BUILD_BACKTRACE_SUPPORT OFF)
ENDIF()
IF(MODIFY_FUNCTION_FLAGS)
ADD_DEFINITIONS(-DMODIFY_FUNCTION_SIGN)
ENDIF()

IF(BUILD_TARGET_LIBRARY)
    ADD_DEFINITIONS(-DBUILD_TARGET_LIB)
ENDIF()

IF(BUILD_USB_SUPPORT)
    ADD_DEFINITIONS(-DUSB_SUPPORT)
    MESSAGE(STATUS "USB support is enabled")
ENDIF()


IF(BUILD_BT_SUPPORT)
    ADD_DEFINITIONS(-DBLUETOOTH_SUPPORT)
    MESSAGE(STATUS "Bluetooth support is enabled")
ENDIF()

IF(BUILD_MME_SUPPORT)
    ADD_DEFINITIONS(-DMME_SUPPORT)
    IF(MME_MQ)
    ADD_DEFINITIONS(-DMME_MQ)
    ENDIF(MME_MQ)
ENDIF(BUILD_MME_SUPPORT)

IF(BUILD_AVAHI_SUPPORT)
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL    "Android")
    ADD_DEFINITIONS(-DAVAHI_SUPPORT)
    # --- Check libavahi-common, libavahi-client availability
    FIND_PACKAGE(Libavahi)
    MESSAGE(STATUS "Avahi support is enabled")
    ENDIF()
ENDIF()

IF(BUILD_RWLOCK_SUPPORT)
    ADD_DEFINITIONS(-DRWLOCK_SUPPORT)
ENDIF()

IF(BUILD_BACKTRACE_SUPPORT)
    ADD_DEFINITIONS(-DBACKTRACE_SUPPORT)
ENDIF()

IF(ENABLE_LOG)
	MESSAGE(STATUS "Add definition: " "ENABLE_LOG")
    ADD_DEFINITIONS(-DENABLE_LOG)
    SET(install-3rd_party_logger "install-3rd_party_logger")
ENDIF()

IF(TIME_TESTER)
    ADD_DEFINITIONS(-DTIME_TESTER)
ENDIF()

# TODO(AK): check current OS here
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX")
    ADD_DEFINITIONS(-DOS_POSIX)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    ADD_DEFINITIONS(-DOS_POSIX)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL    "Android")
    ADD_DEFINITIONS(-DOS_POSIX)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
#option(CMAKE_BUILD_TYPE "build type" "Debug")
#set(CMAKE_BUILD_TYPE "Release")
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG" )
    ADD_DEFINITIONS(-DDEBUG)
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE" )
    ADD_DEFINITIONS(-DRELEASE)
    ELSE()
    SET(CMAKE_BUILD_TYPE "Debug")
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG" )
    ADD_DEFINITIONS(-DDEBUG)
    ENDIF()
ELSE()
    # FIXME(DC): weird logic
    IF(CMAKE_C_FLAGS_DEBUG)
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG" )
    ADD_DEFINITIONS(-DDEBUG)
    ELSE (CMAKE_C_FLAGS_DEBUG)
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE" )
    ADD_DEFINITIONS(-DRELEASE)
    ENDIF(CMAKE_C_FLAGS_DEBUG)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    IF(EXTENDED_MEDIA_MODE)
    ADD_DEFINITIONS(-DEXTENDED_MEDIA_MODE)
    # required to find 'glibconfig.h'
    FIND_PACKAGE(PkgConfig)
    PKG_CHECK_MODULES(GLIB2 REQUIRED glib-2.0)
    ADD_DEFINITIONS(${GLIB2_CFLAGS})
    ENDIF()
ENDIF()


# --- Interface generator

FIND_PACKAGE(PythonInterp)

IF(NOT PYTHONINTERP_FOUND)
    MESSAGE(STATUS "Python interpreter is not found")
    MESSAGE(STATUS "To install it type in the command line:")
    MESSAGE(STATUS "sudo apt-get install python")
    MESSAGE(FATAL_ERROR "Exiting!")
ENDIF(NOT PYTHONINTERP_FOUND)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL    "Android")
    IF(HMI STREQUAL "qt")
    CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)
    IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
        SET(qt_version "4.8.5")
    ELSE ()
        SET(qt_version "5.1.0")
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_SOURCE_DIR}/FindQt.sh -v ${qt_version}
        OUTPUT_VARIABLE qt_bin_dir
    )
    MESSAGE(STATUS "Binary directory Qt ${qt_version} is ${qt_bin_dir}")
    SET(ENV{PATH} ${qt_bin_dir}:$ENV{PATH})

    IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
        FIND_PACKAGE(Qt4 ${qt_version} REQUIRED QtCore QtGui QtDBus QtDeclarative)
    ELSE ()
        FIND_PACKAGE(Qt5Core REQUIRED)
        FIND_PACKAGE(Qt5DBus REQUIRED)
        FIND_PACKAGE(Qt5Qml REQUIRED)
        FIND_PACKAGE(Qt5Quick REQUIRED)
        SET(qmlplugindump_binary ${qt_bin_dir}/qmlplugindump)
    ENDIF()
     ENDIF()
ENDIF()

SET(INTEFRACE_GENERATOR "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/Generator.py")
SET(INTEFRACE_GENERATOR_CMD ${PYTHON_EXECUTABLE} -B ${INTEFRACE_GENERATOR})
FILE(GLOB_RECURSE INTERFACE_GENERATOR_DEPENDENCIES "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/*.*")

MACRO(GenerateInterface arg_xml_name arg_namespace parser_type)
    string(REGEX MATCH "^[a-zA-Z_0-9]*[^.]" file_name ${arg_xml_name})     # TODO: make expression more robust

    SET(hpp_file
    "${CMAKE_CURRENT_BINARY_DIR}/${file_name}.h"
    "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.h"
    )

    SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.cc")
    SET(full_xml_name "${CMAKE_CURRENT_SOURCE_DIR}/${arg_xml_name}")

    IF(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    ADD_CUSTOM_COMMAND( OUTPUT ${hpp_file} ${cpp_file}
        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} ${arg_namespace} ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "${parser_type}"
        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
        COMMENT "Generating files:\n     ${hpp_file}\n     ${cpp_file}\nfrom:\n     ${arg_xml_name} ..."
        VERBATIM
    )
    ELSE()
    ADD_CUSTOM_COMMAND( OUTPUT ${hpp_file} ${cpp_file}
        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} ${arg_namespace} ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "${parser_type}"
        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
        COMMENT ""
        VERBATIM
    )
    ENDIF()

    INCLUDE_DIRECTORIES (
        ${COMPONENTS_DIR}/smart_objects/include
        ${COMPONENTS_DIR}/formatters/include/
        ${CMAKE_BINARY_DIR}
     )

     ADD_LIBRARY("${file_name}" ${cpp_file})
ENDMACRO(GenerateInterface)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    # --- Useful macro
    MACRO(create_test NAME SOURCES LIBS)
    ADD_EXECUTABLE("${NAME}" ${CMAKE_SOURCE_DIR}/src/components/test_main.cc ${SOURCES})
    TARGET_LINK_LIBRARIES("${NAME}" ${LIBS})
    TARGET_LINK_LIBRARIES("${NAME}" Utils)
    IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
        add_test(${NAME} ${CMAKE_SOURCE_DIR}/qnx/remote_run_test.sh ${NAME})
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_test(NAME ${NAME}
        COMMAND ${NAME} --gtest_output=xml:${CMAKE_BINARY_DIR}/test_results/)
    ELSE()
        add_test(${NAME} ${NAME})
    ENDIF()
    ENDMACRO(create_test)
ENDIF()


# Include direction and link library

IF(MODIFY_FUNCTION_FLAGS)
INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}/include/all
)
ENDIF()
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}/include/win32
    )
    LINK_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/lib/win32
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}/include/wince
    )
    LINK_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/lib/wince
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    INCLUDE_DIRECTORIES (
    /usr/local/Cellar/log4cxx/0.10.0/include
    )
    LINK_DIRECTORIES(
    /usr/local/Cellar/log4cxx/0.10.0/lib
    )
ENDIF()

# --replace in list macro
MACRO(LIST_REPLACE LIST INDEX NEWVALUE)
    LIST(INSERT ${LIST} ${INDEX} ${NEWVALUE})
    MATH(EXPR __INDEX "${INDEX} + 1")
    list (REMOVE_AT ${LIST} ${__INDEX})
ENDMACRO(LIST_REPLACE)


# Building application

# --- Type HMI
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    IF(HMI STREQUAL "qt")
    SET(QT_HMI ON)
    ADD_DEFINITIONS(-DQT_HMI)
    ELSEIF(HMI STREQUAL "web")
    SET(WEB_HMI ON)
    ADD_DEFINITIONS(-DWEB_HMI)
    ELSE ()
    SET(HMI "no")
    ADD_DEFINITIONS(-DNO_HMI)
    ENDIF()
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    SET(WEB_HMI ON)
    ADD_DEFINITIONS(-DWEB_HMI)
ENDIF()

IF(HMI STREQUAL "qt" AND NOT DEFINED HMIADAPTER)
    SET(HMIADAPTER "dbus")
ENDIF()
IF(HMI STREQUAL "web" AND NOT DEFINED HMIADAPTER)
    SET(HMIADAPTER "messagebroker")
ENDIF()
IF(HMI STREQUAL "no" AND NOT DEFINED HMIADAPTER)
    SET(HMIADAPTER "mqueue")
ENDIF()

IF(HMIADAPTER STREQUAL "dbus")
    SET(HMI_DBUS_API ON)
    ADD_DEFINITIONS(-DDBUS_HMIADAPTER)
    ADD_DEFINITIONS(-DHMI_DBUS_API)
    SET(install-3rd_party_dbus "install-3rd_party_dbus")
ENDIF()
IF(HMIADAPTER STREQUAL "messagebroker")
    SET(HMI_JSON_API ON)
    ADD_DEFINITIONS(-DMESSAGEBROKER_HMIADAPTER)
    ADD_DEFINITIONS(-DHMI_JSON_API)
ENDIF()
IF(HMIADAPTER STREQUAL "mqueue")
    SET(HMI_JSON_API ON)
    ADD_DEFINITIONS(-DMQUEUE_HMIADAPTER)
    ADD_DEFINITIONS(-DHMI_JSON_API)
ENDIF()

# --- Directory with SDL interfaces, global types and ProtocolLib component
INCLUDE_DIRECTORIES(
    ${COMPONENTS_DIR}/include
    ${COMPONENTS_DIR}/protocol/include
)

# --- 3rd party libs
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/src/3rd_party/set_3rd_party_paths.cmake)

SET(3RD_PARTY_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/3rd_party)
SET(3RD_PARTY_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/3rd_party)

SET(install-3rd_party_logger_var "")
SET(install-3rd_party_dbus_var "")

IF(NO_REBUILD_3RD_PARTY)
    SET(NO_REBUILD_3RD_PARTY_LOGGER ON)
    SET(NO_REBUILD_3RD_PARTY_DBUS ON)
ENDIF()

IF(FORCE_3RD_PARTY)
    IF(NO_REBUILD_3RD_PARTY)
    MESSAGE(FATAL_ERROR "Please don't turn on both FORCE_3RD_PARTY and NO_REBUILD_3RD_PARTY at the same time.")
    ELSE()
    SET(FORCE_3RD_PARTY_LOGGER ON)
    SET(FORCE_3RD_PARTY_DBUS ON)
    ENDIF()
ENDIF()

IF(ENABLE_LOG)
    IF(NO_REBUILD_3RD_PARTY_LOGGER)
    MESSAGE(STATUS "Not rebuilding logger.")
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    MESSAGE(STATUS "CMAKE_SYSTEM_NAME is Windows.")
    ELSE()
    IF(FORCE_3RD_PARTY_LOGGER)
        MESSAGE(STATUS "Force to rebuild logger.")
        
        #build logger
        ADD_CUSTOM_TARGET(3rd_party_logger
        make 
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
        
        #install logger
        #install either to default place with sudo or non-default plase without sudo.
        #to install with sudo to non-default place use manual installation
        ADD_CUSTOM_TARGET(install-3rd_party_logger
        COMMAND /bin/bash -c \"USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\; 
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then 
                                 sudo -k \; 
                                 sudo make install\; 
                                 else 
                                 make install\; 
                                 fi\"
        DEPENDS 3rd_party_logger
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
    ELSE()
        #build logger
        ADD_CUSTOM_TARGET(3rd_party_logger
        COMMAND /bin/bash -c \"cd ${CMAKE_CURRENT_SOURCE_DIR} && 
                                 grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 1>/dev/null 2>&1\; 
                                 if [ \\$$? == 0 ]\; then 
                                 VAR1=\\$$\( readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 2>/dev/null\)\; 
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\; 
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then 
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/apache-log4cxx-0.10.0\)\; 
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then 
                                     echo " Need to rebuild logger. " \; 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\; 
                                     make\; 
                                 else 
                                     echo " Logger is actual. " \; 
                                 fi\; 
                                 else 
                                 echo " Need to build logger. " \; 
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}\; 
                                 make\; 
                                 fi\"
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
        
        #install logger
        #install either to default place with sudo or non-default plase without sudo.
        #to install with sudo to non-default place use manual installation
        ADD_CUSTOM_TARGET(install-3rd_party_logger
        COMMAND /bin/bash -c \"cd ${CMAKE_CURRENT_SOURCE_DIR} && 
                                 grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 1>/dev/null 2>&1\; 
                                 if [ \\$$? == 0 ]\; then 
                                 VAR1=\\$$\( readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 2>/dev/null\)\; 
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\; 
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then 
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/apache-log4cxx-0.10.0\)\; 
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then 
                                     USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\; 
                                     if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     sudo -k \; 
                                     sudo make install\; 
                                     else 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     make install\; 
                                     fi\; 
                                 fi\; 
                                 else 
                                 USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\; 
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     sudo -k \; 
                                     sudo make install\; 
                                 else 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     make install\; 
                                 fi\;
                                 fi\"
        DEPENDS 3rd_party_logger
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
    ENDIF()
        
    SET(install-3rd_party_logger_var "install-3rd_party_logger")
    ENDIF()
ENDIF()

IF(HMIADAPTER STREQUAL "dbus")
    IF(NO_REBUILD_3RD_PARTY_DBUS)
    MESSAGE(STATUS "Not rebuilding D-Bus.")
    ELSE()
    IF(FORCE_3RD_PARTY_DBUS)
        MESSAGE(STATUS "Force to rebuild D-Bus.")
        
        #build d-bus
        ADD_CUSTOM_TARGET(3rd_party_dbus
        make
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
        
        #install d-bus
        #install either to default place with sudo or non-default plase without sudo.
        #to install with sudo to non-default place use manual installation
        ADD_CUSTOM_TARGET(install-3rd_party_dbus
        COMMAND /bin/bash -c \"USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\; 
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then 
                                 sudo -k \; 
                                 sudo make install\; 
                                 else 
                                 make install\; 
                                 fi\"
        DEPENDS 3rd_party_dbus
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
    ELSE()
        #build d-bus
        ADD_CUSTOM_TARGET(3rd_party_dbus
        COMMAND /bin/bash -c \"grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 1>/dev/null 2>&1\; 
                                 if [ \\$$? == 0 ]\; then 
                                 VAR1=\\$$\(readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 2>/dev/null\)\; 
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\; 
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then 
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/dbus-1.7.8\)\; 
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then 
                                     echo " Need to rebuild D-Bus. " \; 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\; 
                                     make\; 
                                 else 
                                     echo " D-Bus is actual. " \; 
                                 fi\; 
                                 else 
                                 echo " Need to build D-Bus. " \; 
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}\; 
                                 make\; 
                                 fi\"
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
        
        #install d-bus
        #install either to default place with sudo or non-default plase without sudo.
        #to install with sudo to non-default place use manual installation
        ADD_CUSTOM_TARGET(install-3rd_party_dbus
        COMMAND /bin/bash -c \"grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 1>/dev/null 2>&1\; 
                                 if [ \\$$? == 0 ]\; then 
                                 VAR1=\\$$\(readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 2>/dev/null\)\; 
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\; 
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then 
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/dbus-1.7.8\)\; 
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then 
                                     USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\; 
                                     if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     sudo -k \; 
                                     sudo make install\; 
                                     else 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     make install\; 
                                     fi\; 
                                 fi\; 
                                 else 
                                 USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\; 
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     sudo -k \; 
                                     sudo make install\; 
                                 else 
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     make install\; 
                                 fi\; 
                                 fi\"
        DEPENDS 3rd_party_dbus
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
        )
    ENDIF()
    
    SET(install-3rd_party_dbus_var "install-3rd_party_dbus")
    ENDIF()
ENDIF()

ADD_SUBDIRECTORY(${3RD_PARTY_SOURCE_DIRECTORY} ${3RD_PARTY_BINARY_DIRECTORY} EXCLUDE_FROM_ALL)
ADD_CUSTOM_TARGET(install-3rd_party
    DEPENDS ${install-3rd_party_logger_var}
    DEPENDS ${install-3rd_party_dbus_var}
    WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)

IF(ENABLE_LOG)
    INCLUDE_DIRECTORIES ( ${LOG4CXX_INCLUDE_DIRECTORY} )
ENDIF()

IF(ENABLE_SECURITY)
    ADD_DEFINITIONS(-DENABLE_SECURITY)
    SET(SecurityManagerLibrary SecurityManager)
    SET(SecurityManagerIncludeDir ${COMPONENTS_DIR}/security_manager/include)
    #SET(SecurityManagerTestIncludeDir ${CMAKE_SOURCE_DIR}/test/components/security_manager/include)
ENDIF()

IF(ENABLE_HMI_PTU_DECRYPTION)
    MESSAGE("USE DHMI_PTU_PARSER")
    ADD_DEFINITIONS(-DUSE_HMI_PTU_DECRYPTION)
ENDIF()

SET(RTLIB rt)
IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
SET(RTLIB )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
SET(RTLIB )
ELSE()
SET(RTLIB rt)
ENDIF()

# Building tests
IF(BUILD_TESTS)
    ENABLE_TESTING()
    ADD_DEFINITIONS(-DBUILD_TESTS)
    # Framework GoogleTest is also integrated together gmock
    # and must not be added separately
    ADD_SUBDIRECTORY(./src/3rd_party-static/gmock-1.7.0)
ENDIF()

# --- 3rd party libs (static)
ADD_SUBDIRECTORY(./src/3rd_party-static)

# --- Tools
#ADD_SUBDIRECTORY(./tools)


# --- Components
ADD_SUBDIRECTORY(./src/components)

# --- Main application
ADD_SUBDIRECTORY(./src/appMain)

# --- Plugins
ADD_SUBDIRECTORY(./src/plugins)


# Building tests
IF(BUILD_TESTS)
    # Directory test is deprecated. Use src/components/<name>/test
    include(Dart)
    ADD_SUBDIRECTORY(./test)
ENDIF()
                                                                                                                                                                                         
# Building documentation                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL    "Android")
    # At first creating directory for generated documentation. Unfortunately doxygen
    # cannot generate it byself
    FIND_PACKAGE(Doxygen)
    IF(DOXYGEN_FOUND)
        option(DOXYGEN_ENABLE_DIAGRAMS "Enable graphical diagram generation" ON)

        IF(DOXYGEN_ENABLE_DIAGRAMS)
        SET(DOXYGEN_ENABLE_DIAGRAMS_PARAM "YES")
        ELSE(DOXYGEN_ENABLE_DIAGRAMS)
        SET(DOXYGEN_ENABLE_DIAGRAMS_PARAM "NO")
        ENDIF()
        configure_FILE("${PROJECT_SOURCE_DIR}/Doxyfile" "${PROJECT_BINARY_DIR}/Doxyfile")
        FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen")
        ADD_CUSTOM_TARGET(doxygen COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_BINARY_DIR}/Doxyfile")
    ELSE(DOXYGEN_FOUND)
        MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
        MESSAGE(STATUS "To enable documentation generation please install doxygen and graphviz packages")
        MESSAGE(STATUS "sudo apt-get install doxygen graphviz")
        MESSAGE(STATUS "To enable processing of MscGen comments please install mscgen")
        MESSAGE(STATUS "sudo apt-get install mscgen")
    ENDIF(DOXYGEN_FOUND)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    SET(CMAKE_BUILD_TYPE OFF)
    MESSAGE(STATUS "set CMAKE_BUILD_TYPE OFF")
ENDIF()
