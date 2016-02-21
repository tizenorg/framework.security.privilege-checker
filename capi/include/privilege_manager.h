/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_SECURITY_PRIVILEGE_MANAGER_H
#define __TIZEN_SECURITY_PRIVILEGE_MANAGER_H

#include <tizen.h>
#include <glib.h>
#include <cert-service.h>

#ifdef __cplusplus
extern "C" {
#endif

//typedef cert_svc_visibility privilege_manager_visibility_e;

typedef enum
{
    PRVMGR_PACKAGE_VISIBILITY_PUBLIC        =  CERT_SVC_VISIBILITY_PUBLIC,
    PRVMGR_PACKAGE_VISIBILITY_PARTNER       =  CERT_SVC_VISIBILITY_PARTNER,
    PRVMGR_PACKAGE_VISIBILITY_PLATFORM      =  CERT_SVC_VISIBILITY_PLATFORM,
} privilege_manager_visibility_e;

typedef enum
{
    PRVMGR_PACKAGE_TYPE_WRT     =  0,
    PRVMGR_PACKAGE_TYPE_CORE    =  1,
} privilege_manager_package_type_e;

/**
 * @brief Enumerations of error code for Privilege Information.
 */
typedef enum
{
    PRVMGR_ERR_NONE                         = 0,
    PRVMGR_ERR_INVALID_PARAMETER            = 1,
    PRVMGR_ERR_OUT_OF_MEMORY                = 2,
    PRVMGR_ERR_INTERNAL_ERROR               = 3,
    PRVMGR_ERR_NO_DATA                      = 4,
    PRVMGR_ERR_NO_EXIST_PRIVILEGE           = 5,
    PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL    = 6,
    PRVMGR_ERR_INVALID_PRIVILEGE            = 7,
    PRVMGR_ERR_DEPRECATED_PRIVILEGE         = 8,
} privilege_manager_error_e;

/**
 * @brief verify privilege in the privilege list by checking its name and level at given api version.
 * @remarks @a privilege_list must be released with free() by you.
 * @remarks @a error_message must be released with free() by you.
 * @param [in] api_version The api version of the application
 * @param [in] privilege_manager_package_type_e The privilege_manager_package_type_e
 * @param [in] privilege_list The privilege_list
 * @param [in] privilege_manager_visibility_e The privilege_manager_visibility_e
 * @param [out] error_message The error_message's value is assigned when the return value isn't PRVMGR_ERR_NONE, otherwise null is assigned.
 * @return 0 on success, otherwise a nonzero error value.
 * @retval #PRVMGR_ERR_NONE Successful
 * @retval #PRVMGR_ERR_OUT_OF_MEMORY Out of memory
 * @retvak #PRVMGR_ERR_INVALID_PRIVILEGE The privilege_list contains invalid privilege(s).
 * @retval #PRVMGR_ERR_INVALID_PARAMETER Invalid parameter
 * @retval #PRVMGR_ERR_INTERNAL_ERROR Internal error
 */
EXPORT_API int privilege_manager_verify_privilege(const char* api_version, privilege_manager_package_type_e package_type, GList* privilege_list, privilege_manager_visibility_e visibility, char **error_message);

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SECURITY_PRIVILEGE_MANAGER_H


