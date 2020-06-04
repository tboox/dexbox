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
 * @file        field.h
 *
 */
#ifndef DX_FIELD_H
#define DX_FIELD_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"
#include "value.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get the field name
 *
 * @param field         the dex field 
 *
 * @return              the field name
 */
tb_char_t const*        dx_field_name(dx_field_ref_t field);

/*! get the field descriptor
 *
 * @param field         the dex field 
 *
 * @return              the field descriptor
 */
tb_char_t const*        dx_field_descriptor(dx_field_ref_t field);

/*! get the field access flags
 *
 * @param field         the dex field 
 *
 * @return              the field access flags
 */
tb_size_t               dx_field_access(dx_field_ref_t field);

/*! get the field value
 *
 * @param field         the dex field 
 *
 * @return              the field value
 */
dx_value_ref_t          dx_field_value(dx_field_ref_t field);

/*! set the field value
 *
 * @param field         the dex field 
 * @param value         the field value
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               dx_field_value_set(dx_field_ref_t field, dx_value_ref_t value);

#ifdef DX_DUMP_ENABLE
/*! dump field
 *
 * @param field         the dex field 
 */
tb_void_t               dx_field_dump(dx_field_ref_t field);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


