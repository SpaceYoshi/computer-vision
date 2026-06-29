cmake_minimum_required(VERSION 4.2.2)

project(computer_vision VERSION 1.0.0 LANGUAGES CXX)

if(WIN32 AND DEFINED VCPKG_TARGET_TRIPLET)
    if(VCPKG_TARGET_TRIPLET STREQUAL "x64-windows" AND NOT MSVC)
        message(FATAL_ERROR
            "The x64-windows vcpkg triplet must be used with the Visual Studio/MSVC toolchain. "
        )
    elseif(VCPKG_TARGET_TRIPLET STREQUAL "x64-mingw-dynamic" AND NOT MINGW)
        message(FATAL_ERROR
            "The x64-mingw-dynamic vcpkg triplet must be used with a MinGW toolchain. "
        )
    endif()
endif()

find_package(OpenCV 4 REQUIRED COMPONENTS
    core
    highgui
    imgcodecs
    imgproc
    objdetect
    videoio
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
    COMMENT "Copying runtime resources"
)

function(add_cv_program target source)
    add_executable(${target} ${source})
    target_compile_features(${target} PRIVATE cxx_std_23)
    target_link_libraries(${target} PRIVATE ${OpenCV_LIBS})
    target_include_directories(${target} SYSTEM PRIVATE ${OpenCV_INCLUDE_DIRS})
    add_dependencies(${target} copy_resources)

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /EHsc)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# Primary application
add_cv_program(russian_plate src/russian_plate.cpp)

# Homework assignments
add_cv_program(exercise_1_2 assignments/exercise_1_2.cpp)
add_cv_program(exercise_1_3 assignments/exercise_1_3.cpp)
add_cv_program(exercise_1_4 assignments/exercise_1_4.cpp)
add_cv_program(exercise_2_1 assignments/exercise_2_1.cpp)
add_cv_program(exercise_2_2 assignments/exercise_2_2.cpp)
add_cv_program(exercise_2_3 assignments/exercise_2_3.cpp)
add_cv_program(exercise_2_4 assignments/exercise_2_4.cpp)
