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

#include <stdlib.h>
#include <libintl.h>
#include <locale.h>
#include <dlog.h>
#include <pkgmgr-info.h>
#include "privilege_db_manager.h"
#include "privilege_info.h"
#include "privilege_info_types.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "PRIVILEGE_INFO"
#endif

#define TryReturn(condition, expr, returnValue, ...)	\
	if (!(condition)) { \
		LOGE(__VA_ARGS__); \
		expr; \
		return returnValue;	\
	} \
	else {;}

int privilege_info_privilege_list_by_pkgid_callback (const char *privilege_name, void *user_data)
{
	LOGD("privilege name = %s", privilege_name);

	int* groupTable = (int*)user_data;
	TryReturn(privilege_name != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege_name is NULL");
	TryReturn(user_data != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] user_data is NULL");

	int group_id = 6;
	//core
	int ret = privilege_db_manager_get_privilege_group_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege_name, "9.9", &group_id);
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
		LOGD("group_id = %d", group_id);
		groupTable[group_id] = 1;

		return PRVMGR_ERR_NONE;
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        return PRVMGR_ERR_INTERNAL_ERROR;
    }
	//wrt
	ret = privilege_db_manager_get_privilege_group_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege_name, "9.9", &group_id);
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
		LOGD("group_id = %d", group_id);
		groupTable[group_id] = 1;

		return PRVMGR_ERR_NONE;
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        return PRVMGR_ERR_INTERNAL_ERROR;
    }
	//osp
	ret = privilege_db_manager_get_privilege_group_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_OSP, privilege_name, "9.9", &group_id);
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
		LOGD("group_id = %d", group_id);
		groupTable[group_id] = 1;

		return PRVMGR_ERR_NONE;
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        return PRVMGR_ERR_INTERNAL_ERROR;
    }

	groupTable[EXTRA_GROUP] = 1;

	return PRVMGR_ERR_NONE;
}

int privilege_info_foreach_privilege_group_list_by_pkgid(const char *package_id, privilege_info_privileges_cb callback, void* user_data)
{
	LOGD("package id = %s", package_id);

	int groupTable[MAX_PRV_GROUP] = {0,};
	int i = 0;
	int res = PRVMGR_ERR_NONE;

	TryReturn(package_id != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] package_id is NULL");
	TryReturn(callback != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] callback is NULL");

	pkgmgrinfo_pkginfo_h handle;
	res = pkgmgrinfo_pkginfo_get_pkginfo(package_id, &handle);
	TryReturn(res == PMINFO_R_OK, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] pkgmgrinfo_pkginfo_get_pkginfo is failed.");

	res = pkgmgrinfo_pkginfo_foreach_privilege(handle, privilege_info_privilege_list_by_pkgid_callback, &groupTable);
	pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
	TryReturn(res == PMINFO_R_OK, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] pkgmgrinfo_pkginfo_foreach_privilege is failed.")

	for (i = 0; i < MAX_PRV_GROUP; i++)
	{
		if (groupTable[i] == 1)
		{
			res = callback(privilege_group_info_table[i].privilege_group, user_data);
			LOGD("group = %s", privilege_group_info_table[i].privilege_group);

			TryReturn(res >= 0, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] return value of callback function is negative.");
		}
	}

	return PRVMGR_ERR_NONE;
}

