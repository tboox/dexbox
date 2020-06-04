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
 * @file        catch.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "catch.h"
#include "leb128.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t dx_catch_init(dx_catch_ref_t trycatch, dx_file_ref_t file, dx_code_ref_t code, tb_uint32_t offset)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(trycatch && dexfile && dexcode, tb_false);

    // the data
    tb_byte_t const* data = dx_get_catch_handle_data(dexcode) + offset;

    // init stream
    if (!tb_static_stream_init(&trycatch->stream, (tb_byte_t*)data, dexfile->data + dexfile->size - data)) return tb_false;

    // get the catch count
    if (!dx_sleb128_read(&trycatch->stream, &trycatch->count_remaining)) return tb_false;

    // catches all?
    if (trycatch->count_remaining <= 0)
    {
        trycatch->catches_all      = tb_true;
        trycatch->count_remaining  = -trycatch->count_remaining;
    }
    else trycatch->catches_all = tb_false;

    // ok
    return tb_true;
}
dx_catch_handler_ref_t dx_catch_next(dx_catch_ref_t trycatch)
{
    // check
    tb_assert_and_check_return_val(trycatch, tb_null);

    // end?
    if (trycatch->count_remaining <= 0) 
    {
        // catches all?
        tb_check_return_val(trycatch->catches_all, tb_null);

        // clear it
        trycatch->catches_all = tb_false;
        trycatch->handler.type_idx = 0xffffffff;
    } 
    else
    {
        // get the catch type index
        if (!dx_uleb128_read(&trycatch->stream, &trycatch->handler.type_idx)) return tb_null;

        // next
        trycatch->count_remaining--;
    }

    // get the catch handler address
    if (!dx_uleb128_read(&trycatch->stream, &trycatch->handler.address)) return tb_null;

    // ok
    return &trycatch->handler;
}
tb_bool_t dx_catch_find(dx_catch_ref_t trycatch, dx_file_ref_t file, dx_code_ref_t code, tb_uint32_t address)
{
    // check
    tb_assert_and_check_return_val(trycatch && file && code, tb_false);

    // get the tries count
    tb_size_t tries_size = dx_code_tries_size(code);
    tb_check_return_val(tries_size, tb_false);

    // get tries data
    dx_try_ref_t tries = dx_code_tries(code);
    tb_assert_and_check_return_val(tries, tb_false);

    // find offset
    tb_uint32_t offset = -1;
    if (tries_size == 1)
    {
        // is this?
        tb_uint32_t start = tries[0].start_addr;
        tb_uint32_t end = start + tries[0].insn_count;
        tb_check_return_val(address >= start && address < end, tb_false); 

        // get offset
        offset = tries[0].handler_off;
    }
    else
    {
        // find it by the binary search
        tb_long_t min = 0;
        tb_long_t max = tries_size - 1;
        while (max >= min)
        {
            // guess it
            tb_long_t guess = (min + max) >> 1;

            // get try
            dx_try_ref_t ptry = &tries[guess];

            // left?
            tb_uint32_t start = ptry->start_addr;
            if (address < start) 
            {
                max = guess - 1;
                continue;
            }

            // right?
            tb_uint32_t end = start + ptry->insn_count;
            if (address >= end)
            {
                min = guess + 1;
                continue;
            }

            // found
            offset = ptry->handler_off;
            break;
        }
    }

    // init catch
    return offset != -1? dx_catch_init(trycatch, file, code, offset) : tb_false;
}
