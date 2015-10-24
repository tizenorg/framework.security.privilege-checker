#include <privilege_checker.h>

static void __test_privilege_checker_check_privilege2()
{
    int ret = PRIVILEGE_CHECKER_ERR_NONE;
    ret = privilege_checker_check_privilege("http://tizen.org/privilege/account.read");
}
