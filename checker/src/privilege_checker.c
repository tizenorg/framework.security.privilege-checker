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

#include <unistd.h>
#include <stdlib.h>
#include <dlog.h>
#include <privilege_checker.h>

#if defined PROFILE_TYPE_MOBILE || defined PROFILE_TYPE_WEARABLE
#include <privacy_checker_client.h>
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "PRIVILEGE_CHECKER"
#endif

#define TryReturn(condition, expr, returnValue, ...)  \
	if (!(condition)) { \
		LOGE(__VA_ARGS__); \
		expr; \
		return returnValue; \
	} \
	else {;}

static int __privilege_checker_check_privilege(const char* privilege_name, const char* caller_function, char* caller_file, int caller_line)
{
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
		TryReturn(object != NULL, free(object), PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR, "[PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR] object's malloc is failed.");
		snprintf(object, PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL_LEN + strlen(privilege_key_name) + 1, "%s%s", PRIVILEGE_CHECKER_TIZEN_PREFIX_LABEL, privilege_key_name);
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
		LOGE("Please declare %s in tizen-manifest.xml or config.xml", privilege_name);
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

	if (subject != NULL)
	{
		free (subject);
	}

	return ret;
}

int privilege_checker_check_privilege(const char* privilege_name)
{
	int ret = __privilege_checker_check_privilege(privilege_name, __func__, __FILE__, __LINE__);
#if defined PROFILE_TYPE_MOBILE || defined PROFILE_TYPE_WEARABLE
	if (ret == PRIVILEGE_CHECKER_ERR_NONE) {
		switch ( ret = privacy_checker_check_by_privilege(privilege_name)) 
		{
			case 0:
				ret = PRIVILEGE_CHECKER_ERR_NONE;
				break;
			case -19:
				ret = PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE;
				break;
			default:
				ret = PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR;
		}
		return ret;
	}
#endif
	return ret;
}

