#include <stdio.h>
#include <dlog.h>
#include <privilege_checker.h>

#define BRIGHTNESS 0
#define RED 31
#define GREEN 32
#define BG_BLACK 40

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

static const char* __get_result_string(privilege_checker_error_e ret){
    if(ret == PRIV_CHECKER_ERR_NONE){
        return "PRIV_CHECKER_ERR_NONE";
    }
    else if(ret == PRIV_CHECKER_ERR_INVALID_PARAMETER){
        return "PRIV_CHECKER_ERR_INVALID_PARAMETER";
    }
    else if(ret == PRIV_CHECKER_ERR_OUT_OF_MEMORY){
        return "PRIV_CHECKER_ERR_OUT_OF_MEMORY";
    }
    else if(ret == PRIV_CHECKER_ERR_INTERNAL_ERROR){
        return "PRIV_CHECKER_ERR_INTERNAL_ERROR";
    }
    else if(ret == PRIV_CHECKER_ERR_NO_DATA){
        return "PRIV_CHECKER_ERR_NO_DATA";
    }
    else if(ret == PRIV_CHECKER_ERR_INVALID_PRIVILEGE){
        return "PRIV_CHECKER_ERR_INVALID_PRIVILEGE";
    }
}

static void __check_result(privilege_checker_error_e expected_result, privilege_checker_error_e result)
{
    printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));

    if(expected_result != result){
        __change_color_to_red();
        printf("test fail\n");
        fail_cnt++;
    }
    else{
        __change_color_to_green();
        printf("test success\n");
        success_cnt++;
    }
    __change_color_to_origin();
}


static void __test_privilege_checker_check_package_privilege()
{
/*    int ret;

    printf("common\n");
    printf("package_id : PACKAGE_ID_YOU_WANT_TO_CHECK_PRIVILEGE\n");
    printf("package_name : http://tizen.org/privilege/account.read\n");
    printf("expected result : PRIV_CHECKER_ERR_NONE\n");
    ret = privilege_checker_check_package_privilege("PACKAGE_ID_YOU_WANT_TO_CHECK_PRIVILEGE", "http://tizen.org/privilege/account.read");
    __check_result(PRIV_CHECKER_ERR_NONE, ret);
    printf("-----------------------------------------------------------\n");

    printf("package_id : org.tizen.setting-location\n");
    printf("package_name : http://tizen.org/privilege/xxxxx\n");
    printf("expected result : PRIV_CHECKER_ERR_INVALID_PRIVILEGE\n");
    ret = privilege_checker_check_package_privilege("PACKAGE_ID_YOU_WANT_TO_CHECK_PRIVILEGE", "http://tizen.org/privilege/xxxxx");
    __check_result(PRIV_CHECKER_ERR_INVALID_PRIVILEGE, ret);
    printf("-----------------------------------------------------------\n");*/
}

int main()
{
    printf("Test function : privilege_checker_check_package_privilege\n");
    __test_privilege_checker_check_package_privilege();

    __change_color_to_green();
    printf("Test Complete\n");
    printf("success : %d, ", success_cnt);
    __change_color_to_red();
    printf("fail : %d\n", fail_cnt);
    __change_color_to_origin();
}

