/*!A lightweight dex file parsing library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2015-2020, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        code.h
 *
 */
#ifndef DX_IMPL_CODE_H
#define DX_IMPL_CODE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"
#include "../code.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex code type: code_item
typedef struct __dx_code_t
{
    // the registers size
    tb_uint16_t             registers_size;

    // the ins size
    tb_uint16_t             ins_size;

    // the outs size
    tb_uint16_t             outs_size;

    // the tries size
    tb_uint16_t             tries_size;

    // file offset to debug info stream
    tb_uint32_t             debug_info_off;

    // size of the insns array, in tb_uint16_t units
    tb_uint32_t             insns_size; 
    tb_uint16_t             insns[1];

    /* optional tb_uint16_t padding 
     * try_item[triesSize] 
     * uleb128 handlersSize 
     * catch_handler_item[handlersSize]
     */

}dx_code_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/* get the catch handler data from the given code
 * 
 * @param code      the code
 *
 * @return          the handler data
 */
static __tb_inline__ tb_byte_t const* dx_get_catch_handle_data(dx_code_t* code) 
{
    // get tries
    dx_try_ref_t tries = dx_code_tries((dx_code_ref_t)code);
    tb_assert_and_check_return_val(tries, tb_null);

    // get handler data
    return (tb_byte_t const*)&tries[code->tries_size];
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


