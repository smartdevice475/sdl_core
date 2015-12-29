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


INCLUDE_DIRECTORIES(
    include/
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/smart_objects/include/
    ${COMPONENTS_DIR}/formatters/include/
    ${COMPONENTS_DIR}/config_profile/include/
    ${COMPONENTS_DIR}/media_manager/include/
    ${COMPONENTS_DIR}/protocol_handler/include
    ${JSONCPP_INCLUDE_DIRECTORY}
    ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}
    ${CMAKE_BINARY_DIR}/src/components/
    ${COMPONENTS_DIR}/dbus/include/
    ${CMAKE_SOURCE_DIR}/
    ${LOG4CXX_INCLUDE_DIRECTORY}
)

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	INCLUDE_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/include/win32
	)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    INCLUDE_DIRECTORIES(
      ${CMAKE_SOURCE_DIR}/src/3rd_party-static/pthread-win32-2.9.1
    )
ENDIF()

IF(HMIADAPTER STREQUAL "dbus")
    SET(DBUS_SOURCE ./src/dbus_message_adapter.cc)
    SET(DBUS_ADAPTER DBus)
ENDIF()

IF(MODIFY_FUNCTION_FLAGS)
SET(SOURCES
    ${COMPONENTS_DIR}/hmi_message_handler/src/hmi_message_handler_impl.cc
    ${COMPONENTS_DIR}/hmi_message_handler/src/messagebroker_adapter.cc
    ${COMPONENTS_DIR}/hmi_message_handler/src/hmi_message_adapter.cc
    ${DBUS_SOURCE}
)
ELSE()
SET(SOURCES
    ${COMPONENTS_DIR}/hmi_message_handler/src/hmi_message_handler_impl.cc
    ${COMPONENTS_DIR}/hmi_message_handler/src/messagebroker_adapter.cc
    ${COMPONENTS_DIR}/hmi_message_handler/src/hmi_message_adapter.cc
	${COMPONENTS_DIR}/hmi_message_handler/src/mqueue_adapter.cc
    ${DBUS_SOURCE}
)
ENDIF()

SET(LIBRARIES
    Utils
    ${DBUS_ADAPTER}
    ${RTLIB}
)

ADD_LIBRARY("HMIMessageHandler" ${SOURCES})
TARGET_LINK_LIBRARIES("HMIMessageHandler" ${LIBRARIES})

IF(ENABLE_LOG)
    TARGET_LINK_LIBRARIES("HMIMessageHandler" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
ENDIF()

# Tests temporary are inactivated. For details please check
# Readme.txt in test directory of hmi_message_handler 

#IF(BUILD_TESTS)
#  ADD_SUBDIRECTORY(test)
#ENDIF()
