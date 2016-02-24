#include <privilege_checker.h>
#include <dlog.h>

void show(char* privilege_name, int ret)
{
	switch (ret)
	{
	case PRIVILEGE_CHECKER_ERR_NONE:
		LOGD("%s is declared in current application", privilege_name);
		break;
	case PRIVILEGE_CHECKER_ERR_UNDECLARED_PRIVILEGE:
		LOGD("%s is not declared in current application", privilege_name);
		break;
	case PRIVILEGE_CHECKER_ERR_INVALID_PARAMETER:
		LOGD("%s is invalid name", privilege_name);
		break;
	case PRIVILEGE_CHECKER_ERR_INTERNAL_ERROR:
	default:
		LOGD("Internal error");
		break;
	}

}

int privilege_api_account_read()
{
	char* privilege_name = "http://tizen.org/privilege/account.read";
	int ret = privilege_checker_check_privilege(privilege_name);
	show(privilege_name, ret);
	return ret;
}
