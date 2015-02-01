#include <stdio.h>
#include <privilege_manager.h>

#define BRIGHTNESS 0
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BG_BLACK 40

static int fail_cnt=0;
static int success_cnt=0;

static void __change_color_to_red(){
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, RED);
}

static void __change_color_to_green(){
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, GREEN);
}

static void __change_color_to_yellow(){
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, YELLOW);
}

static void __change_color_to_origin(){
	printf("%c[%dm", 0x1B, 0);
}
static const char* __get_result_string(privilege_manager_error_e ret){
	if(ret == PRVMGR_ERR_NONE){
		return "PRVMGR_ERR_NONE";
	}
	else if(ret == PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL){
		return "PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL";
	}
	else if(ret == PRVMGR_ERR_NO_EXIST_PRIVILEGE){
		return "PRVMGR_ERR_NO_EXIST_PRIVILEGE";
	}
	else if(ret == PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL){
		return "PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL";
	}
	else if(ret == PRVMGR_ERR_INVALID_PARAMETER){
		return "PRVMGR_ERR_INVALID_PARAMETER";
	}
	else if(ret == PRVMGR_ERR_INTERNAL_ERROR){
		return "PRVMGR_ERR_INTERNAL_ERROR";
	}
}

static void __check_result(privilege_manager_error_e expected_result, privilege_manager_error_e result, char* error_privilege_name)
{
	printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));

	if(expected_result != result)
	{
		printf("not matched\n");
		__change_color_to_red();
		printf("test fail\n");
		fail_cnt++;
	}else{
		printf("matched\n");
		if(result == PRVMGR_ERR_NO_EXIST_PRIVILEGE || result == PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL)
		{
			if(error_privilege_name == NULL){
				printf("error_privilege_name must not be NULL\n");
				__change_color_to_red();
				printf("test fail\n");
				fail_cnt++;
				__change_color_to_origin();
				return;
			}else{
				printf("error_privilege_name = %s\n", error_privilege_name);
			}
		}
		else{
			if(error_privilege_name != NULL){
				printf("error_privilege_name must be NULL\n");
				__change_color_to_red();
				printf("test fail\n");
				fail_cnt++;
				__change_color_to_origin();
				return;
			}
		}
		__change_color_to_green();
		printf("test success\n");
		success_cnt++;
	}
	__change_color_to_origin();
}

