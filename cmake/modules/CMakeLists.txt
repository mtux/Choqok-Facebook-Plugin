# - Try to find the KFacebook library
# Once done this will define
#
#  KFACEBOOK_FOUND - system has the QJson library
#  KFACEBOOK_INCLUDE_DIR - the QJson include directory
#  KFACEBOOK_LIBRARY - Link this to use the QJson library
#
# Copyright (c) 2011, Mehrdad Momeny, <mehrdad.momeny@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (KFACEBOOK_INCLUDE_DIR AND KFACEBOOK_LIBRARY)
    # in cache already
set(KFACEBOOK_FOUND TRUE)

else (KFACEBOOK_INCLUDE_DIR AND KFACEBOOK_LIBRARY)
    if (NOT WIN32)
        find_package(PkgConfig)
        pkg_check_modules(PC_QJSON QJson)
    endif(NOT WIN32)
        
        find_path(KFACEBOOK_INCLUDE_DIR qjson/parser.h
        HINTS
        ${PC_KFACEBOOK_INCLUDE_DIRS}
        )
        
        find_library(KFACEBOOK_LIBRARY NAMES qjson
        HINTS
        ${PC_KFACEBOOK_LIBRARY_DIRS}
        )
        
        include(FindPackageHandleStandardArgs)
        find_package_handle_standard_args(QJson DEFAULT_MSG KFACEBOOK_LIBRARY KFACEBOOK_INCLUDE_DIR)
        
        mark_as_advanced(KFACEBOOK_INCLUDE_DIR KFACEBOOK_LIBRARY)
        
        endif (KFACEBOOK_INCLUDE_DIR AND KFACEBOOK_LIBRARY)
        
