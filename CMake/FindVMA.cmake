# FindVMA.cmake
#
# Finds the Vulkan Memory Allocator (VMA)
#
# This will define the following variables
#
#   VMA_FOUND
#   VMA_INCLUDE_DIR
#
# and the following imported targets
#
#   VMA::VMA
#
# The following variables can be set as arguments
#
#   VMA_ROOT
#

find_path(
    VMA_INCLUDE_DIR
    "vk_mem_alloc.h"
    PATHS
        ${Vulkan_INCLUDE_DIRS} # From find_package(Vulkan)
        ${VULKAN_SDK}
        ENV VULKAN_SDK
    PATH_SUFFIXES
        vma
        include
        Include
        include/vma
        Include/vma
)

mark_as_advanced(
    VMA_INCLUDE_DIR
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    VMA
    REQUIRED_VARS
        VMA_INCLUDE_DIR
)

if(VMA_FOUND)

    if(NOT TARGET VMA::VMA)

        add_library(VMA::VMA INTERFACE IMPORTED)

        set_target_properties(
            VMA::VMA
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${VMA_INCLUDE_DIR}"
        )
        
    endif()

endif()