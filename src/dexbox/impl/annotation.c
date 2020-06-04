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
 * @file        annotation.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "annotation.h"
#include "../leb128.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex encoded value type enum
typedef enum __dx_dex_value_type_e
{
    DX_DEX_VALUE_BYTE       = 0x00
,   DX_DEX_VALUE_SHORT      = 0x02
,   DX_DEX_VALUE_CHAR       = 0x03
,   DX_DEX_VALUE_INT        = 0x04
,   DX_DEX_VALUE_LONG       = 0x06
,   DX_DEX_VALUE_FLOAT      = 0x10
,   DX_DEX_VALUE_DOUBLE     = 0x11
,   DX_DEX_VALUE_STRING     = 0x17
,   DX_DEX_VALUE_TYPE       = 0x18
,   DX_DEX_VALUE_FIELD      = 0x19
,   DX_DEX_VALUE_METHOD     = 0x1a
,   DX_DEX_VALUE_ENUM       = 0x1b
,   DX_DEX_VALUE_ARRAY      = 0x1c
,   DX_DEX_VALUE_ANNOTATION = 0x1d
,   DX_DEX_VALUE_NULL       = 0x1e
,   DX_DEX_VALUE_BOOLEAN    = 0x1f
,   DX_DEX_VALUE_NONE       = 0xff  //< custom, invalid value

}dx_dex_value_type_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_sint32_t dx_annotation_value_read_s32(tb_byte_t const* p, tb_size_t bytes0)
{
    // check
    tb_assert(p && bytes0 < 4);

    // done
    tb_long_t i;
    tb_sint32_t val = 0;
    for (i = bytes0; i >= 0; --i)
        val = ((tb_uint32_t)val >> 8) | (((tb_sint32_t)*p++) << 24);
    val >>= (3 - bytes0) * 8;

    // ok?
    return val;
}
static tb_uint32_t dx_annotation_value_read_u32(tb_byte_t const* p, tb_size_t bytes0, tb_bool_t fill_on_right)
{
    // check
    tb_assert(p && bytes0 < 4);

    // done
    tb_long_t i;
    tb_uint32_t val = 0;
    for (i = bytes0; i >= 0; --i)
        val = (val >> 8) | (((tb_uint32_t)*p++) << 24);

    // fill on left?
    if (!fill_on_right) val >>= (3 - bytes0) * 8;

    // ok?
    return val;
}
static tb_sint64_t dx_annotation_value_read_s64(tb_byte_t const* p, tb_size_t bytes0)
{
    // check
    tb_assert(p && bytes0 < 8);

    // done
    tb_long_t i;
    tb_sint64_t val = 0;
    for (i = bytes0; i >= 0; --i)
        val = ((tb_uint64_t)val >> 8) | (((tb_sint64_t)*p++) << 56);
    val >>= (7 - bytes0) * 8;

    // ok
    return val;
}
static tb_uint64_t dx_annotation_value_read_u64(tb_byte_t const* p, tb_size_t bytes0, tb_bool_t fill_on_right)
{
    // check
    tb_assert(p && bytes0 < 8);

    // done
    tb_long_t   i;
    tb_uint64_t val = 0;
    for (i = bytes0; i >= 0; --i)
        val = (val >> 8) | (((tb_uint64_t)*p++) << 56);

    // fill on left?
    if (!fill_on_right) val >>= (7 - bytes0) << 3;

    // ok
    return val;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_size_t dx_annotation_array_size(tb_static_stream_ref_t stream)
{
    // check
    tb_assert(stream);

    // get size
    tb_uint32_t size = 0;
    return dx_uleb128_read(stream, &size)? size : 0;
}
tb_bool_t dx_annotation_array_next(tb_static_stream_ref_t stream, dx_value_ref_t value)
{
    // check
    tb_assert(stream && value);

    // check space
    if (!tb_static_stream_left(stream))
    {
        // abort
        tb_assertf(0, "cannot decode dex array value");
        return tb_false;
    }

    // the start and end position
    tb_byte_t const* p = tb_static_stream_pos(stream);
    tb_byte_t const* e = tb_static_stream_end(stream);
    tb_assert(p < e); tb_used(e);

    // the value type and width
    tb_byte_t value_type    = *p & 0x1f;
    tb_size_t value_arg     = *p >> 5;
    tb_size_t value_width   = value_arg + 1;
    p++;

    // save type
    value->type = value_type;

    // get data
    switch (value->type)
    {
    case DX_DEX_VALUE_BYTE:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 1);

            // read it
            value->type     = DX_VALUE_BYTE;
            value->u._byte  = (tb_sint8_t)dx_annotation_value_read_s32(p, value_arg);
        }
        break;
    case DX_DEX_VALUE_SHORT:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 2);

            // read it
            value->type     = DX_VALUE_SHORT;
            value->u._short = (tb_sint16_t)dx_annotation_value_read_s32(p, value_arg);
        }
        break;
    case DX_DEX_VALUE_CHAR:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 2);

            // read it
            value->type     = DX_VALUE_CHAR;
            value->u._char  = (tb_uint16_t)dx_annotation_value_read_u32(p, value_arg, tb_false);
        }
        break;
    case DX_DEX_VALUE_INT:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 4);

            // read it
            value->type     = DX_VALUE_INT;
            value->u._int   = dx_annotation_value_read_s32(p, value_arg);
        }
        break;
    case DX_DEX_VALUE_LONG:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 8);

            // read it
            value->type     = DX_VALUE_LONG;
            value->u._long  = dx_annotation_value_read_s64(p, value_arg);
        }
        break;
    case DX_DEX_VALUE_FLOAT:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 4);

            // read it
            value->type     = DX_VALUE_FLOAT;
            value->u._int   = dx_annotation_value_read_u32(p, value_arg, tb_true);
        }
        break;
    case DX_DEX_VALUE_DOUBLE:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 8);

            // read it
            value->type     = DX_VALUE_DOUBLE;
            value->u._long  = dx_annotation_value_read_u64(p, value_arg, tb_true);
        }
        break;
