#ifndef _FLAGS_H
#define _FLAGS_H 1
// -----------------------------------------------------------------------------
// Copyright 2018 Stephen Stebbing. telecnatron.com
//
//    Licensed under the Telecnatron License, Version 1.0 (the “License”);
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://telecnatron.com/software/licenses/
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an “AS IS” BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// -----------------------------------------------------------------------------   
/**
 * @file   flags.h
 * @author steves
 * @date   2017/02/21 23:15:02
 * 
 * @brief  Definitions and descriptions of values for messages' FLAGS byte.
 * 
 * 
 */

#include "config.h"

// flag for bootloader messages are all zeros
#define MMP_FLAGS_BOOTLOADER 0x0
// bit indicates that message received by MCU is a command message,
// or message received by PC is a response to a command message.
#define MMP_FLAGS_BIT_CMD    0x1
// bit indicates that message received by PC is a async message.
#define MMP_FLAGS_BIT_ASYNC  0x2


// convenience macros
#define MMP_FLAGS_IS_BOOTLOADER_MSG(flags) ( !(flags && MMP_FLAGS_BOOTLOADER) )
#define MMP_FLAGS_IS_ASYNC_MSG(flags)      ( flags && MMP_FLAGS_BIT_ASYNC )
#define MMP_FLAGS_IS_CMD_MSG(flags)         ( flags && MMP_FLAGS_BIT_CMD )

#endif /* _FLAGS_H */

