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
 * @file        proto.h
 *
 */
#ifndef DX_IMPL_PROTO_H
#define DX_IMPL_PROTO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex class method proto type
typedef struct __dx_proto_t
{
    // the dex file
    dx_file_t*              dexfile;

    // index to a proto_id_item
    tb_uint32_t             proto_idx;  

    // the parameters
    dx_type_list_ref_t      parameters;

}dx_proto_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


