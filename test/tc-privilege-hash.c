#include <stdio.h>
#include <dlog.h>
#include "privilege_info.h"
#include "privilege_info_types.h"
#include <privilege_db_manager.h>


#define BRIGHTNESS 0
#define RED 31
#define GREEN 32
#define BG_BLACK 40

#define TryReturn(condition, returnValue, ...)	\
	if (!(condition)) { \
		LOGE(__VA_ARGS__); \
		return returnValue;	\
	} \
	else {;}


#define SUB_HASH(str, len, value) ( (str[len-1] << 24) | (str[len-2] << 16) | value )

static int fail_cnt=0;
static int success_cnt=0;

static void __change_color_to_red(){
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, RED);
}

static void __change_color_to_green(){
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, GREEN);
}

static void __change_color_to_origin(){
	printf("%c[%dm", 0x1B, 0);
}



int __privilege_checker_hash(const char* privilege_name)
{
	TryReturn(privilege_name != NULL, -1, "[PRIV_CHECKER_ERR_INVALID_PARAMETER] privilege_name is NULL!");
	int len = strlen(privilege_name);
	TryReturn(len >= 2, -1, "[PRIV_CHECKER_ERR_INVALID_PARAMETER] privilege_name's size must be more than 2!");
	int hash_value = 0;
	int i=0;

	for(i=0; i<len; i++)
	{
		hash_value += privilege_name[i];
	}

	hash_value = SUB_HASH(privilege_name, len, hash_value);

	return hash_value;
}

static const char* __get_result_string(privilege_db_manager_error_e ret){
	if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE){
		return "PRIVILEGE_DB_MANAGER_ERR_NONE";
	}
	else if(ret == PRIVILEGE_DB_MANAGER_ERR_CONNECTION_FAIL){
		return "PRIVILEGE_DB_MANAGER_ERR_CONNECTION_FAIL";
	}
	else if(ret == PRIVILEGE_DB_MANAGER_ERR_INVALID_QUERY){
		return "PRIVILEGE_DB_MANAGER_ERR_INVALID_QUERY";
	}
	else if(ret == PRIVILEGE_DB_MANAGER_ERR_OUT_OF_MEMORY){
		return "PRIVILEGE_DB_MANAGER_ERR_OUT_OF_MEMORY";
	}

    return "PRIVILEGE_DB_ERR_UNDEFINED_ERR";
}

static void __free_privilege_list(GList* privilege_list)
{
	GList* l = NULL;
	for (l = privilege_list; l != NULL; l = l->next)
	{
		privilege_info_db_row_s* privilege_info_db_row = (privilege_info_db_row_s*)l->data;
		if(privilege_info_db_row->profile != NULL)
			free(privilege_info_db_row->profile);
		if(privilege_info_db_row->package_type != NULL)
			free(privilege_info_db_row->package_type);
		if(privilege_info_db_row->privilege_name != NULL)
			free(privilege_info_db_row->privilege_name);
		if(privilege_info_db_row->privilege_display != NULL)
			free(privilege_info_db_row->privilege_display);
		if(privilege_info_db_row->privilege_description != NULL)
			free(privilege_info_db_row->privilege_description);
		if(privilege_info_db_row->privilege_level != NULL)
			free(privilege_info_db_row->privilege_level);
		if(privilege_info_db_row->issued_version != NULL)
			free(privilege_info_db_row->issued_version);
        if(privilege_info_db_row->expired_version != NULL)
            free(privilege_info_db_row->expired_version);
	}
}


static void __test_hash_function()
{
	GList* privilege_list;
	int ret = privilege_db_manager_get_privilege_list("2.4", PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, &privilege_list);
	if(ret != PRIVILEGE_DB_MANAGER_ERR_NONE){
		printf("failed to call privilege_db_manager_get_privilege_list()\n");
		printf("error message = %s\n", __get_result_string(ret));
		__change_color_to_red();
		printf("test fail\n");
		__change_color_to_origin();
		fail_cnt++;
		return;
	}

	GList* l = NULL;
	GList* l2 = NULL;
	int i=0;
	int j=0;
	for (l = privilege_list, i=0; l != NULL; l = l->next, i++)
	{
		privilege_info_db_row_s* privilege_info_db_row = (privilege_info_db_row_s*)l->data;
		printf("hash_value = %d,  privilege_name = %s\n", __privilege_checker_hash(privilege_info_db_row->privilege_name), privilege_info_db_row->privilege_name);
		
		for (l2 = privilege_list, j=0; l2 != NULL; l2 = l2->next, j++)
		{
			privilege_info_db_row_s* privilege_info_db_row2 = (privilege_info_db_row_s*)l2->data;

			if(i < j && __privilege_checker_hash(privilege_info_db_row->privilege_name) == __privilege_checker_hash(privilege_info_db_row2->privilege_name)){
				printf("hash conflict----------------------------------------------------------\n");
				printf("conflict privilege name = %s\n", privilege_info_db_row->privilege_name);
				printf("conflict privilege hash = %d\n", __privilege_checker_hash(privilege_info_db_row->privilege_name));
				printf("conflict privilege name = %s\n", privilege_info_db_row2->privilege_name);
				printf("conflict privilege hash = %d\n", __privilege_checker_hash(privilege_info_db_row2->privilege_name));
				printf("-----------------------------------------------------------------------\n");
				fail_cnt++;
			}
		}
	}
	__free_privilege_list(privilege_list);

	__change_color_to_green();
	printf("test success\n");
	__change_color_to_origin();
	success_cnt++;
}

int main()
{
	printf("Test function : privilege_checker_hash\n");
	__test_hash_function();

	__change_color_to_green();
	printf("Test Complete\n");
	__change_color_to_red();
	printf("fail : %d\n", fail_cnt);
	__change_color_to_origin();
}

