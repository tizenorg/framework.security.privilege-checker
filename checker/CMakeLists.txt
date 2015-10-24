CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(fw_name "security-privilege-checker")
SET(VERSION_MAJOR 1)
SET(VERSION ${VERSION_MAJOR}.0.1)
INCLUDE(FindPkgConfig)
PROJECT(${fw_name})

IF("${PROFILE_TYPE}" STREQUAL "WEARABLE")
    ADD_DEFINITIONS("-DPROFILE_TYPE_WEARABLE")
ELSEIF("${PROFILE_TYPE}" STREQUAL "MOBILE")
    ADD_DEFINITIONS("-DPROFILE_TYPE_MOBILE")
ELSEIF("${PROFILE_TYPE}" STREQUAL "TV")
    ADD_DEFINITIONS("-DPROFILE_TYPE_TV")
ENDIF()

IF("${PROFILE_TYPE}" STREQUAL "TV")
	pkg_check_modules(${fw_name} REQUIRED dlog libsmack capi-base-common)
ELSE()
	pkg_check_modules(${fw_name} REQUIRED dlog libsmack capi-base-common privacy-manager-client)
ENDIF()

FOREACH(flag ${${fw_name}_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/checker/include)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -Werror")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed")

SET(${fw_name}_framework_src
	src/test_framework.c
)

ADD_LIBRARY(${fw_name} SHARED src/privilege_checker.c)

TARGET_LINK_LIBRARIES(${fw_name} ${${fw_name}_LDFLAGS})

SET_TARGET_PROPERTIES(${fw_name} 
    PROPERTIES 
    SOVERSION ${API_VERSION} 
    VERSION ${VERSION} 
    CLEAN_DIRECT_OUTPUT 1
)

INSTALL(TARGETS ${fw_name} DESTINATION ../lib)
IF("${PROFILE_TYPE}" STREQUAL "TV")
	SET(pc_requires "libsmack")
ELSE()
	SET(pc_requires "libsmack privacy-manager-client")
ENDIF()
SET(PC_NAME ${fw_name})
SET(PC_REQUIRED ${pc_requires})
SET(PC_LDFLAGS "-l${fw_name}")
SET(PC_CFLAGS "")

CONFIGURE_FILE(${fw_name}.pc.in ${fw_name}.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/privilege_checker.h DESTINATION /usr/include)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc DESTINATION ../lib/pkgconfig)



