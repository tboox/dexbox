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
 * @file        value.h
 *
 */
#ifndef DX_VALUE_H
#define DX_VALUE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the value type enum
typedef enum __dx_value_type_e
{
    DX_VALUE_NONE       = 0
,   DX_VALUE_BYTE       = 1
,   DX_VALUE_SHORT      = 2
,   DX_VALUE_CHAR       = 3
,   DX_VALUE_INT        = 4
,   DX_VALUE_LONG       = 5
,   DX_VALUE_FLOAT      = 6
,   DX_VALUE_DOUBLE     = 7
,   DX_VALUE_NULL       = 8
,   DX_VALUE_BOOLEAN    = 9
,   DX_VALUE_OBJECT     = 10

}dx_value_type_e;

/// the value type
typedef struct __dx_value_t
{
    /// the value type
    tb_uint8_t              type;

    /// the value
    union 
    {
        tb_sint8_t          _byte;
        tb_sint16_t         _short;
        tb_uint16_t         _char;
        tb_sint32_t         _int;
        tb_sint64_t         _long;
        tb_float_t          _float;
        tb_double_t         _double;
        tb_bool_t           _boolean;
        dx_object_ref_t     _object;
    
    }u;

}dx_value_t, *dx_value_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init value from the descriptor
 *
 * @param value         the value 
 * @param descriptor    the descriptor
 */
tb_void_t               dx_value_init(dx_value_ref_t value, tb_char_t const* descriptor);

/*! make c-string for the value 
 *
 * @param value         the value 
 * @param data          the string data
 * @param maxn          the string maxn
 *
 * @return              the string pointer
 */
tb_char_t const*        dx_value_cstr(dx_value_ref_t value, tb_char_t* data, tb_size_t maxn);

#ifdef DX_DUMP_ENABLE
/*! get the type name of the value 
 *
 * @param value         the value 
 *
 * @return              the type name
 */
tb_char_t const*        dx_value_typename(dx_value_ref_t value);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


