# Copyright (c) Microsoft Corporation.
# Licensed under the GNU General Public License v3.0 or later.
# See License.txt in the project root for license information.
#

if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_DIFI difi)

FIND_PATH(
    DIFI_INCLUDE_DIRS
    NAMES DIFI/api.h
    HINTS $ENV{DIFI_DIR}/include
        ${PC_DIFI_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DIFI_LIBRARIES
    NAMES gnuradio-DIFI
    HINTS $ENV{DIFI_DIR}/lib
        ${PC_DIFI_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/DIFITarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DIFI DEFAULT_MSG DIFI_LIBRARIES DIFI_INCLUDE_DIRS)
MARK_AS_ADVANCED(DIFI_LIBRARIES DIFI_INCLUDE_DIRS)
