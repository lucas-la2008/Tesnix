#ifndef __HANDLERS_TABLE_H__
#define __HANDLERS_TABLE_H__

#include <lib/kstdint.h>
#include <lib/kstddef.h>
#include "interrupts/common_handler.h"

extern void (*handlers[256])(registers_t*);

#endif