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
 * @file        proto.h
 *
 */
#ifndef DX_PROTO_H
#define DX_PROTO_H

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

/// the dex proto ref type
typedef __dx_typeref__(proto);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get the count of parameters 
 *
 * @param proto         the dex proto 
 *
 * @return              the count of parameters 
 */
tb_size_t               dx_proto_param_size(dx_proto_ref_t proto);

/*! get the descriptor of parameter 
 *
 * @param proto         the dex proto 
 * @param index         the index of parameters
 *
 * @return              the descriptor of parameter 
 */
tb_char_t const*        dx_proto_param_descriptor(dx_proto_ref_t proto, tb_size_t index);

/*! get the descriptor of return value 
 *
 * @param proto         the dex proto 
 *
 * @return              the descriptor of return value 
 */
tb_char_t const*        dx_proto_retval_descriptor(dx_proto_ref_t proto);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


