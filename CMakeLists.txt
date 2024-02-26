cmake_minimum_required(VERSION 3.12)

project(Chat)

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

set(CHAT_LIBRARIES)
set(Qt5Modules Core Widgets)
foreach(QtModule IN LISTS Qt5Modules)
    list(APPEND CHAT_LIBRARIES Qt5::${QtModule})
endforeach(QtModule)

find_package(Qt5 COMPONENTS ${Qt5Modules} REQUIRED LinguistTools)

set(TS_FILES
    ${PROJECT_SOURCE_DIR}/translations/my_ru.ts
)
file(GLOB_RECURSE TS_SOURCES "*.cpp" "*.h" "*.ui")
add_custom_target(translations)
foreach(_ts_file ${TS_FILES})
    get_filename_component(_ts_name ${_ts_file} NAME_WE)
    add_custom_command(OUTPUT "${_ts_file}"
        DEPENDS "${TS_SOURCES}"
        COMMAND ${Qt5_LUPDATE_EXECUTABLE} -recursive ${PROJECT_SOURCE_DIR} -ts ${_ts_file}
    )
    add_custom_target(translate_${ts_name} DEPENDS ${_ts_file})
    add_dependencies(translations translate_${ts_name})
endforeach()

file(GLOB_RECURSE SRC_FILES sources/*.cpp)
file(GLOB_RECURSE HEADERS_FILES sources/*.h)
file(GLOB_RECURSE RC_FILES sources/*.qrc)
file(GLOB_RECURSE UI_FILES sources/*.ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt5_add_translation(QM_FILES ${TS_FILES})
qt5_wrap_ui(QUI ${UI_FILES})
qt5_add_resources(QRC ${RC_FILES})

add_executable(Chat
    ${SRC_FILES} ${HEADERS_FILES} ${RC_FILES} ${UI_FILES} ${QM_FILES}
    ${PROJECT_SOURCE_DIR}/mainwindow.h
    ${PROJECT_SOURCE_DIR}/mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/mainwindow.ui
    ${PROJECT_SOURCE_DIR}/startscreen.h ${PROJECT_SOURCE_DIR}/startscreen.cpp ${PROJECT_SOURCE_DIR}/startscreen.ui
    ${PROJECT_SOURCE_DIR}/loginform.h ${PROJECT_SOURCE_DIR}/loginform.cpp ${PROJECT_SOURCE_DIR}/loginform.ui
    ${PROJECT_SOURCE_DIR}/registrationform.h ${PROJECT_SOURCE_DIR}/registrationform.cpp ${PROJECT_SOURCE_DIR}/registrationform.ui)
target_link_libraries(Chat
    PUBLIC
    ${CHAT_LIBRARIES}
)

