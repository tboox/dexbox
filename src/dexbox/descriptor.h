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
 * @file        descriptor.h
 *
 */
#ifndef DX_DESCRIPTOR_H
#define DX_DESCRIPTOR_H

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

/*! get the type from the descriptor
 *
 * "Ljava/lang/String;" => "java.lang.String" and "[I" => "int[]". and '$' => '.'
 *
 * @param descriptor    the descriptor 
 * @param result        the result string
 *
 * @return              the type name, return tb_null if be not class
 */
tb_char_t const*        dx_descriptor_type(tb_char_t const* descriptor, tb_string_ref_t result);

/*! get the short type from the descriptor
 * 
 * "Ljava/lang/String;" => String and "[I" => "int[]". and '$' => '.'
 *
 * @param descriptor    the descriptor 
 * @param result        the result string
 *
 * @return              the class name or primitive type
 */
tb_char_t const*        dx_descriptor_type_short(tb_char_t const* descriptor, tb_string_ref_t result);

/*! get the class name from the descriptor
 *
 * @param descriptor    the descriptor 
 * @param result        the result string
 *
 * @return              the class name, return tb_null if be not class
 */
tb_char_t const*        dx_descriptor_class_name(tb_char_t const* descriptor, tb_string_ref_t result);

/*! get the class name from the descriptor for jni
 *
 * <pre>
 * .e.g
 *
 * object: java/lang/String 
 * array: [Ljava/lang/String;
 * </pre>
 *
 * @param descriptor    the descriptor 
 * @param result        the result string
 *
 * @return              the class name, return tb_null if be not class
 */
tb_char_t const*        dx_descriptor_jclass_name(tb_char_t const* descriptor, tb_string_ref_t result);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