static void __test_privilege_manager_verify_privilege_list()
{
	GList* privilege_list = NULL;
	char* error_privilege_name = NULL;
	int ret;

	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("signature level : public\n");
	printf("privilege level : public\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name);
	__check_result(PRVMGR_ERR_NONE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : platform\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name);
	__check_result(PRVMGR_ERR_NONE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("signature level : platform\n");
	printf("privilege level : public\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name);
	__check_result(PRVMGR_ERR_NONE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : platform\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name);
	__check_result(PRVMGR_ERR_NONE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : public\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name);
	__check_result(PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
	printf("package type : core\n");
	printf("signature level : platform\n");
	printf("expected result : PRVMGR_ERR_NO_EXIST_PRIVILEGE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name);
	__check_result(PRVMGR_ERR_NO_EXIST_PRIVILEGE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
	printf("signature level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NO_EXIST_PRIVILEGE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name);
	__check_result(PRVMGR_ERR_NO_EXIST_PRIVILEGE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : public\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name);
	__check_result(PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : public\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name);
	__check_result(PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : platform\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NO_EXIST_PRIVILEGE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name);
	__check_result(PRVMGR_ERR_NO_EXIST_PRIVILEGE, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("test privilege list = NULL\n");
	printf("expected result : PRVMGR_ERR_INVALID_PARAMETER\n");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name);
	__check_result(PRVMGR_ERR_INVALID_PARAMETER, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);

	error_privilege_name = NULL;
	privilege_list = NULL;
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, NULL);
	printf("test privilege list[0] = http://tizen.org/privilege/account.read\n");
	printf("test privilege list[1] = NULL\n");
	printf("expected result : PRVMGR_ERR_INVALID_PARAMETER\n");
	ret = privilege_manager_verify_privilege_list(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name);
	__check_result(PRVMGR_ERR_INVALID_PARAMETER, ret, error_privilege_name);
	printf("-----------------------------------------------------------\n");
    free(error_privilege_name);
}


static void __check_privilege_manager_verify_privilege_name_result(privilege_manager_error_e expected_result, privilege_manager_error_e result, GList* error_privilege_list)
{
	printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));

	if(expected_result != result)
	{
		printf("not matched\n");

		__change_color_to_red();
		printf("test fail\n");
		fail_cnt++;
	}else{
		printf("matched\n");

		GList* l;
		int i=0;
		for (l = error_privilege_list; l != NULL; l = l->next)
		{
			char* error_privilege_name = (char*)l->data;
			printf("error_privilege_name[%d] = %s\n", i, error_privilege_name);
			i++;
		}
		__change_color_to_green();
		printf("test success\n");
		success_cnt++;
	}
	__change_color_to_origin();
}


static void __test_privilege_manager_verify_privilege_name()
{
	GList* privilege_list = NULL;
	GList* error_privilege_list = NULL;
	int ret;

	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NONE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/bluetooth\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NONE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NONE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NONE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
    printf("privilege name : http://tizen.org/privilege/bluetooth.adsdfsdfsdfTTT\n");
    printf("privilege name : http://tizen.org/privilege/shosdfsdfsfc.dfsdfTTT\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NO_EXIST_PRIVILEGE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adsdfsdfsdfTTT");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/shosdfsdfsfc.dfsdfTTT");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NO_EXIST_PRIVILEGE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");


	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NO_EXIST_PRIVILEGE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NO_EXIST_PRIVILEGE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.adminTTTTT\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NO_EXIST_PRIVILEGE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.adminTTTTT");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_NO_EXIST_PRIVILEGE, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("test privilege list = NULL\n");
	printf("expected result : PRVMGR_ERR_INVALID_PARAMETER\n");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_INVALID_PARAMETER, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");

	if(error_privilege_list != NULL)
    {
        g_list_free(error_privilege_list);
        error_privilege_list = NULL;
    }
	privilege_list = NULL;
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, NULL);
	printf("test privilege list[0] = http://tizen.org/privilege/account.read\n");
	printf("test privilege list[1] = NULL\n");
	printf("expected result : PRVMGR_ERR_INVALID_PARAMETER\n");
	ret = privilege_manager_verify_privilege_name(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, &error_privilege_list);
	__check_privilege_manager_verify_privilege_name_result(PRVMGR_ERR_INVALID_PARAMETER, ret, error_privilege_list);
	printf("-----------------------------------------------------------\n");
    g_list_free(error_privilege_list);
}



static void __check_privilege_manager_verify_privilege_level_result(privilege_manager_error_e expected_result, privilege_manager_error_e result, GList* error_privilege_name_list, GList* error_privilege_level_list)
{
	printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));

	if(expected_result != result)
	{
		printf("not matched\n");

		__change_color_to_red();
		printf("test fail\n");
		fail_cnt++;
	}else{
		printf("matched\n");

		GList* l;
		int i=0;
		for (l = error_privilege_name_list; l != NULL; l = l->next)
		{
			char* error_privilege_name = (char*)l->data;
			printf("error_privilege_name[%d] = %s\n", i, error_privilege_name);
			i++;
		}
		i=0;
		for (l = error_privilege_level_list; l != NULL; l = l->next)
		{
			char* error_privilege_level = (char*)l->data;
			printf("error_privilege_level[%d] = %s\n", i, error_privilege_level);
			i++;
		}
		__change_color_to_green();
		printf("test success\n");
		success_cnt++;
	}
	__change_color_to_origin();
}