#if 0
    case DX_DEX_VALUE_STRING:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 4);

            // read it
            value->u._string_idx = dx_annotation_value_read_u32(p, value_arg, tb_false);
        }
        break;
    case DX_DEX_VALUE_FIELD:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 4);

            // read it
            value->u._field_idx = dx_annotation_value_read_u32(p, value_arg, tb_false);
        }
        break;
    case DX_DEX_VALUE_METHOD:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 4);

            // read it
            value->u._method_idx = dx_annotation_value_read_u32(p, value_arg, tb_false);
        }
        break;
    case DX_DEX_VALUE_ENUM:
        {
            // check
            tb_assert(p + value_width < e && value_width <= 4);

            // read it
            value->u._enum_field_idx = dx_annotation_value_read_u32(p, value_arg, tb_false);
        }
        break;
#else
    case DX_DEX_VALUE_STRING:
        value->type = DX_VALUE_OBJECT;
        break;
    case DX_DEX_VALUE_FIELD:
        value->type = DX_VALUE_OBJECT;
        break;
    case DX_DEX_VALUE_METHOD:
        value->type = DX_VALUE_OBJECT;
        break;
    case DX_DEX_VALUE_ENUM:
        value->type = DX_VALUE_OBJECT;
        break;
#endif
    case DX_DEX_VALUE_BOOLEAN:
        value->type = DX_VALUE_BOOLEAN;
        value->u._boolean = value_arg != 0;
        value_width = 0;
        break;
    case DX_DEX_VALUE_ARRAY:
    case DX_DEX_VALUE_ANNOTATION:
        tb_assert(0);
        break;
    case DX_DEX_VALUE_NULL:
        value->type = DX_VALUE_NULL;
        value_width = 0;
        break;
    default:
        {
            // abort
            tb_assertf(0, "invalid annotation value type: %x", value->type);

            // failed
            return tb_false;
        }
    }

    // skip value
    p += value_width;

    // ok?
    return tb_static_stream_goto(stream, (tb_byte_t*)p);
}

