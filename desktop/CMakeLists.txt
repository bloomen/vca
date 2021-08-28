set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Qml REQUIRED Quick REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
    src
)

add_executable(vca_desktop
    src/desktop.cpp
    src/engine/SearchHandler.h
    src/engine/SearchHandler.cpp
    src/qtc/Action.h
    src/qtc/CommandQueue.h
    src/qtc/CommandQueue.cpp
    src/qtc/Handler.h
    src/qtc/Handler.cpp
    src/qtc/Input.h
    src/qtc/Input.cpp
    src/qtc/Model.h
    src/qtc/Model.cpp
    src/qtc/Node.h
    src/qtc/NodeBase.h
    src/qtc/NodeBase.cpp
    src/qtc/Output.h
    src/qtc/Output.cpp
    src/qtc/ThreadPool.h
    src/qtc/ThreadPool.cpp
    src/qtc/UpdateType.h
    src/resource.qrc
    src/view/View.h
    src/view/View.cpp
)

target_link_libraries(vca_desktop
    vca_core
    restclient-cpp
    Qt5::Qml
    Qt5::Quick
)
