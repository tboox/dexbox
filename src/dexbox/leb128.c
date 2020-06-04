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
 * @file        leb128.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "leb128"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "leb128.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
tb_bool_t dx_uleb128_read(tb_static_stream_ref_t stream, tb_uint32_t* pvalue)
{
    // check
    tb_assert(stream && pvalue);

    // the start and end pointer
    tb_byte_t const* p = tb_static_stream_pos(stream);
    tb_byte_t const* e = tb_static_stream_end(stream);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // check
        tb_check_break(p < e);

        // read it
        tb_int32_t value = *p++;
        if (value > 0x7f) 
        {
            // check
            tb_check_break(p < e);

            // read it
            tb_int32_t tmp = *p++;
            value = (value & 0x7f) | ((tmp & 0x7f) << 7);
            if (tmp > 0x7f) 
            {
                // check
                tb_check_break(p < e);

                // read it
                tmp = *p++;
                value |= (tmp & 0x7f) << 14;
                if (tmp > 0x7f) 
                {
                    // check
                    tb_check_break(p < e);

                    // read it
                    tmp = *p++;
                    value |= (tmp & 0x7f) << 21;
                    if (tmp > 0x7f)
                    {
                        // check
                        tb_check_break(p < e);

                        // read it
                        tmp = *p++;
                        value |= tmp << 28;

                        // check
                        tb_assert(tmp <= 0x0f);
                        tb_check_break(tmp <= 0x0f);
                    }
                }
            }
        }

        // update stream
        if (!tb_static_stream_goto(stream, (tb_byte_t*)p)) break;

        // check
        tb_assert(value >= 0);
        tb_check_break(value >= 0);

        // save value
        if (pvalue) *pvalue = (tb_uint32_t)value;

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
tb_bool_t dx_sleb128_read(tb_static_stream_ref_t stream, tb_sint32_t* pvalue)
{
    // check
    tb_assert(stream && pvalue);

    // the start and end pointer
    tb_byte_t const* p = tb_static_stream_pos(stream);
    tb_byte_t const* e = tb_static_stream_end(stream);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // check
        tb_check_break(p < e);

        // read it
        tb_int32_t value = *p++;
        if (value <= 0x7f) value = (value << 25) >> 25;
        else 
        {
            // read it
            tb_int32_t tmp = *p++;
            value = (value & 0x7f) | ((tmp & 0x7f) << 7);
            if (tmp <= 0x7f) value = (value << 18) >> 18;
            else 
            {
                // check
                tb_check_break(p < e);

                // read it
                tmp = *p++;
                value |= (tmp & 0x7f) << 14;
                if (tmp <= 0x7f) value = (value << 11) >> 11;
                else
                {   
                    // check
                    tb_check_break(p < e);

                    // read it
                    tmp = *p++;
                    value |= (tmp & 0x7f) << 21;
                    if (tmp <= 0x7f) value = (value << 4) >> 4;
                    else
                    { 
                        // check
                        tb_check_break(p < e);

                        // read it
                        tmp = *p++;
                        value |= tmp << 28;

                        // check
                        tb_assert(tmp <= 0x0f);
                        tb_check_break(tmp <= 0x0f);
                    }
                }
            }
        }

        // update stream
        if (!tb_static_stream_goto(stream, (tb_byte_t*)p)) break;

        // save value
        if (pvalue) *pvalue = (tb_uint32_t)value;

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
tb_bool_t dx_uleb128_writ(tb_static_stream_ref_t stream, tb_uint32_t value)
{
    // check
    tb_assert(stream);

    // done
    while (1) 
    {
        tb_byte_t tmp = value & 0x7f;
        if (tmp != value) 
        {
            if (!tb_static_stream_writ_u8(stream, tmp | 0x80)) return tb_false;
            value >>= 7;
        } 
        else
        {
            if (!tb_static_stream_writ_u8(stream, tmp)) return tb_false;
            break;
        }
    }

    // ok
    return tb_true;
}
tb_bool_t dx_sleb128_writ(tb_static_stream_ref_t stream, tb_sint32_t value)
{
    // noimpl
    tb_trace_noimpl();
    return tb_false;
}
tb_size_t dx_uleb128_size(tb_uint32_t value)
{
    // done
    tb_size_t count = 0;
    do 
    {
        value >>= 7;
        count++;

    } while (value);

    // get count
    return count;
}
