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
    else if(ret == PRVMGR_ERR_INVALID_PARAMETER){
        return "PRVMGR_ERR_INVALID_PARAMETER";
    }
    else if(ret == PRVMGR_ERR_INTERNAL_ERROR){
        return "PRVMGR_ERR_INTERNAL_ERROR";
    }
    else if(ret == PRVMGR_ERR_INVALID_PRIVILEGE){
        return "PRVMGR_ERR_INVALID_PRIVILEGE";
    }
    else
    {
        return "FAIL";
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

static void __check_verify_result(privilege_manager_error_e expected_result, privilege_manager_error_e result)
{
    printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));
    if(expected_result != result)
    {
        printf("not matched\n");
        printf("expected result = %d , result = %d", expected_result, result);
        __change_color_to_red();
        printf("test fail\n");
        fail_cnt++;
    }else{
        printf("matched\n");
        __change_color_to_green();
        printf("test success\n");
            success_cnt++;
    }
    __change_color_to_origin();
}

static void __test_privilege_manager_verify_privilege()
{
    GList* privilege_list = NULL;
    char* error_message = NULL;
    int ret = 0;

    printf("-----------------------------------------------------------\n");
    printf("TEST FOR CORE PRIVILEGE\n");
    printf("-----------------------------------------------------------\n\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/healthinfo\n");
    printf("privilege name : http://tizen.org/privilege/led\n");
    printf("privilege name : http://tizen.org/privilege/se\n");
    printf("privilege version : 2.3.1\n");
    printf("signature level : public\n");
    printf("privilege level : public, public, x\n");
    printf("package type : core\n");
    printf("expected result : PRVMGR_ERR_INVALID_PRIVILEGE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/healthinfo");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/led");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/se");
    ret = privilege_manager_verify_privilege("2.3.1", PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_INVALID_PRIVILEGE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/haptic\n");
    printf("privilege name : http://tizen.org/privilege/led\n");
    printf("privilege name : http://tizen.org/privilege/keymanager.admin\n");
    printf("privilege version : 2.3.1\n");
    printf("signature level : public\n");
    printf("privilege level : public, public, platform\n");
    printf("package type : core\n");
    printf("expected result : PRVMGR_ERR_INVALID_PRIVILEGE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/haptic");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/led");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/keymanager.admin");
    ret = privilege_manager_verify_privilege("2.3.1", PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_INVALID_PRIVILEGE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/haptic\n");
    printf("privilege name : http://tizen.org/privilege/secureelement\n");
    printf("privilege name : http://tizen.org/privilege/keymanager\n");
    printf("privilege version : 2.3.1\n");
    printf("signature level : public\n");
    printf("privilege level : public, public, public\n");
    printf("package type : core\n");
    printf("expected result : PRVMGR_ERR_NONE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/haptic");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/secureelement");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/keymanager");
    ret = privilege_manager_verify_privilege("2.3.1", PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_NONE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/haptic\n");
    printf("privilege name : http://tizen.org/privilege/secureelement\n");
    printf("privilege name : http://tizen.org/privilege/keymanager\n");
    printf("privilege version : 2.3\n");
    printf("signature level : public\n");
    printf("privilege level : public, public, public\n");
    printf("package type : core\n");
    printf("expected result : PRVMGR_ERR_INVALID_PRIVILEGE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/haptic");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/secureelement");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/keymanager");
    ret = privilege_manager_verify_privilege("2.3", PRVMGR_PACKAGE_TYPE_CORE, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_INVALID_PRIVILEGE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n");

    printf("-----------------------------------------------------------\n");
    printf("TEST FOR WRT PRIVILEGE\n");
    printf("-----------------------------------------------------------\n\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/xxxx\n");
    printf("privilege name : http://tizen.org/privilege/telephony\n");
    printf("privilege name : http://tizen.org/privilege/contact\n");
    printf("privilege version : 2.3\n");
    printf("signature level : public\n");
    printf("privilege level : x, x, public\n");
    printf("package type : wrt\n");
    printf("expected result : PRVMGR_ERR_NONE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/xxxx");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/telephony");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/contact");
    ret = privilege_manager_verify_privilege("2.3", PRVMGR_PACKAGE_TYPE_WRT, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_NONE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/xxxx\n");
    printf("privilege name : http://tizen.org/privilege/telephony\n");
    printf("privilege name : http://tizen.org/privilege/systemmanager\n");
    printf("privilege version : 2.3\n");
    printf("signature level : public\n");
    printf("privilege level : x, x, partner\n");
    printf("package type : wrt\n");
    printf("expected result : PRVMGR_ERR_INVALID_PRIVILEGE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/xxxx");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/api/tizen");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/systemmanager");
    ret = privilege_manager_verify_privilege("2.3", PRVMGR_PACKAGE_TYPE_WRT, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_INVALID_PRIVILEGE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/xxxx\n");
    printf("privilege name : http://tizen.org/privilege/telephony\n");
    printf("privilege name : http://tizen.org/privilege/systemmanager\n");
    printf("privilege version : 2.3.1\n");
    printf("signature level : public\n");
    printf("privilege level : x, public, partner\n");
    printf("package type : wrt\n");
    printf("expected result : PRVMGR_ERR_INVALID_PRIVILEGE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/xxxx");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/telephony");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/systemmanager");
    ret = privilege_manager_verify_privilege("2.3.1", PRVMGR_PACKAGE_TYPE_WRT, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_INVALID_PRIVILEGE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/healthinfo\n");
    printf("privilege name : http://tizen.org/privilege/led\n");
    printf("privilege name : http://tizen.org/privilege/secureelement\n");
    printf("privilege version : 2.3.1\n");
    printf("signature level : public\n");
    printf("privilege level : public, public, public\n");
    printf("package type : wrt\n");
    printf("expected result : PRVMGR_ERR_INVALID_PRIVILEGE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/healthinfo");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/led");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/secureelement");
    ret = privilege_manager_verify_privilege("2.3.1", PRVMGR_PACKAGE_TYPE_WRT, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_INVALID_PRIVILEGE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n\n");

    g_list_free(privilege_list);
    privilege_list = NULL;
    printf("privilege name : http://tizen.org/privilege/healthinfo\n");
    printf("privilege name : http://tizen.org/privilege/led\n");
    printf("privilege name : http://tizen.org/privilege/secureelement\n");
    printf("privilege version : 2.3\n");
    printf("signature level : public\n");
    printf("privilege level : public, public, public\n");
    printf("package type : wrt\n");
    printf("expected result : PRVMGR_ERR_NONE\n");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/healthinfo");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/led");
    privilege_list = g_list_append(privilege_list, "http://tizen.org/privilege/secureelement");
    ret = privilege_manager_verify_privilege("2.3", PRVMGR_PACKAGE_TYPE_WRT, privilege_list, CERT_SVC_VISIBILITY_PUBLIC, &error_message);
    __check_verify_result(PRVMGR_ERR_NONE, ret);
    if(error_message != NULL)
    {
        printf("%s\n", error_message);
        free(error_message);
        error_message = NULL;
    }
    printf("-----------------------------------------------------------\n\n");

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
    g_list_free(privilege_list);
    privilege_list = NULL;
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
    printf("privilege name : http://tizen.org/privilege/bluetooth.adminTT\n");
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

int main()
{
    __change_color_to_yellow();
    printf("Test function : privilege_manager_verify_privilege_list\n");
    __change_color_to_origin();
    __test_privilege_manager_verify_privilege_list();

    __change_color_to_yellow();
    printf("Test function : privilege_manager_verify_privilege\n");
    __change_color_to_origin();
    __test_privilege_manager_verify_privilege();

    __change_color_to_green();
    printf("Test Complete\n");
    printf("success : %d, ", success_cnt);
    __change_color_to_red();
    printf("fail : %d\n", fail_cnt);
    __change_color_to_origin();

    return 0;
}
