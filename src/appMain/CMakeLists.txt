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

IF(HMIADAPTER STREQUAL "messagebroker")
    SET(BROKER_LIBRARIES
        MessageBrokerClient
        MessageBrokerServer
    )
ENDIF()

CMAKE_POLICY(PUSH)
# make link_directories() treat paths relative to the source dir
# info: cmake --help-policy CMP0015
CMAKE_POLICY(SET CMP0015 NEW)
LINK_DIRECTORIES(${LIBUSB_LIBS_DIRECTORY})
CMAKE_POLICY(POP)

IF(EXTENDED_MEDIA_MODE)
    SET(default_media_inc
    ${GSTREAMER_gst_INCLUDE_DIR}
    )
ELSE(EXTENDED_MEDIA_MODE)
    SET(default_media_inc
    )
ENDIF()

IF(TIME_TESTER)
    SET(TIME_TESTER_LIB
        TimeTester)
ENDIF()

SET(LIBRARIES
    MediaManager
    ApplicationManager
    HMI_API
    MOBILE_API
    v4_protocol_v1_2_no_extra
    SmartObjects
    ${TIME_TESTER_LIB}
    formatters
    ProtocolLibrary
    ProtocolHandler
    connectionHandler
    TransportManager
    ${SecurityManagerLibrary}
    HMIMessageHandler
    MessageBroker
    ${BROKER_LIBRARIES}
    Utils
    jsoncpp
    ConfigProfile
    Resumption
)

IF(MODIFY_FUNCTION_FLAGS)
    #LIST(APPEND LIBRARIES    lib_msp_vr)
ENDIF()
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    LIST(APPEND LIBRARIES pthreadVC2 ws2_32)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    LIST(APPEND LIBRARIES pthreadVC2 ws2)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    #LIST(APPEND LIBRARIES pthread)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    LIST(APPEND LIBRARIES pthread)
    LIST(APPEND LIBRARIES dl)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    LIST(APPEND LIBRARIES apr-1 aprutil-1)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    IF(ENABLE_LOG)
        LIST(APPEND LIBRARIES dl apr-1 aprutil-1)
    ELSE()
        LIST(APPEND LIBRARIES dl)
    ENDIF()
ELSE()
    LIST(APPEND LIBRARIES pthread)
ENDIF()

IF(MODIFY_FUNCTION_FLAGS)
    IF(ENABLE_LOG)
        IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL "Windows"    OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
            LIST(APPEND LIBRARIES log4cxx)
        ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
            LIST(APPEND LIBRARIES log4cxx apr-1 aprutil-1 expat winsock)
        ENDIF()
    ENDIF()
ENDIF()

IF(BUILD_BT_SUPPORT)
    LIST(APPEND LIBRARIES bluetooth)
ENDIF()
IF(BUILD_AVAHI_SUPPORT)
    LIST(APPEND LIBRARIES avahi-client avahi-common)
ENDIF()
IF(BUILD_USB_SUPPORT)
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        LIST(APPEND LIBRARIES Libusb-1.0.16)
    ENDIF()
ENDIF()