int privilege_info_privilege_list_callback (const char *privilege_name, void *user_data)
{
	LOGD("privilege_name = %s", privilege_name);

	int res = PRVMGR_ERR_NONE;
	privilege_list_cb_data data = *((privilege_list_cb_data*)(user_data));
	int group_id = 6;

	TryReturn(privilege_name != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege_name is NULL");
	TryReturn(user_data != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] user_data is NULL");

	//core
	int ret = privilege_db_manager_get_privilege_group_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege_name, "9.9", &group_id);
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
		if(group_id == data.privilege_group){
			LOGD("data.privilege_group = %d", data.privilege_group);
			res = data.callback(privilege_name, data.user_data);
			TryReturn(res >= 0, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] return value of callback function is negative.");

			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        return PRVMGR_ERR_INTERNAL_ERROR;
    }

	//wrt
	ret = privilege_db_manager_get_privilege_group_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege_name, "9.9", &group_id);
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
		if(group_id == data.privilege_group){
			LOGD("data.privilege_group = %d", data.privilege_group);
			res = data.callback(privilege_name, data.user_data);
			TryReturn(res >= 0, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] return value of callback function is negative.");

			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        return PRVMGR_ERR_INTERNAL_ERROR;
    }

	//osp
	ret = privilege_db_manager_get_privilege_group_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_OSP, privilege_name, "9.9", &group_id);
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
		if(group_id == data.privilege_group){
			LOGD("data.privilege_group = %d", data.privilege_group);
			res = data.callback(privilege_name, data.user_data);
			TryReturn(res >= 0, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] return value of callback function is negative.");

			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        return PRVMGR_ERR_INTERNAL_ERROR;
    }

	if ( data.privilege_group == EXTRA_GROUP )
	{
		LOGD("data.privilege_group = %d", data.privilege_group);
		res = data.callback(privilege_name, data.user_data);
		TryReturn(res >= 0, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] return value of callback function is negative.");
	}

	return PRVMGR_ERR_NONE;
}

