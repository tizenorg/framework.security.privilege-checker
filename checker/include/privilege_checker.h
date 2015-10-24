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

#ifndef __TIZEN_SECURITY_PRIVILEGE_CHECKER_H
#define __TIZEN_SECURITY_PRIVILEGE_CHECKER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief check the privilege is declared or not for current application
 * @param [in] privilege_name Privilege name such as "http:tizen.org/privilege/$name"
 * @retval #PRIVILEGE_CHECKER_ERR_NONE Success. Privilege is declared in current application
 * @retval #PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE Privilege is not declared in current application
 * @retval #PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER Invalid privilege name
 * @retval #PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR Internal I/O Error
 * int privilege_checker_check_privilege(const char* privilege_name)
 */

#include <tizen.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlog.h>
#include <sys/smack.h>

typedef enum
{
	PRIVILEGE_CHECKER_ERR_NONE					= TIZEN_ERROR_NONE,
	PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE	= TIZEN_ERROR_PERMISSION_DENIED,
	PRIVILEGE_CHECKER_ERR_UNCONSENTED			= TIZEN_ERROR_PERMISSION_DENIED,
	PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER 	= TIZEN_ERROR_INVALID_PARAMETER, 
	PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR		= TIZEN_ERROR_UNKNOWN, 
} privilege_checker_error_e;

#define PRIVILEGE_CHECKER_TIZEN_PRIVILEGE "http://tizen.org/privilege/"
#define PRIVILEGE_CHECKER_TIZEN_PRIVILEGE_LEN strlen(PRIVILEGE_CHECKER_TIZEN_PRIVILEGE)
#define PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL "privilege::tizen::"
#define PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL_LEN strlen(PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL)
#define PRIVILEGE_CHECKER_IS_TIZEN_PRIVILEGE(name) \
	!strncmp(PRIVILEGE_CHECKER_TIZEN_PRIVILEGE, name, PRIVILEGE_CHECKER_TIZEN_PRIVILEGE_LEN)
#define PRIVILEGE_CHECKER_GET_PRIVILEGE_NAME(name, len) (char *)(name + len)
#define PRIVILEGE_CHECKER_RESERVED_PERMISSION "rw"


/**
 * @brief check the privilege is declared or not for current application
 * @param [in] privilege_name Privilege name such as "http://tizen.org/privilege/$name"
 * @retval #PRIVILEGE_CHECKER_ERR_NONE Success. Privilege is declared in current application
 * @retval #PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE Privilege is not declared in current application
 * @retval #PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER Invalid privilege name
 * @retval #PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR Internal I/O Error
 * int privilege_checker_check_privilege(const char* privilege_name)
 */
EXPORT_API int privilege_checker_check_privilege(const char* privilege_name);

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SECURITY_PRIVILEGE_INFO_H
