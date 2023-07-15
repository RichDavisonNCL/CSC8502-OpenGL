cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(CSC8502-OpenGL CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ASSET_ROOT "${CMAKE_SOURCE_DIR}/Assets/" CACHE STRING "" FORCE)
add_compile_definitions(ASSETROOTLOCATION="${ASSET_ROOT}") 

if(MSVC)
    add_compile_definitions("NOMINMAX")
    add_compile_definitions("WIN32_LEAN_AND_MEAN")  
endif()

################################################################################
# Sub-projects
################################################################################
add_subdirectory(NCLCoreClasses)
add_subdirectory(GLTFLoader)
add_subdirectory(OpenGLRendering)
add_subdirectory(CSC8502)

FetchContent_Declare(nclassets 
	URL https://github.com/RichDavisonNCL/NCLCoreAssets/raw/main/NCLCoreAssets.zip
	#GIT_TAG main
	DOWNLOAD_NO_EXTRACT true  	 
)    
 
FetchContent_Populate(nclassets)       

file(
	ARCHIVE_EXTRACT 
	INPUT ${nclassets_SOURCE_DIR}/NCLCoreAssets.zip  
	DESTINATION ${ASSET_ROOT}
)
 
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CSC8502)
endif()