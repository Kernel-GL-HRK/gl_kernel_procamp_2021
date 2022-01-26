cmake_minimum_required(VERSION 3.21)
project(ioctl_userspace_app)

set(CMAKE_CXX_STANDARD 17)


if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES
                      fmt/8.0.1
                      GENERATORS cmake_find_package
                      IMPORTS "bin, *.dll -> ${CMAKE_CURRENT_BINARY_DIR}"
                      IMPORTS "lib, *.dylib* -> ${CMAKE_CURRENT_BINARY_DIR}"
                      OPTIONS fmt:header_only=True
                      )

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    PROFILE orange_zero_profile
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(fmt REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)