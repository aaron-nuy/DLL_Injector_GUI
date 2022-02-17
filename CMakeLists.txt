cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DUNICODE -D_UNICODE)

# set the project name
project(DLL_InjectorGUI VERSION 0.1.0)



# add the executable
add_executable(${PROJECT_NAME} WIN32
                    src/cApp.cpp
                    src/cFrame.cpp
                    src/Injector.cpp
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/vc_x64_lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
                                                ${CMAKE_CURRENT_SOURCE_DIR}/include/msvc/)