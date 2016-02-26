# Copyright (c) 2013, Ford Motor Company
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

SET(target Policy)
SET(install_destination bin)
SET(copy_destination ${CMAKE_BINARY_DIR}/src/appMain)
SET(library_name ${CMAKE_SHARED_LIBRARY_PREFIX}${target}${CMAKE_SHARED_LIBRARY_SUFFIX})

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

IF(EXTENDED_POLICY_FLAG)
  ADD_DEFINITIONS(-DEXTENDED_POLICY)
ENDIF()

#Generation of policy table interface...
#include(${CMAKE_SOURCE_DIR}/tools/intergen/GenerateInterfaceLibrary.cmake)

INCLUDE_DIRECTORIES(
  ./include
  ./usage_statistics/include
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  #${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/utils
  ${CMAKE_SOURCE_DIR}/src/components
  ${CMAKE_SOURCE_DIR}/src/components/config_profile/include
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	INCLUDE_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/include/win32
	)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    INCLUDE_DIRECTORIES(
      ${CMAKE_SOURCE_DIR}/include/wince
      ${CMAKE_SOURCE_DIR}/src/3rd_party-static/pthread-win32-2.9.1
    )
ENDIF()

SET(SOURCES
  ./src/policy_manager_impl.cc
  ./src/policy_helper.cc
  ./src/policy_table.cc
  ./src/sql_pt_queries.cc
  ./src/sql_pt_representation.cc
  ./src/update_status_manager.cc
  ./src/cache_manager.cc
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/src/rpc_base/rpc_base.cc
)


ADD_SUBDIRECTORY(usage_statistics)

INCLUDE_DIRECTORIES(./policy_table/table_struct)
ADD_SUBDIRECTORY(policy_table/table_struct)

SET(LIBRARIES ConfigProfile policy_struct dbms jsoncpp Utils)

IF(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- QDB Wrapper 
  INCLUDE_DIRECTORIES(${COMPONENTS_DIR}/utils/include/utils)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # --- SQLite Wrapper
  INCLUDE_DIRECTORIES(${COMPONENTS_DIR}/utils/include/utils)
  LIST(APPEND LIBRARIES sqlite3)
  LIST(APPEND LIBRARIES pthreadVC2)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
  INCLUDE_DIRECTORIES(${COMPONENTS_DIR}/utils/include/utils/sqlite_wrapper)
  LIST(APPEND LIBRARIES sqlite3)
  LIST(APPEND LIBRARIES pthreadVC2)
  LIST(APPEND LIBRARIES Utils)
  LIST(APPEND LIBRARIES ws2)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Android")
  INCLUDE_DIRECTORIES(sqlite_wrapper/include)
ELSE()
  # --- SQLite Wrapper
  INCLUDE_DIRECTORIES(${COMPONENTS_DIR}/utils/include/utils)
  LIST(APPEND LIBRARIES sqlite3)
ENDIF()

IF(MODIFY_FUNCTION_FLAGS)
IF(ENABLE_LOG)
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "Android" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
  LIST(APPEND LIBRARIES log4cxx)
ENDIF()
ENDIF()
ENDIF()

ADD_LIBRARY(${target} SHARED ${SOURCES})

TARGET_LINK_LIBRARIES(${target} ${LIBRARIES} )

IF(ENABLE_LOG)
  ADD_DEPENDENCIES(${target} liblog4cxx)
  TARGET_LINK_LIBRARIES(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
ADD_CUSTOM_TARGET(copy_library_${target} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${library_name}
    ${copy_destination}
    DEPENDS ${target}
    COMMENT "Copying library ${library_name}")
ELSE()
ADD_CUSTOM_TARGET(copy_library_${target} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/${library_name}
    ${copy_destination}
    DEPENDS ${target}
    COMMENT "Copying library ${library_name}")
ENDIF()

INSTALL(TARGETS ${target}
  DESTINATION ${install_destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
