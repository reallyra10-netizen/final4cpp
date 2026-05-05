cmake_minimum_required(VERSION 3.14)
project(WorkerConsole)

set(CMAKE_CXX_STANDARD 17)

# Enable UTF-8 encoding for MSVC compiler
if(MSVC)
    add_compile_options(/utf-8)
endif()

include(FetchContent)

# Fetch xlnt
FetchContent_Declare(
  xlnt
  GIT_REPOSITORY https://github.com/tfussell/xlnt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(xlnt)

# Fetch tabulate
FetchContent_Declare(
  tabulate
  GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
  GIT_TAG master
)
FetchContent_MakeAvailable(tabulate)

# Add your main.cpp or other source files
add_executable(WorkerConsole main.cpp src/Student.cpp src/ExcelUtils.cpp src/MenuUtils.cpp src/User.cpp)
# Tell the compiler where to find all the headers 
target_include_directories(WorkerConsole PRIVATE include )
# Link libraries
target_link_libraries(WorkerConsole PRIVATE xlnt tabulate)

add_custom_command(TARGET WorkerConsole POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/build/_deps/xlnt-build/source/Release/xlnt.dll"
        $<TARGET_FILE_DIR:WorkerConsole>)
