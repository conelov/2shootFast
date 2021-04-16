cmake_minimum_required(VERSION 3.19)
project(2shoot)
set(CMAKE_CXX_STANDARD 20)
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
set(${PROJECT_NAME}_includeDirs ${PROJECT_SOURCE_DIR})
if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
add_subdirectory("$ENV{CLP_DIR}/ch" CHBuild)

add_library(${PROJECT_NAME}_utils STATIC
        utils/Paint.hpp utils/Paint.cpp
        utils/Define.hpp
        utils/InputManager.hpp utils/InputManager.cpp utils/SceneItem.cpp utils/SceneItem.hpp)
target_link_libraries(${PROJECT_NAME}_utils PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        )
target_include_directories(${PROJECT_NAME}_utils PRIVATE ${${PROJECT_NAME}_includeDirs})

add_library(${PROJECT_NAME}_moc STATIC
        moc/Scene.hpp moc/Scene.cpp)
set_target_properties(${PROJECT_NAME}_moc PROPERTIES
        AUTOMOC ON
        INCLUDE_CURRENT_DIR ON
        )
target_link_libraries(${PROJECT_NAME}_moc PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        ch
#        PRIVATE
#        ${PROJECT_NAME}_utils
        )
target_include_directories(${PROJECT_NAME}_moc PRIVATE ${${PROJECT_NAME}_includeDirs})

add_library(${PROJECT_NAME}_ui STATIC
        ui/FormMain.hpp ui/FormMain.cpp ui/FormMain.ui
        ui/FormDraw.cpp ui/FormDraw.hpp)
set_target_properties(${PROJECT_NAME}_ui PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        INCLUDE_CURRENT_DIR ON
        )
target_link_libraries(${PROJECT_NAME}_ui PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        PRIVATE
        ${PROJECT_NAME}_utils
        ${PROJECT_NAME}_moc
        )
target_include_directories(${PROJECT_NAME}_ui PRIVATE ${${PROJECT_NAME}_includeDirs} ui)

add_executable(${PROJECT_NAME}_main
        main.cpp)
set_target_properties(${PROJECT_NAME}_main PROPERTIES
        AUTORCC ON
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
        )
target_link_libraries(${PROJECT_NAME}_main
        ${PROJECT_NAME}_utils
        ${PROJECT_NAME}_ui
        )
target_include_directories(${PROJECT_NAME}_main PRIVATE ${${PROJECT_NAME}_includeDirs})