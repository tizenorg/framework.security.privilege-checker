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
	if(ret == PRIVILEGE_CHECKER_ERR_NONE){
		return "PRIVILEGE_CHECKER_ERR_NONE";
	}
	else if(ret == PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER){
		return "PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER";
	}
	else if(ret == PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR){
		return "PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR";
	}
	else if(ret == PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE){
		return "PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE";
	}
    else
    {
        return "FAIL";
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

static void __check_smack_result(int result)
{
	int expected_result = 1;
	printf("expected result = 1, result = %d\n", result);

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

static void __test_privilege_checker_check_privilege()
{
	int ret = PRIVILEGE_CHECKER_ERR_NONE;

	printf("privilege : http://tizen.org/privilege/account.read\n");
	printf("expected result : PRIVILEGE_CHECKER_ERR_NONE\n");
	ret = privilege_checker_check_privilege("http://tizen.org/privilege/account.read");
	__check_result(PRIVILEGE_CHECKER_ERR_NONE, ret);
	printf("-----------------------------------------------------------\n");

	printf("privilege : http://tizen.org/privilege/contact.read\n");
	printf("expected result : PRIVILEGE_CHECKER_ERR_NONE\n");
	ret = privilege_checker_check_privilege("http://tizen.org/privilege/contact.read");
	__check_result(PRIVILEGE_CHECKER_ERR_NONE, ret);
	printf("-----------------------------------------------------------\n");

}

static void __test_privilege_checker_check_single_install_privilege(char* privilege_name, char* object)
{
	int ret = 1;
	const char* subject = "org.tizen.test-privilege-checker";
	printf("privilege : %s\n", privilege_name);
	printf("%s %s %s\n", subject, object, "rw");
	ret = smack_have_access (subject, object, PRIVILEGE_CHECKER_RESERVED_PERMISSION);
	__check_smack_result(ret);
}


static void __test_privilege_checker_check_install_privilege()
{
	__test_privilege_checker_check_single_install_privilege("http://tizen.org/privilege/account.read", "privilege::tizen::account.read");
	__test_privilege_checker_check_single_install_privilege("http://tizen.org/privilege/contact.read", "privilege::tizen::contact.read");
}

int main()
{
	printf("Test function : privilege_checker_check_privilege\n");
	__test_privilege_checker_check_privilege();
	printf("Test function : privilege_checker_install_privilege\n");
	__test_privilege_checker_check_install_privilege();

	__change_color_to_green();
	printf("Test Complete\n");
	printf("success : %d, ", success_cnt);
	__change_color_to_red();
	printf("fail : %d\n", fail_cnt);
	__change_color_to_origin();

    return 0;
}

