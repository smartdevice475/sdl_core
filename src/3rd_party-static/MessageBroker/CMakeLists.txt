SET(MESSAGE_BROKER_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(MESSAGE_BROKER_INCLUDE_DIRECTORY ${MESSAGE_BROKER_INCLUDE_DIRECTORY} PARENT_SCOPE)

INCLUDE_DIRECTORIES(
    ./include/
    ../jsoncpp/include
    ../../components/utils/include
)

IF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    INCLUDE_DIRECTORIES(
        ../pthread-win32-2.9.1
    )
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/include/win32
    )
ENDIF()

SET(MESSAGE_BROKER_SOURCES
    ./src/lib_messagebroker/CMessageBroker.cpp
    ./src/lib_messagebroker/CMessageBrokerRegistry.cpp
    ./src/lib_messagebroker/md5.cpp
    ./src/lib_messagebroker/system.cpp
    ./src/lib_messagebroker/websocket_handler.cpp
)

SET(MESSAGE_BROKER_CLIENT_SOURCES
    ./src/client/mb_client.cpp
    ./src/client/mb_tcpclient.cpp
    ./src/client/mb_controller.cpp
)

SET(MESSAGE_BROKER_SERVER_SOURCES
    ./src/server/mb_server.cpp
    ./src/server/mb_tcpserver.cpp
    ./src/server/networking.cpp
)

ADD_LIBRARY("MessageBroker" ${MESSAGE_BROKER_SOURCES})
TARGET_LINK_LIBRARIES(MessageBroker Utils)


ADD_LIBRARY("MessageBrokerClient" ${MESSAGE_BROKER_CLIENT_SOURCES})
TARGET_LINK_LIBRARIES("MessageBrokerClient" "MessageBroker")

ADD_LIBRARY("MessageBrokerServer" ${MESSAGE_BROKER_SERVER_SOURCES})
TARGET_LINK_LIBRARIES("MessageBrokerServer" "MessageBroker")
