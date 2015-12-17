SET(LIBUSB_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
SET(LIBUSB_INCLUDE_DIRECTORY ${LIBUSB_INCLUDE_DIRECTORY} PARENT_SCOPE)

SET(LIBUSB_LIBS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
SET(LIBUSB_LIBS_DIRECTORY ${LIBUSB_LIBS_DIRECTORY} PARENT_SCOPE)

INCLUDE_DIRECTORIES(
    ./libusb/os/
    ./libusb/
    ./
)

IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    SET(LIBUSB_SOURCES
        ./libusb/core.c
        ./libusb/descriptor.c
        ./libusb/hotplug.c
        ./libusb/io.c
        ./libusb/strerror.c
        ./libusb/sync.c
        ./libusb/os/linux_netlink.c
        ./libusb/os/linux_usbfs.c
        ./libusb/os/poll_posix.c
        ./libusb/os/threads_posix.c
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
    SET(LIBUSB_SOURCES
        ./libusb/missing.c
        ./libusb/core.c
        ./libusb/descriptor.c
        ./libusb/hotplug.c
        ./libusb/io.c
        ./libusb/strerror.c
        ./libusb/sync.c
        ./libusb/os/wince_usb.c
        ./libusb/os/poll_windows.c
        ./libusb/os/threads_windows.c
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(LIBUSB_SOURCES
        ./libusb/core.c
        ./libusb/descriptor.c
        ./libusb/hotplug.c
        ./libusb/io.c
        ./libusb/strerror.c
        ./libusb/sync.c
        ./libusb/os/windows_usb.c
        ./libusb/os/poll_windows.c
        ./libusb/os/threads_windows.c
    )
ELSE()
    SET(LIBUSB_SOURCES
        ./libusb/core.c
        ./libusb/descriptor.c
        ./libusb/hotplug.c
        ./libusb/io.c
        ./libusb/strerror.c
        ./libusb/sync.c
        ./libusb/os/linux_udev.c
        ./libusb/os/linux_netlink.c
        ./libusb/os/linux_usbfs.c
        ./libusb/os/poll_posix.c
        ./libusb/os/threads_posix.c
    )
ENDIF()

ADD_LIBRARY("Libusb-1.0.16" ${LIBUSB_SOURCES})


IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "QNX" OR CMAKE_SYSTEM_NAME STREQUAL  "Darwin")
    TARGET_LINK_LIBRARIES("Libusb-1.0.16" udev)
ENDIF()