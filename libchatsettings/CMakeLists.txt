cmake_minimum_required(VERSION 3.12)
project(libchatsettings)

set(BINDIR ${PROJECT_SOURCE_DIR}/../bin)
set(LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINDIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINDIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINDIR})

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBDIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBDIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBDIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${PROJECT_SOURCE_DIR}/sources)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

set(LIBCHATDBCHAT_LIBRARIES)
set(Qt5Modules Core Widgets Sql)
foreach(QtModule IN LISTS Qt5Modules)
    list(APPEND LIBCHATDB_LIBRARIES Qt5::${QtModule})
endforeach(QtModule)

find_package(Qt5 COMPONENTS ${Qt5Modules} REQUIRED LinguistTools)

file(GLOB_RECURSE TS_SOURCES "*.cpp" "*.h" "*.ui")

file(GLOB_RECURSE SRC_FILES sources/*.cpp)
file(GLOB_RECURSE HEADERS_FILES sources/*.h)
file(GLOB_RECURSE RC_FILES sources/*.qrc)
file(GLOB_RECURSE UI_FILES sources/*.ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt5_wrap_ui(QUI ${UI_FILES})
qt5_add_resources(QRC ${RC_FILES})

add_library(${PROJECT_NAME}
    ${SRC_FILES} ${HEADERS_FILES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "chatsettings")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "lib")

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${LIBCHATDB_LIBRARIES}
)

