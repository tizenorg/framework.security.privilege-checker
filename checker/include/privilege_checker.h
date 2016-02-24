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

#include <tizen.h>

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
#include "privilege_checker_extension.h"

typedef enum
{
	PRIVILEGE_CHECKER_ERR_NONE					= TIZEN_ERROR_NONE,
	PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE	= TIZEN_ERROR_PERMISSION_DENIED,
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

#define privilege_checker_check_privilege(name) __privilege_checker_check_privilege(name, __func__, __FILE__, __LINE__)

#define TryReturn(condition, expr, returnValue, ...)  \
    if (!(condition)) { \
        LOGE(__VA_ARGS__); \
        expr; \
        return returnValue; \
    } \
    else {;}

static int __privilege_checker_check_privilege(const char* privilege_name, const char* caller_function, char* caller_file, int caller_line)
{
#ifdef LOG_TAG
#define BACKUP_TAG LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "PRIVILEGE_CHECKER"

#define VALUE(x) #x

	int ret;
	char *subject, *object = NULL;
	char *privilege_key_name = NULL;

	LOGD("_ENTER_");

	if ( privilege_name == NULL )
	{
		LOGE("Invalid privilege name : %s", privilege_name);
		return PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER;
	}

	if (smack_new_label_from_self(&subject) == -1)
	{
		LOGE("smack_new_label_from_self failed : %d", errno);
		return PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR;
	}

	if ( PRIVILEGE_CHECKER_IS_TIZEN_PRIVILEGE(privilege_name) )
	{
		privilege_key_name = PRIVILEGE_CHECKER_GET_PRIVILEGE_NAME(privilege_name, PRIVILEGE_CHECKER_TIZEN_PRIVILEGE_LEN);
		object = (char*)malloc( PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL_LEN + strlen(privilege_key_name) + 1);
		TryReturn( object != NULL,, PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR, "[PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR] object's malloc failed");
		snprintf(object, PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL_LEN + strlen(privilege_key_name) + 1, "%s%s", PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL, privilege_key_name);
	}
	else if ( PRIVILEGE_CHECKER_IS_EXTENSION_PRIVILEGE(privilege_name) )
	{
		privilege_key_name = PRIVILEGE_CHECKER_GET_PRIVILEGE_NAME(privilege_name, PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE_LEN);
		object = (char*)malloc( PRIVILEGE_CHECKER_EXTENSION_PREFIX_LABEL_LEN + strlen(privilege_key_name) + 1);
		TryReturn( object != NULL,, PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR, "[PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR] object's malloc failed");
		snprintf(object, PRIVILEGE_CHECKER_EXTENSION_PREFIX_LABEL_LEN + strlen(privilege_key_name) + 1, "%s%s", PRIVILEGE_CHECKER_EXTENSION_PREFIX_LABEL, privilege_key_name);
	}
	else
	{
		LOGE("Invalid privilege name : %s", privilege_name);
		return PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER;
	}

	switch ( ret = smack_have_access (subject, object, PRIVILEGE_CHECKER_RESERVED_PERMISSION) )
	{
	case 1:
		ret = PRIVILEGE_CHECKER_ERR_NONE;
		break;
	case 0:
		LOGE("Please declare %s in tizen-manifest.xml", privilege_name);
		ret = PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE;
		break;
	case -1:
		LOGE("Internal error[%d]: %s, %s, %s", ret, privilege_name, subject, object);
		ret = PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR;
		break;
	default:
		LOGE("Internal error[%d]: %s, %s, %s", ret, privilege_name, subject, object);
		ret = PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR;
	}

	if (object != NULL)
	{
		free (object);
		object = NULL;
	}
#ifdef BACKUP_TAG
#undef LOG_TAG
#define LOG_TAG VALUE(BACKUP_TAG)
#undef BACKUP_TAG
#endif
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SECURITY_PRIVILEGE_INFO_H
