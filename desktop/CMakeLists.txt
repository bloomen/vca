set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(vca_desktop
    src/desktop.cpp
    src/engine/DaemonHandler.h
    src/engine/DaemonHandler.cpp
    src/engine/SearchHandler.h
    src/engine/SearchHandler.cpp
    src/engine/StatusHandler.h
    src/engine/StatusHandler.cpp
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

target_include_directories(vca_desktop
    PRIVATE ${PROJECT_SOURCE_DIR}/core
    PRIVATE src
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/restclient-cpp/include
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/served/src
)

target_link_libraries(vca_desktop
    Qt6::Qml
    Qt6::Quick
    restclient-cpp
    served
    vca_core
)

if(MSVC)

add_custom_command(TARGET vca_desktop POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CONAN_QT_ROOT}/bin"
    $<TARGET_FILE_DIR:vca_desktop>)

add_custom_command(TARGET vca_desktop POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CONAN_QT_ROOT}/res"
    $<TARGET_FILE_DIR:vca_desktop>/../res)

add_custom_command(TARGET vca_desktop POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CONAN_GLIB_ROOT}/bin"
    $<TARGET_FILE_DIR:vca_desktop>)

endif()
