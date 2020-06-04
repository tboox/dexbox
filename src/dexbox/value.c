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
 * @file        value.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "value.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t dx_value_init(dx_value_ref_t value, tb_char_t const* descriptor)
{
    // check
    tb_assert_and_check_return(value && descriptor);

    // not primary type?
    if (descriptor[1])
    {
        value->type = DX_VALUE_NONE;
        return ;
    }

    // get type
    tb_char_t type = descriptor[0];

    // init it
    if (type == 'I')
    {
        value->type = DX_VALUE_INT;
        value->u._int = 0;
    }
    else if (type == 'Z')
    {
        value->type = DX_VALUE_BOOLEAN;
        value->u._boolean = tb_false;
    }
    else if (type == 'L')
    {
        value->type = DX_VALUE_OBJECT;
        value->u._object = tb_null;
    }
    else if (type == 'J')
    {
        value->type = DX_VALUE_LONG;
        value->u._long = 0;
    }
    else if (type == 'B')
    {
        value->type = DX_VALUE_BYTE;
        value->u._byte = 0;
    }
    else if (type == 'S')
    {
        value->type = DX_VALUE_SHORT;
        value->u._short = 0;
    }
    else if (type == 'C')
    {
        value->type = DX_VALUE_CHAR;
        value->u._char = 0;
    }
    else if (type == 'F')
    {
        value->type = DX_VALUE_FLOAT;
        value->u._float = 0;
    }
    else if (type == 'D')
    {
        value->type = DX_VALUE_DOUBLE;
        value->u._double = 0;
    }
    else value->type = DX_VALUE_NONE;
}
tb_char_t const* dx_value_cstr(dx_value_ref_t value, tb_char_t* data, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(value && data && maxn, tb_null);

    // done 
    tb_long_t size = 0;
    switch (value->type)
    {
    case DX_VALUE_BYTE:
        size = tb_snprintf(data, maxn, "%d", value->u._byte);
        break;
    case DX_VALUE_SHORT:
        size = tb_snprintf(data, maxn, "%d", value->u._short);
        break;
    case DX_VALUE_CHAR:
        size = tb_snprintf(data, maxn, "%c", (tb_char_t)value->u._char);
        break;
    case DX_VALUE_INT:
        size = tb_snprintf(data, maxn, "%d", value->u._int);
        break;
    case DX_VALUE_LONG:
        size = tb_snprintf(data, maxn, "%lld", (tb_char_t)value->u._long);
        break;
    case DX_VALUE_FLOAT:
        size = tb_snprintf(data, maxn, "%f", value->u._float);
        break;
    case DX_VALUE_DOUBLE:
        size = tb_snprintf(data, maxn, "%lf", value->u._double);
        break;
    case DX_VALUE_BOOLEAN:
        size = tb_snprintf(data, maxn, "%s", value->u._boolean? "true" : "false");
        break;
    case DX_VALUE_NULL:
        size = tb_snprintf(data, maxn, "null");
        break;
    case DX_VALUE_OBJECT:
        size = tb_snprintf(data, maxn, "<%p>", value->u._object);
        break;
    case DX_VALUE_NONE:
        break;
    default:
        {
            // abort
            tb_assertf(0, "invalid value type: %x", value->type);
        }
    }

    // end
    if (size >= 0 && size < maxn)
        data[size] = '\0';

    // ok?
    return size > 0? data : tb_null;
}
#ifdef DX_DUMP_ENABLE
tb_char_t const* dx_value_typename(dx_value_ref_t value)
{
    // check
    tb_assert_and_check_return_val(value, tb_null);

    // the names
    static tb_char_t const* s_names[] =
    {
        "none"
    ,   "byte"
    ,   "short"
    ,   "char"
    ,   "int"
    ,   "long"
    ,   "float"
    ,   "double"
    ,   "null"
    ,   "boolean"
    ,   "object"
    };
    tb_assert_and_check_return_val(value->type < tb_arrayn(s_names), tb_null);

    // ok?
    return s_names[value->type];
}
#endif


