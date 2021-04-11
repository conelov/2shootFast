cmake_minimum_required(VERSION 3.19)
project(2shoot)
set(CMAKE_CXX_STANDARD 20)
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
set(CMAKE_DEBUG_POSTFIX _debug)
set(${PROJECT_NAME}_includeDirs ui utils qObjs)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
#add_subdirectory("$ENV{CLP_DIR}/ch" CHBuild)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

add_library(${PROJECT_NAME}_qObjs STATIC
        qObjs/Scene.hpp qObjs/Scene.cpp qObjs/Player.cpp qObjs/Player.hpp qObjs/FigureSelectorAdapter.hpp qObjs/FigureSelectorAdapter.cpp)
set_target_properties(${PROJECT_NAME}_qObjs PROPERTIES
        AUTOMOC ON
        INCLUDE_CURRENT_DIR ON
        )
target_link_libraries(${PROJECT_NAME}_qObjs PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        #        ch
        )
target_include_directories(${PROJECT_NAME}_qObjs PRIVATE ${${PROJECT_NAME}_includeDirs})

add_library(${PROJECT_NAME}_utils STATIC
        utils/UserInputBase.hpp utils/UserInputCreator.cpp
        utils/UserInputCreator.hpp utils/UserInputBase.cpp utils/Serialize.cpp utils/Serialize.hpp)
target_link_libraries(${PROJECT_NAME}_utils PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        )
target_include_directories(${PROJECT_NAME}_utils PRIVATE ${${PROJECT_NAME}_includeDirs})

add_library(${PROJECT_NAME}_ui STATIC
        ui/FormMain.hpp ui/FormMain.hpp ui/FormMain.cpp
        ui/FiguresPanel.cpp ui/FiguresPanel.hpp ui/FormDraw.cpp ui/FormDraw.hpp)
set_target_properties(${PROJECT_NAME}_ui PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        INCLUDE_CURRENT_DIR ON
        )
target_link_libraries(${PROJECT_NAME}_ui PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        )
target_include_directories(${PROJECT_NAME}_ui PRIVATE ${${PROJECT_NAME}_includeDirs})

add_executable(${PROJECT_NAME}_main
        main.cpp
        resources.qrc
        )
set_target_properties(${PROJECT_NAME}_main PROPERTIES
        AUTORCC ON
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
        )
target_link_libraries(${PROJECT_NAME}_main
        ${PROJECT_NAME}_ui
        ${PROJECT_NAME}_qObjs
        ${PROJECT_NAME}_utils
        )
target_include_directories(${PROJECT_NAME}_main PRIVATE ${${PROJECT_NAME}_includeDirs})