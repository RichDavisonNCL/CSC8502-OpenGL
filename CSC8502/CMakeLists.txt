set(PROJECT_NAME CSC8502)

################################################################################
# Source groups
################################################################################

file(GLOB_RECURSE Header_Files CONFIGURE_DEPENDS *.h)
source_group("Header Files" FILES ${Header_Files})

file(GLOB_RECURSE Source_Files CONFIGURE_DEPENDS *.cpp)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <memory>
    <unordered_set>
    <vector>
    <string>
    <fstream>
    <iostream>
    <string>
    <iosfwd>
    <set>

    <Matrix2.h>
    <Matrix3.h>
    <Matrix4.h>
    <Vector2.h>
    <Vector3.h>
    <Vector4.h>
    <Quaternion.h>

    <Camera.h>
    <TextureLoader.h>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
    )
endif()

################################################################################
# Dependencies
################################################################################
target_include_directories (${PROJECT_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/NCLCoreClasses
    PUBLIC ${CMAKE_SOURCE_DIR}/OpenGLRendering
    PUBLIC ${CMAKE_SOURCE_DIR}/GLTFLoader
)	

set(PROJECT_DEPENDENCIES
    NCLCoreClasses
    OpenGLRendering
    GLTFLoader
)

add_dependencies(${PROJECT_NAME}
    ${PROJECT_DEPENDENCIES}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE ${PROJECT_DEPENDENCIES}
)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC NCLCoreClasses)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenGLRendering)