cmake_minimum_required(VERSION 3.16)

#=============================================================================
# PROJECT CONFIGURATION
#=============================================================================
# Basic project setup, versioning, and testing framework initialization
project(
  "stuffy"
  VERSION 0.1.0
  DESCRIPTION "Desktop platform layer."
)

#-----------------------------------------------------------------------------
# BUILD MODE DETECTION
#-----------------------------------------------------------------------------
# Detect if this is a standalone build or being included as a subdirectory
string(
  COMPARE EQUAL
  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}"
  STUFFY_IS_STANDALONE_BUILD
)


#-----------------------------------------------------------------------------
# COMPILER-SPECIFIC SETTINGS
#-----------------------------------------------------------------------------
# Configure compiler flags and options for different toolchains
set(STUFFY_DEFAULT_BUILD_TYPE "Debug")

if (MSVC)
  set(STUFFY_COMPILE_OPTIONS
    /W4
    # add new msvc compile options here...
  )
else()
  set(STUFFY_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic

    -Wno-gnu-zero-variadic-macro-arguments
    # add new clang or gnu compile options here...
  )
endif()

#-----------------------------------------------------------------------------
# CMAKE GLOBAL SETTINGS
#-----------------------------------------------------------------------------
# Set global CMake behavior, C standard, and build system options
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ${STUFFY_IS_STANDALONE_BUILD})

#-----------------------------------------------------------------------------
# PROJECT OPTIONS
#-----------------------------------------------------------------------------
# Include user-configurable build options
include(CMakeOptions.txt)

#-----------------------------------------------------------------------------
# BUILD TYPE CONFIGURATION
#-----------------------------------------------------------------------------
# Set default build type for standalone builds
if (STUFFY_IS_STANDALONE_BUILD AND CMAKE_BUILD_TYPE STREQUAL "")
  message("${PROJECT_NAME} | warn: Build type wasn't specified. Automatically set to \"${STUFFY_DEFAULT_BUILD_TYPE}\".")
  set(CMAKE_BUILD_TYPE ${STUFFY_DEFAULT_BUILD_TYPE})
endif()


#-----------------------------------------------------------------------------
# BUILD INFORMATION DISPLAY
#-----------------------------------------------------------------------------
# Print project information and configuration summary
include(CMakeInfo.txt)

#-----------------------------------------------------------------------------
# SOURCE FILES CONFIGURATION
#-----------------------------------------------------------------------------
# Define source files for the library
set(STUFFY_SOURCE_FILES
  src/app.c
  src/input/keyboard.c
  src/input/mouse.c
  # add new platform independent source files here...
)

# Platform-specific source files
if(APPLE)
  set(STUFFY_SOURCE_FILES
    ${STUFFY_SOURCE_FILES}
    src/macos/App.m
    src/macos/AppDelegate.m
    src/macos/ContentView.m
    src/macos/Window.m
    src/macos/WindowDelegate.m
    # add new macos source files here...
  )

  find_library(COCOA_LIBRARY Cocoa)
  find_library(QUARTZ_LIBRARY QuartzCore)

  set(STUFFY_DEPS
    ${COCOA_LIBRARY}
    ${QUARTZ_LIBRARY}
  )
elseif(LINUX)
  set(STUFFY_SOURCE_FILES
    ${STUFFY_SOURCE_FILES}
    src/linux/app.c
    src/linux/window.c
    # add new linux source files here...
  )
  set(STUFFY_DEPS "")
elseif(WIN32)
  set(STUFFY_SOURCE_FILES
    ${STUFFY_SOURCE_FILES}
    src/windows/app.c
    src/windows/window.c
    # add new windows source files here...
  )
  set(STUFFY_DEPS "")
else()
  message(FATAL_ERROR "Unknown target platform.")
endif()

#=============================================================================
# LIBRARY TARGET CREATION
#=============================================================================
# Create the main library target with appropriate configuration
if(STUFFY_BUILD_SHARED)
  add_library(stuffy SHARED ${STUFFY_SOURCE_FILES})
  target_compile_definitions(stuffy PUBLIC __STUFFY_SHARED__)
  target_compile_definitions(stuffy PRIVATE __STUFFY_EXPORT__)
else()
  add_library(stuffy STATIC ${STUFFY_SOURCE_FILES})
endif()

target_include_directories(stuffy PUBLIC include)
target_include_directories(stuffy PRIVATE .)

target_compile_options(stuffy PRIVATE ${STUFFY_COMPILE_OPTIONS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(stuffy PUBLIC __STUFFY_DEBUG__)
endif()

if(STUFFY_DEPS)
  target_link_libraries(stuffy PRIVATE ${STUFFY_DEPS})
endif()

if(STUFFY_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

if(STUFFY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
