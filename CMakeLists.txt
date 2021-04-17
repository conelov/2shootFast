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

add_library(${PROJECT_NAME}_impl STATIC
        utils/SceneItem.hpp utils/SceneItem.cpp
        utils/InputHandler.hpp utils/InputHandler.cpp
        utils/PrintMethods.hpp utils/PrintMethods.cpp
        utils/Define.hpp
        ui/FormMain.hpp ui/FormMain.cpp ui/FormMain.ui
        ui/FormDraw.hpp ui/FormDraw.cpp ui/FormDraw.ui
        moc/Scene.hpp moc/Scene.cpp)
target_link_libraries(${PROJECT_NAME}_impl PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        )
set_target_properties(${PROJECT_NAME}_impl PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        INCLUDE_CURRENT_DIR ON
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
        )
target_include_directories(${PROJECT_NAME}_impl PRIVATE ${${PROJECT_NAME}_includeDirs} ui)

add_executable(${PROJECT_NAME}_main
        main.cpp)
set_target_properties(${PROJECT_NAME}_main PROPERTIES
        AUTORCC ON
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
        )
target_link_libraries(${PROJECT_NAME}_main
        ${PROJECT_NAME}_impl
        )
target_include_directories(${PROJECT_NAME}_main PRIVATE ${${PROJECT_NAME}_includeDirs})