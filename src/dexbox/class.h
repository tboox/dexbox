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
 * @file        class.h
 *
 */
#ifndef DX_CLASS_H
#define DX_CLASS_H

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

/*! get the class filename
 *
 * @param clasz         the class 
 *
 * @return              the class count
 */
tb_char_t const*        dx_class_filename(dx_class_ref_t clasz);

/*! get the class descriptor
 *
 * @param clasz         the class 
 *
 * @return              the class descriptor
 */
tb_char_t const*        dx_class_descriptor(dx_class_ref_t clasz);

/*! get super class descriptor with the given class
 *
 * @param clasz         the class 
 *
 * @return              the super class 
 */
tb_char_t const*        dx_class_descriptor_super(dx_class_ref_t clasz);

/*! get the direct method 
 *
 * @param clasz         the dex class
 * @param method_idx    the dex method index
 *
 * @return              the dex method 
 */
dx_method_ref_t         dx_class_method_direct(dx_class_ref_t clasz, tb_size_t method_idx);

/*! get the virtual method 
 *
 * @param clasz         the dex class
 * @param method_idx    the dex method index
 *
 * @return              the dex method 
 */
dx_method_ref_t         dx_class_method_virtual(dx_class_ref_t clasz, tb_size_t method_idx);

/*! get the direct method count
 *
 * @param clasz         the dex class
 *
 * @return              the dex method count
 */
tb_size_t               dx_class_method_direct_size(dx_class_ref_t clasz);

/*! get the virtual method count
 *
 * @param clasz         the dex class
 *
 * @return              the dex method count
 */
tb_size_t               dx_class_method_virtual_size(dx_class_ref_t clasz);

/*! get the static field 
 *
 * @param clasz         the dex class
 * @param field_idx     the dex field index
 *
 * @return              the dex field 
 */
dx_field_ref_t          dx_class_field_static(dx_class_ref_t clasz, tb_size_t field_idx);

/*! get the instance field 
 *
 * @param clasz         the dex class
 * @param field_idx     the dex field index
 *
 * @return              the dex field 
 */
dx_field_ref_t          dx_class_field_instance(dx_class_ref_t clasz, tb_size_t field_idx);

/*! get the static field count
 *
 * @param clasz         the dex class
 *
 * @return              the dex field count
 */
tb_size_t               dx_class_field_static_size(dx_class_ref_t clasz);

/*! get the instance field count
 *
 * @param clasz         the dex class
 *
 * @return              the dex field count
 */
tb_size_t               dx_class_field_instance_size(dx_class_ref_t clasz);

#ifdef DX_DUMP_ENABLE
/*! dump class
 *
 * @param clasz         the class 
 */
tb_void_t               dx_class_dump(dx_class_ref_t clasz);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


