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
 * @file        catch.h
 *
 */
#ifndef DX_CATCH_H
#define DX_CATCH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "code.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex catch handler type
typedef struct __dx_catch_handler_t
{
    // the type index of the caught exception type
    tb_uint32_t             type_idx;

    // the handler address
    tb_uint32_t             address;

}dx_catch_handler_t, *dx_catch_handler_ref_t;

// the dex catch type
typedef struct __dx_catch_t
{
    // the encoded data stream
    tb_static_stream_t      stream;

    // catches all?
    tb_bool_t               catches_all;

    // the handler
    dx_catch_handler_t      handler;

    // the remaining count
    tb_sint32_t             count_remaining;

}dx_catch_t, *dx_catch_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init catch 
 *
 * @param trycatch          the catch
 * @param file              the dex file
 * @param code              the dex code
 * @param offset            the catch offset
 */
tb_bool_t                   dx_catch_init(dx_catch_ref_t trycatch, dx_file_ref_t file, dx_code_ref_t code, tb_uint32_t offset);

/* get the next catch handler
 *
 * @param trycatch          the catch
 *
 * @return                  the handler
 */
dx_catch_handler_ref_t      dx_catch_next(dx_catch_ref_t trycatch);

/* find the catch handler from the given address
 *
 * @param trycatch          the catch
 * @param file              the dex file
 * @param code              the dex code
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   dx_catch_find(dx_catch_ref_t trycatch, dx_file_ref_t file, dx_code_ref_t code, tb_uint32_t address);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


