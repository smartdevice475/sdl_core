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

SET(target TransportManager)
SET(TM_SRC_DIR ${COMPONENTS_DIR}/transport_manager/src)

INCLUDE_DIRECTORIES(
  include
  ${COMPONENTS_DIR}/include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/resumption/include
  ${COMPONENTS_DIR}/transport_manager
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LIBUSB_INCLUDE_DIRECTORY}
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

IF(BUILD_BT_SUPPORT)
  INCLUDE_DIRECTORIES(
  )
ENDIF()

SET(SOURCES
  ${TM_SRC_DIR}/transport_manager_impl.cc
  ${TM_SRC_DIR}/transport_manager_default.cc
  ${TM_SRC_DIR}/transport_adapter/transport_adapter_listener_impl.cc
  ${TM_SRC_DIR}/transport_adapter/transport_adapter_impl.cc
  ${TM_SRC_DIR}/tcp/tcp_transport_adapter.cc
  ${TM_SRC_DIR}/transport_adapter/threaded_socket_connection.cc
  ${TM_SRC_DIR}/tcp/tcp_client_listener.cc
  ${TM_SRC_DIR}/tcp/tcp_device.cc
  ${TM_SRC_DIR}/tcp/tcp_socket_connection.cc
  ${TM_SRC_DIR}/tcp/tcp_connection_factory.cc
)

IF(BUILD_AVAHI_SUPPORT)
  list (APPEND SOURCES
    ${TM_SRC_DIR}/tcp/dnssd_service_browser.cc
  )
ENDIF()
IF(BUILD_BT_SUPPORT)
  list (APPEND SOURCES
  ${TM_SRC_DIR}/bluetooth/bluetooth_device_scanner.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_transport_adapter.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_connection_factory.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_socket_connection.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_device.cc
  )
ENDIF()

IF(BUILD_USB_SUPPORT)
  list (APPEND SOURCES
    ${TM_SRC_DIR}/usb/usb_aoa_adapter.cc
    ${TM_SRC_DIR}/usb/usb_connection_factory.cc
    ${TM_SRC_DIR}/usb/usb_device_scanner.cc
  )
  IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE" OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list (APPEND SOURCES
    ${TM_SRC_DIR}/usb/libusb/usb_handler.cc
    ${TM_SRC_DIR}/usb/libusb/usb_connection.cc
    ${TM_SRC_DIR}/usb/libusb/platform_usb_device.cc
    )
  ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    LIST(APPEND SOURCES
    ${TM_SRC_DIR}/usb/qnx/usb_handler.cc
    ${TM_SRC_DIR}/usb/qnx/usb_connection.cc
    ${TM_SRC_DIR}/usb/qnx/platform_usb_device.cc
    )
  ENDIF()
ENDIF(BUILD_USB_SUPPORT)

IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
ENDIF()
ADD_LIBRARY(${target} ${SOURCES})
TARGET_LINK_LIBRARIES(${target} ProtocolLibrary)

IF(BUILD_USB_SUPPORT)
  IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    TARGET_LINK_LIBRARIES(${target} usbdi)
  ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows"  OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    TARGET_LINK_LIBRARIES(${target} Libusb-1.0.16)
  ENDIF()
ENDIF(BUILD_USB_SUPPORT)


IF(BUILD_AVAHI_SUPPORT)
  TARGET_LINK_LIBRARIES(${target} avahi-common)
ENDIF()

IF(ENABLE_LOG)
IF(MODIFY_FUNCTION_FLAGS)
ELSE()
  TARGET_LINK_LIBRARIES(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
ENDIF()
ENDIF()

IF(BUILD_BT_SUPPORT)
  TARGET_LINK_LIBRARIES(${target} bluetooth)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
ENDIF()

IF(BUILD_TESTS)
  ADD_SUBDIRECTORY(test)
ENDIF()
