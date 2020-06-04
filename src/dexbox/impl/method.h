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
 * @file        method.h
 *
 */
#ifndef DX_IMPL_METHOD_H
#define DX_IMPL_METHOD_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"
#include "code.h"
#include "proto.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex class method type: encoded_method 
typedef struct __dx_method_t
{
    // the dex file
    dx_file_t*              dexfile;

    // index to a method_id_item
    tb_uint32_t             method_idx;  

    // the access flags
    tb_uint32_t             access_flags;

    // file offset to a code_item
    tb_uint32_t             code_off; 

    // the code
    dx_code_t               code;

    // the proto
    dx_proto_t              proto;

}dx_method_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */
static __tb_inline__ dx_code_t* dx_method_get_code(dx_file_t* dexfile, dx_method_t* method)
{
    // check
    tb_check_return_val(method->code_off, tb_null);

    // get it
    return (dx_code_t*)(dexfile->data + method->code_off);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


