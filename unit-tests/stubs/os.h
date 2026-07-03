#pragma once

#include <stdio.h>

#include "cx.h"
#include "os_print.h"
#include "os_pic.h"
#include "os_lib.h"

typedef uint32_t bolos_err_t;
#define SWO_OK 0x0000

void os_sched_exit(int status);
