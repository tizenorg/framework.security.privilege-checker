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

#ifndef __TIZEN_SECURITY_PRIVILEGE_CHECKER_EXTENSION_H
#define __TIZEN_SECURITY_PRIVILEGE_CHECKER_EXTENSION_H

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <tizen.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlog.h>

#define PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE "http://developer.samsung.com/tizen/privilege/"
#define PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE_LEN strlen(PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE)
#define PRIVILEGE_CHECKER_EXTENSION_PREFIX_LABEL "privilege::samsung::"
#define PRIVILEGE_CHECKER_EXTENSION_PREFIX_LABEL_LEN strlen(PRIVILEGE_CHECKER_EXTENSION_PREFIX_LABEL)
#define PRIVILEGE_CHECKER_IS_EXTENSION_PRIVILEGE(name) \
	( (strlen (name) < PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE_LEN) ? \
	0 : \
	!strncmp(PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE, name, PRIVILEGE_CHECKER_EXTENSION_PRIVILEGE_LEN) )

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SECURITY_PRIVILEGE_CHECKER_EXTENSION_H
