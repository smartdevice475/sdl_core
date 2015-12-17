SET(JSONCPP_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(JSONCPP_INCLUDE_DIRECTORY ${JSONCPP_INCLUDE_DIRECTORY} PARENT_SCOPE)

INCLUDE_DIRECTORIES(
    ./include/
)

SET(SOURCES
    ./src/lib_json/json_value.cpp
    ./src/lib_json/json_writer.cpp
    ./src/lib_json/json_reader.cpp
)

ADD_LIBRARY("jsoncpp" ${SOURCES})
