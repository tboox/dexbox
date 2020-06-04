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
 * @file        code.h
 *
 */
#ifndef DX_CODE_H
#define DX_CODE_H

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

/* get the tries list from the given code
 * 
 * @param code          the dex code 
 *
 * @return              the tries list
 */
dx_try_ref_t            dx_code_tries(dx_code_ref_t code);

/*! the tries size
 *
 * @param code          the dex code 
 *
 * @return              the tries size
 */
tb_size_t               dx_code_tries_size(dx_code_ref_t code);

/*! the register size
 *
 * @param code          the dex code 
 *
 * @return              the register size
 */
tb_size_t               dx_code_register_size(dx_code_ref_t code);

/*! the number of words of incoming arguments
 *
 * @param code          the dex code 
 *
 * @return              the register size
 */
tb_size_t               dx_code_incoming_size(dx_code_ref_t code);

/*! the number of words of outgoing arguments
 *
 * @param code          the dex code 
 *
 * @return              the register size
 */
tb_size_t               dx_code_outgoing_size(dx_code_ref_t code);

/*! the instruction size
 *
 * @param code          the dex code 
 *
 * @return              the instruction size
 */
tb_size_t               dx_code_instr_size(dx_code_ref_t code);

/*! the instruction data
 *
 * @param code          the dex code 
 *
 * @return              the instruction data
 */
tb_uint16_t const*      dx_code_instr_data(dx_code_ref_t code);

#ifdef DX_DUMP_ENABLE
/*! dump code
 *
 * @param code          the dex code 
 * @param file          the dex file
 */
tb_void_t               dx_code_dump(dx_code_ref_t code, dx_file_ref_t file);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


