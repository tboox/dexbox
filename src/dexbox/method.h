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
 * @file        method.h
 *
 */
#ifndef DX_METHOD_H
#define DX_METHOD_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "code.h"
#include "class.h"
#include "proto.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get the method name
 *
 * @param method        the dex method 
 *
 * @return              the method name
 */
tb_char_t const*        dx_method_name(dx_method_ref_t method);

/*! get the method descriptor
 *
 * @param method        the dex method 
 * @param descriptor    the method descriptor
 *
 * @return              the method descriptor
 */
tb_char_t const*        dx_method_descriptor(dx_method_ref_t method, tb_string_ref_t descriptor);

/*! get the method class descriptor
 *
 * @param method        the dex method 
 *
 * @return              the method name
 */
tb_char_t const*        dx_method_class_descriptor(dx_method_ref_t method);

/*! get the method code
 *
 * @param method        the dex method 
 *
 * @return              the method code
 */
dx_code_ref_t           dx_method_code(dx_method_ref_t method);

/*! get the method proto
 *
 * @param method        the dex method 
 *
 * @return              the method proto
 */
dx_proto_ref_t          dx_method_proto(dx_method_ref_t method);

/*! get the method access flags
 *
 * @param method        the dex method 
 *
 * @return              the method access flags
 */
tb_size_t               dx_method_access(dx_method_ref_t method);

/*! get the dex file
 *
 * @param method        the dex method 
 *
 * @return              the dex file
 */
dx_file_ref_t           dx_method_dexfile(dx_method_ref_t method);

#ifdef DX_DUMP_ENABLE
/*! dump method
 *
 * @param method        the dex method 
 */
tb_void_t               dx_method_dump(dx_method_ref_t method);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


