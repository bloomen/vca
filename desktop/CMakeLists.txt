add_executable(vca_desktop 
    src/main.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_desktop 
    gcl
    vca_core
)