IF(ENABLE_LOG)
    IF(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        LIST(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
        LIST(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
        LIST(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
        LIST(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
    ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(
    ./
    ${COMPONENTS_DIR}/protocol_handler/include/
    ${JSONCPP_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}/application_manager/include
    ${COMPONENTS_DIR}/formatters/include
    ${COMPONENTS_DIR}/transport_manager/include
    ${COMPONENTS_DIR}/security_manager/include
    ${SecurityManagerIncludeDir}
    ${COMPONENTS_DIR}/security_manager/include
    ${COMPONENTS_DIR}/config_profile/include
    ${COMPONENTS_DIR}/utils/include
    ${COMPONENTS_DIR}/connection_handler/include/
    ${COMPONENTS_DIR}/hmi_message_handler/include
    ${COMPONENTS_DIR}/request_watchdog/include
    ${COMPONENTS_DIR}/smart_objects/include/
    ${COMPONENTS_DIR}/media_manager/include/
    ${COMPONENTS_DIR}/time_tester/include
    ${COMPONENTS_DIR}/policy/src/policy/include/
    ${COMPONENTS_DIR}/policy/src/policy/usage_statistics/include/
    ${COMPONENTS_DIR}/resumption/include/
    ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
    ${ENCRYPTION_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}
    ${CMAKE_BINARY_DIR}/src/components/
    ${COMPONENTS_DIR}/dbus/include/
    ${CMAKE_BINARY_DIR}/src/components/policy/src/policy
    ${CMAKE_SOURCE_DIR}
    ${default_media_inc}
    ${LOG4CXX_INCLUDE_DIRECTORY}
    ${CMAKE_SOURCE_DIR}/src/3rd_party/dbus-1.7.8
    ${CMAKE_SOURCE_DIR}/src/3rd_party/dbus-1.7.8/dbus/
    ${CMAKE_SOURCE_DIR}/src/LibVrMsp
)

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    INCLUDE_DIRECTORIES(
      ${CMAKE_SOURCE_DIR}/src/3rd_party-static/pthread-win32-2.9.1
      ${CMAKE_SOURCE_DIR}/lib/win32
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    INCLUDE_DIRECTORIES(
      ${CMAKE_SOURCE_DIR}/src/3rd_party-static/pthread-win32-2.9.1
      ${CMAKE_SOURCE_DIR}/lib/wince
    )
ENDIF()

SET(SOURCES
    main.cc
    life_cycle.cc
    signal_handlers.cc
)

IF( NOT CMAKE_BUILD_TYPE )
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
ENDIF()

IF(BUILD_TARGET_LIBRARY)
    ADD_LIBRARY(${PROJECT} SHARED ${HEADERS} ${SOURCES})
ELSE()
    ADD_EXECUTABLE(${PROJECT} ${SOURCES})
ENDIF()
TARGET_LINK_LIBRARIES(${PROJECT} ${LIBRARIES})

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    ELSE()
        SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
    ENDIF()
ENDIF()

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/audio.8bit.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/smartDeviceLink.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/init_policy.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    FILE(COPY ${CMAKE_SOURCE_DIR}/lib/wince/msvcr90.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Android")
ENDIF()

# Replace commit in ini file
SET(GITCOMMIT "")
IF(EXISTS ${CMAKE_SOURCE_DIR}/.git)
    FIND_PACKAGE(Git)
    IF(GIT_FOUND)
        EXECUTE_PROCESS(
            COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE GITCOMMIT ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        SET(INI_FILE ${CMAKE_CURRENT_BINARY_DIR}/smartDeviceLink.ini)
        IF(EXISTS ${INI_FILE})
            FILE(READ ${INI_FILE} FILE_CONTENT)
            SET(LINE SDLVersion)
            SET(SEARCH_REGEX "${LINE}([^/\r/\n]+)")
            string(REGEX REPLACE "${SEARCH_REGEX}" "${LINE} = ${GITCOMMIT}" 
                         MODIFIED_FILE_CONTENT "${FILE_CONTENT}")
            FILE(WRITE "${INI_FILE}" "${MODIFIED_FILE_CONTENT}")
        ENDIF()
    ENDIF(GIT_FOUND)
ENDIF()

IF(${QT_HMI})
    SET(main_qml "hmi/MainWindow.qml")
    SET(plugins_dir "hmi/plugins")

    IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
        SET(start_command "./start-qml.sh")
        SET(command_arguments "${main_qml} ${plugins_dir}")
    ELSE()
        EXECUTE_PROCESS(
            COMMAND ${CMAKE_SOURCE_DIR}/FindQt.sh -v ${qt_version} -b qmlscene
            OUTPUT_VARIABLE start_command
        )
        SET(command_arguments "${main_qml} -I ${plugins_dir}")
    ENDIF()

    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/start_hmi.sh
        "#!/bin/sh\n${start_command} ${command_arguments}\n")
ELSEIF(${WEB_HMI})
    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hmi_link
        "${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html")
ENDIF()

# Install rules
INSTALL(TARGETS ${PROJECT} DESTINATION bin)
INSTALL(
    FILES log4cxx.properties audio.8bit.wav test.txt smartDeviceLink.ini
        hmi_capabilities.json sdl_preloaded_pt.json
        ${CMAKE_SOURCE_DIR}/mycert.pem ${CMAKE_SOURCE_DIR}/mykey.pem
    DESTINATION bin
)

IF(${WEB_HMI})
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        INSTALL( DIRECTORY ${CMAKE_HOME_DIRECTORY}/src/components/HMI/ DESTINATION bin/HMI)
    ENDIF()
ELSEIF(${QT_HMI})
    IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
        INSTALL(FILES start-qml.sh DESTINATION bin
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
                GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    ENDIF()
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/start_hmi.sh DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
            GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()
IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    INSTALL(FILES init_policy.sh DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
            GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()
