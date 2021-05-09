set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Qml REQUIRED Quick REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

add_executable(vca_desktop
    src/AppInput.qml
    src/AppOutput.qml
    src/Handler.h
    src/Handler.cpp
    src/HandlerFactory.h
    src/HandlerFactory.cpp
    src/handler/SettingsBar.h
    src/handler/SettingsBar.cpp
    src/handler/SettingsFoo.h
    src/handler/SettingsFoo.cpp
    src/Input.h
    src/Input.cpp
    src/Model.h
    src/ModelImpl.h
    src/ModelImpl.cpp
    src/Output.h
    src/Output.cpp
    src/desktop.cpp
    src/desktop.qml
    src/resource.qrc
)

target_link_libraries(vca_desktop
    vca_core
    Qt5::Qml
    Qt5::Quick
)
