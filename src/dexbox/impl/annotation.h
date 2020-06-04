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
 * @file        annotation.h
 *
 */
#ifndef DX_IMPL_ANNOTATION_H
#define DX_IMPL_ANNOTATION_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* get array size
 *
 * @param stream    the array stream
 *
 * @return          the array size
 */
tb_size_t           dx_annotation_array_size(tb_static_stream_ref_t stream);

/* get the next value 
 *
 * @param stream    the array stream
 * @param value     the value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           dx_annotation_array_next(tb_static_stream_ref_t stream, dx_value_ref_t value);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