int privilege_info_foreach_privilege_list_by_pkgid_and_privilege_group(const char *package_id, const char* privilege_group, privilege_info_privileges_cb callback, void* user_data)
{
	LOGD("package_id = %s, privilege_group = %s", package_id, privilege_group);

	int i = 0;
	int res = PRVMGR_ERR_NONE;
	privilege_list_cb_data data;

	TryReturn(package_id != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] package_id is NULL");
	TryReturn(privilege_group != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege_group is NULL");
	TryReturn(callback != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] callback is NULL");

	data.privilege_group = -1;
	data.callback = callback;
	data.user_data = user_data;

	for (i = 0; i < MAX_PRV_GROUP; i++)
	{
		if (strcmp(privilege_group_info_table[i].privilege_group, privilege_group) == 0)
		{
			data.privilege_group = privilege_group_info_table[i].privilege_group_enum;
			break;
		}
	}

	if (data.privilege_group > -1)
	{
		pkgmgrinfo_pkginfo_h handle;
		res = pkgmgrinfo_pkginfo_get_pkginfo(package_id, &handle);
		TryReturn(res == PMINFO_R_OK, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] pkgmgrinfo_pkginfo_get_pkginfo is failed.")

		res = pkgmgrinfo_pkginfo_foreach_privilege(handle, privilege_info_privilege_list_callback, &data);
		pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
		TryReturn(res == PMINFO_R_OK, , PRVMGR_ERR_INTERNAL_ERROR, "[PRVMGR_ERR_INTERNAL_ERROR] pkgmgrinfo_pkginfo_foreach_privilege is failed.")
	}

	return PRVMGR_ERR_NONE;
}

int privilege_info_get_group_name_string_id(const char *privilege_group, char **group_string_id)
{
	LOGD("privilege_group = %s", privilege_group);

	int index = 0;
	TryReturn(privilege_group != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");

	for (index = 0; index < MAX_PRV_GROUP; index++)
	{
		if (strcmp(privilege_group_info_table[index].privilege_group, privilege_group) == 0)
		{
			*group_string_id = (char*)calloc(strlen(privilege_group_info_table[index].name_string_id) + 1, sizeof(char));
			TryReturn(*group_string_id != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

			memcpy(*group_string_id, privilege_group_info_table[index].name_string_id, strlen(privilege_group_info_table[index].name_string_id));
			break;
		}
	}

	return PRVMGR_ERR_NONE;
}

int privilege_info_get_privilege_group_display_name_by_string_id(const char *string_id, char **name)
{
	LOGD("string_id = %s", string_id);

	char *temp = NULL;

	TryReturn(string_id != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] string_id is NULL");

	temp = dgettext("privilege", string_id);

	*name = (char*)calloc(strlen(temp) + 1, sizeof(char));
	TryReturn(*name != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

	memcpy(*name, temp, strlen(temp));

	return PRVMGR_ERR_NONE;
}

int privilege_info_get_privilege_group_display_name(const char *privilege_group, char **name)
{
	LOGD("privilege_group = %s", privilege_group);

	int ret = 0;
	char* name_string_id = NULL;

	TryReturn(privilege_group != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");


	ret = privilege_info_get_group_name_string_id(privilege_group, &name_string_id);


	
	if (name_string_id == NULL)
	{
		char tempPrivilegeGroup[256] = {0,};
		char* temp = NULL;
		char* buffer = NULL;
		char* save = NULL;
		memcpy(tempPrivilegeGroup, privilege_group, strlen(privilege_group));
		temp = strtok_r(tempPrivilegeGroup, "/", &save);
		while(temp)
		{
			buffer = temp;
			temp = strtok_r(NULL, "/", &save);
		}
		*name = (char*)calloc(strlen(buffer) + 1, sizeof(char));
		TryReturn(*name != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

		memcpy(*name, buffer, strlen(buffer));
	}
	else
	{
		ret = privilege_info_get_privilege_group_display_name_by_string_id(name_string_id, name);
		free(name_string_id);
		TryReturn(ret == PRVMGR_ERR_NONE, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");
	}
	return  PRVMGR_ERR_NONE;
}

int privilege_info_get_name_string_id(const char *privilege, char **name_string_id)
{
	TryReturn(privilege != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");

	char* temp = NULL;

	// Check CORE
    int ret = privilege_db_manager_get_privilege_display_by_name(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, &temp);

    LOGD("privilege = %s, string id = %s", privilege, temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            *name_string_id = NULL;
        }
        else if(strcmp(temp,"") == 0)
        {
            //*name_string_id = NULL;
			*name_string_id = strdup("");
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
            return PRVMGR_ERR_NONE;
        }
        else
        {
            *name_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
            TryReturn(*name_string_id != NULL, free(temp), PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation is failed.");
            memcpy(*name_string_id, temp, strlen(temp));
            LOGD("display_name_string_id = %s", *name_string_id);
            if(temp != NULL){
				free(temp);
				temp = NULL;
			}
            return PRVMGR_ERR_NONE;
        }
    }   
    else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
		if(temp != NULL){
			free(temp);
			temp = NULL;
		}
        return PRVMGR_ERR_INTERNAL_ERROR;
    }

	if(temp != NULL)
	{
		free(temp);
		temp = NULL;
	}

	// Check WRT
	ret = privilege_db_manager_get_privilege_display_by_name(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege, &temp);

	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
	{
		if(temp == NULL)
		{
			*name_string_id = NULL;
		}
		else if(strcmp(temp, "") == 0)
		{
			//*name_string_id = NULL;
			*name_string_id = strdup("");
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
		else
		{
			*name_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
			TryReturn(*name_string_id != NULL, free(temp), PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation is failed.");
			memcpy(*name_string_id, temp, strlen(temp));
			LOGE("display_name_string_id = %s", *name_string_id);
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
	{
		if(temp != NULL){
			free(temp);
			temp = NULL;
		}
		return PRVMGR_ERR_INTERNAL_ERROR;
	}

	if(temp != NULL)
	{
		free(temp);
		temp = NULL;
	}

	// Check OSP
    ret = privilege_db_manager_get_privilege_display_by_name(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_OSP, privilege, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            *name_string_id = NULL;
            return PRVMGR_ERR_NONE;
        }
        else if(strcmp(temp,"") == 0)
        {
            //*name_string_id = NULL;
            *name_string_id = strdup("");
            if(temp != NULL){
				free(temp);
				temp = NULL;
			}
            return PRVMGR_ERR_NONE;
        }
        else
        {
            *name_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
            TryReturn(*name_string_id != NULL, free(temp), PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation is failed.");
            memcpy(*name_string_id, temp, strlen(temp));
            LOGD("display_name_string_id = %s", *name_string_id);
            if(temp != NULL){
				free(temp);
				temp = NULL;
			}
            return PRVMGR_ERR_NONE;
        }
    }
    else if(ret == PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        *name_string_id = NULL;
        if(temp != NULL){
			free(temp);
			temp = NULL;
		}
		LOGD("There is no %s in db", privilege);
        return PRVMGR_ERR_NONE;
    }
    else
    {
        if(temp != NULL){
			free(temp);
			temp = NULL;
		}
        return PRVMGR_ERR_INTERNAL_ERROR;
    }

	return PRVMGR_ERR_NONE;
}

int privilege_info_get_privilege_display_name_by_string_id(const char *string_id, char **name)
{
	char *temp = NULL;

	TryReturn(string_id != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] string_id is NULL");

	temp = dgettext("privilege", string_id);

	*name = (char*)calloc(strlen(temp) + 1, sizeof(char));
	TryReturn(*name != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

	memcpy(*name, temp, strlen(temp));

	return PRVMGR_ERR_NONE;
}

int privilege_info_get_privilege_display_name(const char *privilege, char **name)
{
	int ret = 0;
	char* name_string_id = NULL;

	TryReturn(privilege != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");

	ret = privilege_info_get_name_string_id(privilege, &name_string_id);
	if (name_string_id == NULL)
	{
		/*
		char tempPrivilege[256] = {0,};
		char* temp = NULL;
		char* buffer = NULL;
		memcpy(tempPrivilege, privilege, strlen(privilege));
		temp = strtok(tempPrivilege, "/");
		while(temp)
		{
			buffer = temp;
			temp = strtok(NULL, "/");
		}
		*name = (char*)calloc(strlen(buffer) + 1, sizeof(char));
		TryReturn(*name != NULL, PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

		memcpy(*name, buffer, strlen(buffer));
		*/
		//*name = strdup("");
	}
	else if(strcmp(name_string_id,"") == 0){
		*name = strdup("display string is not defined yet");
	}
	else
	{
		ret = privilege_info_get_privilege_display_name_by_string_id(name_string_id, name);
		free(name_string_id);
		name_string_id = NULL;
		TryReturn(ret == PRVMGR_ERR_NONE, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");
	}

	if(name_string_id != NULL)
		free(name_string_id);
	
	return PRVMGR_ERR_NONE;
}

int privilege_info_get_description_string_id(const char *privilege, char **description_string_id)
{
	TryReturn(privilege != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");

	char* temp = NULL;

	// Check CORE
	int ret = privilege_db_manager_get_privilege_description_by_name(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, &temp);

	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
	{
		if(temp == NULL)
		{
			*description_string_id = NULL;
		}
		else if(strcmp(temp, "") == 0)
		{
			//*description_string_id = NULL;
			*description_string_id = strdup("");
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
		else
		{
			*description_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
			TryReturn(*description_string_id != NULL, free(temp), PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation is failed.");
			memcpy(*description_string_id, temp, strlen(temp));
			LOGD("description_string_id = %s", *description_string_id);
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
	{
		if(temp != NULL){
			free(temp);
			temp = NULL;
		}
		return PRVMGR_ERR_INTERNAL_ERROR;
	}

	if(temp != NULL)
	{
		free(temp);
		temp = NULL;
	}

	// Check WRT
	ret = privilege_db_manager_get_privilege_description_by_name(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege, &temp);

	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
	{
		if(temp == NULL)
		{
			*description_string_id = NULL;
		}
		else if(strcmp(temp, "") == 0)
		{
			//*description_string_id = NULL;
			*description_string_id = strdup("");
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
		else
		{
			*description_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
			TryReturn(*description_string_id != NULL, free(temp), PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation is failed.");
			memcpy(*description_string_id, temp, strlen(temp));
			LOGE("description_string_id = %s", *description_string_id);
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
	{
		if(temp != NULL){
			free(temp);
			temp = NULL;
		}
		return PRVMGR_ERR_INTERNAL_ERROR;
	}

	if(temp != NULL)
	{
		free(temp);
		temp = NULL;
	}

	// Check OSP
	ret = privilege_db_manager_get_privilege_description_by_name(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_OSP, privilege, &temp);

	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
	{
		if(temp == NULL)
		{
			*description_string_id = NULL;
			return PRVMGR_ERR_NONE;
		}
		else if(strcmp(temp,"") == 0)
		{
			//*description_string_id = NULL;
			*description_string_id = strdup("");
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
		else
		{
			*description_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
			TryReturn(*description_string_id != NULL, free(temp), PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation is failed.");
			memcpy(*description_string_id, temp, strlen(temp));
			LOGD("description_string_id = %s", *description_string_id);
			if(temp != NULL){
				free(temp);
				temp = NULL;
			}
			return PRVMGR_ERR_NONE;
		}
	}
	else if(ret == PRIVILEGE_DB_NO_EXIST_RESULT)
	{
		*description_string_id = NULL;
		if(temp != NULL){
			free(temp);
			temp = NULL;
		}
		LOGE("There is no %s in db", privilege);
		return PRVMGR_ERR_NONE;
	}
	else
	{
		if(temp != NULL){
			free(temp);
			temp = NULL;
		}
		return PRVMGR_ERR_INTERNAL_ERROR;
	}

	return PRVMGR_ERR_NONE;
}

int privilege_info_get_privilege_description_by_string_id(const char *string_id, char **description)
{
	char *temp = NULL;

	TryReturn(string_id != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] string_id is NULL");

	temp = dgettext("privilege", string_id);

	*description = (char*)calloc(strlen(temp) + 1, sizeof(char));
	TryReturn(*description != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

	memcpy(*description, temp, strlen(temp));

	return  PRVMGR_ERR_NONE;
}

int privilege_info_get_privilege_description(const char *privilege, char **description)
{
	int ret = 0;
	char* description_string_id = NULL;

	TryReturn(privilege != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");

	ret = privilege_info_get_description_string_id(privilege, &description_string_id);
	if (description_string_id == NULL)
	{
		/*
		char *temp = NULL;
		temp = dgettext("privilege", "IDS_TPLATFORM_BODY_THIS_PRIVILEGE_IS_NOT_DEFINED");
		*description = (char*)calloc(strlen(temp) + 1, sizeof(char));
		TryReturn(*description != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

		memcpy(*description, temp, strlen(temp));
		*/
		//*description = strdup("");
	}
	else if(strcmp(description_string_id,"") == 0){

		
		*description = strdup("description string is not defined yet");
	}
	else
	{
		ret = privilege_info_get_privilege_display_name_by_string_id(description_string_id, description);
		free(description_string_id);
		description_string_id = NULL;
		TryReturn(ret == PRVMGR_ERR_NONE, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");
	}

	if(description_string_id != NULL)
		free(description_string_id);

	return  PRVMGR_ERR_NONE;
}

int privilege_info_get_external_privilege_level(const char *privilege, char **privilege_level)
{
	int index = 0;
	bool matchedFlag = false;
	char* public_level = "public";
	TryReturn(privilege != NULL, , PRVMGR_ERR_INVALID_PARAMETER, "[PRVMGR_ERR_INVALID_PARAMETER] privilege is NULL");

	for (index = 0; index < EXTERNAL_PRIVILEGE_NUM; index++)
	{
		if (strcmp(external_privilege_info_table[index].privilege, privilege) == 0)
		{
			matchedFlag = true;
			*privilege_level = (char*)calloc(strlen(external_privilege_info_table[index].privilege_level) + 1, sizeof(char));
			TryReturn(*privilege_level != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

			memcpy(*privilege_level, external_privilege_info_table[index].privilege_level, strlen(external_privilege_info_table[index].privilege_level));
			break;
		}
	}

	if (matchedFlag == false)
	{
		*privilege_level = (char*)calloc(strlen(public_level) + 1, sizeof(char));
		TryReturn(*privilege_level != NULL, , PRVMGR_ERR_OUT_OF_MEMORY, "[PRVMGR_ERR_OUT_OF_MEMORY] Memory allocation failed.");

		memcpy(*privilege_level, public_level, strlen(public_level));
	}

	return  PRVMGR_ERR_NONE;
}

