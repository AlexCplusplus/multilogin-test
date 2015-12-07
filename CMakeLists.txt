cmake_minimum_required (VERSION 2.8)

set (APP multilogin)
project (${APP})

message ("Project: ${PROJECT_NAME}")

find_package (FLTK REQUIRED)
find_package (X11 REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set (APP_SRC application.cpp
             callbacks.cpp
             logic.cpp
             main.cpp
             page_login.cpp
             page_serv.cpp)

set (APP_HDR application.h
             callbacks.h
             logic.h
             page_login.h
             page_serv.h)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x -s")
endif ()

add_executable (${APP} ${APP_SRC} ${APP_HDR})
target_link_libraries (${APP} ${FLTK_LIBRARIES})
target_link_libraries (${APP} ${X11_Xcursor_LIB})
target_link_libraries (${APP} ${X11_Xft_LIB})
target_link_libraries (${APP} ${X11_Xinerama_LIB})
