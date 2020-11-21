# FindWindowsSDK.cmake
#
# Finds the Windows SDK
#
# This will define the following variables
#
#   WindowsSDK_FOUND
#   WindowsSDK_dxc_PROGRAM
#
# The following variables can be set as arguments
#
#   WindowsSDK_ROOT_DIR
#

FIND_PROGRAM(
    WindowsSDK_dxc_PROGRAM
    dxc
    PATHS
        ${WindowsSDK_ROOT_DIR}
    PATH_SUFFIXES
        bin
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    WindowsSDK
    HANDLE_COMPONENTS
    REQUIRED_VARS
        WindowsSDK_dxc_PROGRAM
)
