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
 * @file        field.h
 *
 */
#ifndef DX_IMPL_FIELD_H
#define DX_IMPL_FIELD_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"
#include "annotation.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex class field type: encoded_field 
typedef struct __dx_field_t
{
    // the dex file
    dx_file_t*              dexfile;

    // index to a field_id_item
    tb_uint32_t             field_idx; 

    // the access flags
    tb_uint32_t             access_flags;

    // the field value
    dx_value_t              value;

}dx_field_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


