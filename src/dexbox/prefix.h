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
 * @file        prefix.h
 *
 */
#ifndef DX_PREFIX_H
#define DX_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "tbox/tbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// define the reference type of object
#define __dx_typeref__(object)                              struct __dx_##object##_dummy_t{tb_int_t dummy;} const* dx_##object##_ref_t

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the access flags and masks enum
typedef enum __dx_access_flags_e
{
    DX_ACCESS_PUBLIC                = 0x00000001        // class, field, method, ic
,   DX_ACCESS_PRIVATE               = 0x00000002        // field, method, ic
,   DX_ACCESS_PROTECTED             = 0x00000004        // field, method, ic
,   DX_ACCESS_STATIC                = 0x00000008        // field, method, ic
,   DX_ACCESS_FINAL                 = 0x00000010        // class, field, method, ic
,   DX_ACCESS_SYNCHRONIZED          = 0x00000020        // method (only allowed on natives)
,   DX_ACCESS_SUPER                 = 0x00000020        // class (not used in Dalvik)
,   DX_ACCESS_VOLATILE              = 0x00000040        // field
,   DX_ACCESS_BRIDGE                = 0x00000040        // method (1.5)
,   DX_ACCESS_TRANSIENT             = 0x00000080        // field
,   DX_ACCESS_VARARGS               = 0x00000080        // method (1.5)
,   DX_ACCESS_NATIVE                = 0x00000100        // method
,   DX_ACCESS_INTERFACE             = 0x00000200        // class, ic
,   DX_ACCESS_ABSTRACT              = 0x00000400        // class, method, ic
,   DX_ACCESS_STRICT                = 0x00000800        // method
,   DX_ACCESS_SYNTHETIC             = 0x00001000        // field, method, ic
,   DX_ACCESS_ANNOTATION            = 0x00002000        // class, ic (1.5)
,   DX_ACCESS_ENUM                  = 0x00004000        // class, field, ic (1.5)
,   DX_ACCESS_CONSTRUCTOR           = 0x00010000        // method (Dalvik only)
,   DX_ACCESS_DECLARED_SYNCHRONIZED = 0x00020000        // method (Dalvik only)
,   DX_ACCESS_CLASS_MASK            = (DX_ACCESS_PUBLIC | DX_ACCESS_FINAL | DX_ACCESS_INTERFACE | DX_ACCESS_ABSTRACT | DX_ACCESS_SYNTHETIC | DX_ACCESS_ANNOTATION | DX_ACCESS_ENUM)
,   DX_ACCESS_INNER_CLASS_MASK      = (DX_ACCESS_CLASS_MASK | DX_ACCESS_PRIVATE | DX_ACCESS_PROTECTED | DX_ACCESS_STATIC)
,   DX_ACCESS_FIELD_MASK            = (DX_ACCESS_PUBLIC | DX_ACCESS_PRIVATE | DX_ACCESS_PROTECTED | DX_ACCESS_STATIC | DX_ACCESS_FINAL | DX_ACCESS_VOLATILE | DX_ACCESS_TRANSIENT | DX_ACCESS_SYNTHETIC | DX_ACCESS_ENUM)
,   DX_ACCESS_METHOD_MASK           = (DX_ACCESS_PUBLIC | DX_ACCESS_PRIVATE | DX_ACCESS_PROTECTED | DX_ACCESS_STATIC | DX_ACCESS_FINAL | DX_ACCESS_SYNCHRONIZED | DX_ACCESS_BRIDGE | DX_ACCESS_VARARGS | DX_ACCESS_NATIVE | DX_ACCESS_ABSTRACT | DX_ACCESS_STRICT | DX_ACCESS_SYNTHETIC | DX_ACCESS_CONSTRUCTOR | DX_ACCESS_DECLARED_SYNCHRONIZED)

}dx_access_flags_e;

/// the object ref type
typedef __dx_typeref__(object);

/// the dex file ref type
typedef __dx_typeref__(file);

/// the dex code ref type
typedef __dx_typeref__(code);

/// the dex class ref type
typedef __dx_typeref__(class);

/// the dex method ref type
typedef __dx_typeref__(method);

/// the dex field ref type
typedef __dx_typeref__(field);

// the dex try type
typedef struct __dx_try_t 
{
    // start address, in 16-bit code units
    tb_uint32_t     start_addr; 

    // instruction count, in 16-bit code units
    tb_uint16_t     insn_count; 

    // offset in encoded handler data to handlers
    tb_uint16_t     handler_off; 

}dx_try_t, *dx_try_ref_t;

#endif


