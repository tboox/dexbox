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
 * @file        file.h
 *
 */
#ifndef DX_FILE_H
#define DX_FILE_H

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

/*! load dex file from the url
 *
 * @param url           the url
 * @param checksum      need verify checksum?
 *
 * @return              the dex file 
 */
dx_file_ref_t           dx_file_load_from_url(tb_char_t const* url, tb_bool_t checksum);

/*! load dex file from the data
 *
 * @param data          the data
 * @param size          the size
 * @param checksum      need verify checksum?
 *
 * @return              the dex file 
 */
dx_file_ref_t           dx_file_load_from_data(tb_byte_t const* data, tb_size_t size, tb_bool_t checksum);

/*! exit the dex file
 *
 * @param file          the dex file 
 */
tb_void_t               dx_file_exit(dx_file_ref_t file);

/*! the dex data
 *
 * @param file          the dex file 
 *
 * @return              the dex data
 */
tb_byte_t const*        dx_file_data(dx_file_ref_t file);

/*! the dex size
 *
 * @param file          the dex file 
 *
 * @return              the dex size
 */
tb_size_t               dx_file_size(dx_file_ref_t file);

/*! get type
 *
 * @param file          the dex file
 * @param type_idx      the type index
 *
 * @return              the type
 */
tb_char_t const*        dx_file_type(dx_file_ref_t file, tb_size_t type_idx);

/*! get string
 *
 * @param file          the dex file
 * @param string_idx    the string index
 *
 * @return              the string
 */
tb_char_t const*        dx_file_string(dx_file_ref_t file, tb_size_t string_idx);

/*! get class with the given index
 *
 * @param file          the dex file
 * @param class_idx     the class index
 *
 * @return              the dex class 
 */
dx_class_ref_t      dx_file_class(dx_file_ref_t file, tb_size_t class_idx);

/*! get the class count
 *
 * @param file          the dex file
 *
 * @return              the class count
 */
tb_size_t               dx_file_class_size(dx_file_ref_t file);

/*! get the class filename
 *
 * @param file          the dex file
 *
 * @return              the class count
 */
tb_size_t               dx_file_class_filename(dx_file_ref_t file, tb_size_t index);

/*! get method from the given index
 *
 * @param file          the dex file
 * @param method_idx    the method index
 *
 * @return              the dex method 
 */
dx_method_ref_t      dx_file_method(dx_file_ref_t file, tb_size_t method_idx);

/*! get field from the given index
 *
 * @param file          the dex file
 * @param field_idx     the field index
 *
 * @return              the dex field 
 */
dx_field_ref_t      dx_file_field(dx_file_ref_t file, tb_size_t field_idx);

/*! get field name from the given index
 *
 * @param file          the dex file
 * @param field_idx     the field index
 *
 * @return              the field name
 */
tb_char_t const*        dx_file_field_name(dx_file_ref_t file, tb_size_t field_idx);

/*! get field descriptor from the given index
 *
 * @param file          the dex file
 * @param field_idx     the field index
 *
 * @return              the field descriptor
 */
tb_char_t const*        dx_file_field_descriptor(dx_file_ref_t file, tb_size_t field_idx);

/*! get class descriptor from the given field index
 *
 * @param file          the dex file
 * @param field_idx     the field index
 *
 * @return              the class descriptor
 */
tb_char_t const*        dx_file_field_class_descriptor(dx_file_ref_t file, tb_size_t field_idx);

/*! get the method name from the given method index
 *
 * @param file          the dex file
 * @param method_idx    the method index
 *
 * @return              the method name
 */
tb_char_t const*        dx_file_method_name(dx_file_ref_t file, tb_size_t method_idx);

/*! get the method descriptor from the given method index
 *
 * @param file          the dex file
 * @param method_idx    the method index
 * @param descriptor    the method descriptor
 *
 * @return              the method descriptor
 */
tb_char_t const*        dx_file_method_descriptor(dx_file_ref_t file, tb_size_t method_idx, tb_string_ref_t descriptor);

/*! get class descriptor from the given method index
 *

 * @param file          the dex file
 * @param method_idx    the method index
 *
 * @return              the class descriptor
 */
tb_char_t const*        dx_file_method_class_descriptor(dx_file_ref_t file, tb_size_t method_idx);

/*! catch this exception and get the catch handler address
 *
 * @param file          the dex file
 * @param code          the dex code
 * @param pc            the pc address for input and the catch handler address for output
 * @param descriptor    the exception descriptor
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               dx_file_catch(dx_file_ref_t file, dx_code_ref_t code, tb_uint32_t* pc, tb_char_t const* descriptor);               

#ifdef DX_DUMP_ENABLE
/*! dump the dex file
 *
 * @param file          the dex file 
 */
tb_void_t               dx_file_dump(dx_file_ref_t file);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


