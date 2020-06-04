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
 * @file        class.h
 *
 */
#ifndef DX_IMPL_CLASS_H
#define DX_IMPL_CLASS_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "field.h"
#include "method.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex class header type
typedef struct __dx_class_header_t
{
    // the static fields count
    tb_uint32_t             static_fields_size;

    // the instance fields count
    tb_uint32_t             instance_fields_size;

    // the direct methods count
    tb_uint32_t             direct_methods_size;

    // the virtual methods count
    tb_uint32_t             virtual_methods_size;

}dx_class_header_t, *dx_class_header_ref_t;

// the dex class type: class_data_item
typedef struct __dx_class_t
{
    // the dex file 
    dx_file_t*              dexfile;

    // the header
    dx_class_header_t       header;

    // the class define
    dx_class_def_ref_t      class_def;

    // the static fields
    dx_field_t*             static_fields;

    // the instance fields
    dx_field_t*             instance_fields;

    // the direct methods
    dx_method_t*            direct_methods;

    // the virtual methods
    dx_method_t*            virtual_methods;

}dx_class_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


