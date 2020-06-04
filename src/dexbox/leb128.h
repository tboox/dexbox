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
 * @file        leb128.h
 *
 */
#ifndef DX_LEB128_H
#define DX_LEB128_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! read unsigned leb128 value
 *
 * @param stream    the stream
 * @param pvalue    the value pointer
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           dx_uleb128_read(tb_static_stream_ref_t stream, tb_uint32_t* pvalue);

/*! read signed leb128 value
 *
 * @param stream    the stream
 * @param pvalue    the value pointer
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           dx_sleb128_read(tb_static_stream_ref_t stream, tb_sint32_t* pvalue);

/*! write unsigned leb128 value
 *
 * @param stream    the stream
 * @param value     the value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           dx_uleb128_writ(tb_static_stream_ref_t stream, tb_uint32_t value);

/*! write signed leb128 value
 *
 * @param stream    the stream
 * @param value     the value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           dx_sleb128_writ(tb_static_stream_ref_t stream, tb_sint32_t value);

/*! get the number of bytes needed to encode value to uleb128 
 *
 * @param value     the value
 *
 * @return          the bytes
 */
tb_size_t           dx_uleb128_size(tb_uint32_t value);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


