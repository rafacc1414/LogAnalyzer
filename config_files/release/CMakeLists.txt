cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "LogAnalyzer") # CUSTOMISE: your project name
set(PROJECT_VERSION 0.1.0) # CUSTOMISE: project version
# set(CMAKE_CXX_STANDARD 17 ) # CUSTOMISE: standard
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

find_package(nlohmann_json 3.11.3 REQUIRED)

# Include all esential directories
include_directories(
    ./../../include
)


# add all files of source folder
file(GLOB SOURCES
    ./../../include/*.hpp
	./../../src/*.cpp
)

message("Building with CMake version: ${CMAKE_VERSION}")

# $source_files is a space-delimited list of filenames.
add_executable(${PROJECT_NAME} ${SOURCES})  # Specifies the executable to build.
set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES 
        OUTPUT_NAME ${PROJECT_NAME}
        SUFFIX ".bin"
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json pthread)