static void __test_privilege_manager_verify_privilege_level()
{
	GList* privilege_list = NULL;
	GList* error_privilege_name_list = NULL;
	GList* error_privilege_level_list = NULL;
	int ret;

	error_privilege_name_list = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/badge\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth\n");
	printf("signature level : public\n");
	printf("privilege level : public\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/badge");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth");
	ret = privilege_manager_verify_privilege_level(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name_list, &error_privilege_level_list);
	__check_privilege_manager_verify_privilege_level_result(PRVMGR_ERR_NONE, ret, error_privilege_name_list, error_privilege_level_list);
	printf("-----------------------------------------------------------\n");
    g_list_free(error_privilege_name_list);
    g_list_free(error_privilege_level_list);

    error_privilege_level_list = NULL;
	error_privilege_name_list = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : public\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_level(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_privilege_name_list, &error_privilege_level_list);
	__check_privilege_manager_verify_privilege_level_result(PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL, ret, error_privilege_name_list, error_privilege_level_list);
	printf("-----------------------------------------------------------\n");
    g_list_free(error_privilege_name_list);
    g_list_free(error_privilege_level_list);

    error_privilege_level_list = NULL;
	error_privilege_name_list = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : partner\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_level(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PARTNER, &error_privilege_name_list, &error_privilege_level_list);
	__check_privilege_manager_verify_privilege_level_result(PRVMGR_ERR_MISMACHED_PRIVILEGE_LEVEL, ret, error_privilege_name_list, error_privilege_level_list);
	printf("-----------------------------------------------------------\n");
    g_list_free(error_privilege_name_list);
    g_list_free(error_privilege_level_list);

    error_privilege_level_list = NULL;
	error_privilege_name_list = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : platform\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_level(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PLATFORM, &error_privilege_name_list, &error_privilege_level_list);
	__check_privilege_manager_verify_privilege_level_result(PRVMGR_ERR_NONE, ret, error_privilege_name_list, error_privilege_level_list);
	printf("-----------------------------------------------------------\n");
    g_list_free(error_privilege_name_list);
    g_list_free(error_privilege_level_list);

    error_privilege_level_list = NULL;
	error_privilege_name_list = NULL;
	g_list_free(privilege_list);
	privilege_list = NULL;
	printf("privilege name : http://tizen.org/privilege/account.read\n");
	printf("privilege name : http://tizen.org/privilege/bluetooth.admin\n");
	printf("signature level : 1 << 1\n");
	printf("privilege level : platform\n");
	printf("package type : core\n");
	printf("expected result : PRVMGR_ERR_NONE\n");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/account.read");
	privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/bluetooth.admin");
	ret = privilege_manager_verify_privilege_level(PRVMGR_PACKAGE_TYPE_CORE, privilege_list, 1<<1, &error_privilege_name_list, &error_privilege_level_list);
	__check_privilege_manager_verify_privilege_level_result(PRVMGR_ERR_INVALID_PARAMETER, ret, error_privilege_name_list, error_privilege_level_list);
	printf("-----------------------------------------------------------\n");
    g_list_free(error_privilege_name_list);
    g_list_free(error_privilege_level_list);
}

int main()
{
	__change_color_to_yellow();
	printf("Test function : privilege_manager_verify_privilege_list\n");
	__change_color_to_origin();
	__test_privilege_manager_verify_privilege_list();

	__change_color_to_yellow();
	printf("Test function : privilege_manager_verify_privilege_name\n");
	__change_color_to_origin();
	__test_privilege_manager_verify_privilege_name();

	__change_color_to_yellow();
	printf("Test function : privilege_manager_verify_privilege_level\n");
	__change_color_to_origin();
	__test_privilege_manager_verify_privilege_level();

	__change_color_to_green();
	printf("Test Complete\n");
	printf("success : %d, ", success_cnt);
	__change_color_to_red();
	printf("fail : %d\n", fail_cnt);
	__change_color_to_origin();
}
