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

SET(COMPONENTS_DIR ${CMAKE_SOURCE_DIR}/src/components)

# --- HMI_API interfaces
ADD_SUBDIRECTORY(./interfaces)

# --- ProtocolHandler
ADD_SUBDIRECTORY(./protocol)

# --- TransportManager
ADD_SUBDIRECTORY(./transport_manager)

# --- Policies
ADD_SUBDIRECTORY(./resumption)

# --- formatters
ADD_SUBDIRECTORY(./formatters)

# --- ProtocolHandler
ADD_SUBDIRECTORY(./protocol_handler)

# --- ConnectionHandler
ADD_SUBDIRECTORY(./connection_handler)

# --- Utils
ADD_SUBDIRECTORY(./utils)

# --- Security manager
IF(ENABLE_SECURITY)
  ADD_SUBDIRECTORY(./security_manager)
ENDIF()
#
# --- Policy_impl
ADD_SUBDIRECTORY(./policy)

# --- Validated types
ADD_SUBDIRECTORY(./rpc_base)

# --- SmartObjects
ADD_SUBDIRECTORY(./smart_objects)

# --- Application manager
ADD_SUBDIRECTORY(./application_manager)

# --- HMI Message Handler
ADD_SUBDIRECTORY(./hmi_message_handler)

# --- Config
ADD_SUBDIRECTORY(./config_profile)

# --- AudioManager
ADD_SUBDIRECTORY(./media_manager)

IF(TIME_TESTER)
    ADD_SUBDIRECTORY(./time_tester)
ENDIF()


IF(${HMI_DBUS_API})
# --- DBus
    ADD_SUBDIRECTORY(./dbus)
ENDIF()
IF (${QT_HMI})
# --- Qt HMI
    ADD_SUBDIRECTORY(./qt_hmi)
ENDIF()

