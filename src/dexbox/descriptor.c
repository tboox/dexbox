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
 * @file        descriptor.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "descriptor"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "descriptor.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation 
 */
static tb_char_t const* dx_descriptor_primitive_type(tb_char_t type)
{
    // done
    switch (type) 
    {
    case 'B':   return "byte";
    case 'C':   return "char";
    case 'D':   return "double";
    case 'F':   return "float";
    case 'I':   return "int";
    case 'J':   return "long";
    case 'S':   return "short";
    case 'V':   return "void";
    case 'Z':   return "boolean";
    default:    break;
    }

    // unknown
    return "unknown";
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
tb_char_t const* dx_descriptor_type(tb_char_t const* descriptor, tb_string_ref_t result)
{
    // check
    tb_assert_and_check_return_val(descriptor && result, tb_null);

    // clear result first
    tb_string_clear(result);

    // strip leading [s; will be added to end
    tb_long_t offset = 0;
    tb_long_t array_depth = 0;
    tb_long_t target_size = tb_strlen(descriptor);
    while (target_size > 1 && descriptor[offset] == '[') 
    {
        offset++;
        target_size--;
    }
    array_depth = offset;

    // adjust the target size
    if (target_size == 1) 
    {
        // the primitive type 
        descriptor = dx_descriptor_primitive_type(descriptor[offset]);
        offset = 0;
        target_size = tb_strlen(descriptor);
    }
    else
    {
        // account for leading 'L' and trailing ';'
        if (target_size >= 2 && descriptor[offset] == 'L' && descriptor[offset + target_size - 1] == ';')
        {
            target_size -= 2;
            offset++;
        }
    }

    // copy class name
    tb_long_t i;
    for (i = 0; i < target_size; i++) 
    {
        tb_char_t ch = descriptor[offset + i];
        tb_string_chrcat(result, (ch == '/' || ch == '$') ? '.' : ch);
    }

    // add the appropriate number of brackets for arrays
    while (array_depth-- > 0) 
    {
        tb_string_chrcat(result, '[');
        tb_string_chrcat(result, ']');
    }

    // ok?
    return tb_string_cstr(result);
}
tb_char_t const* dx_descriptor_type_short(tb_char_t const* descriptor, tb_string_ref_t result)
{
    // attempt to get class name first (short)
    tb_char_t const* p = dx_descriptor_class_name(descriptor, result);

    // get the other long type name
    if (!p) p = dx_descriptor_type(descriptor, result);

    // ok?
    return p;
}
tb_char_t const* dx_descriptor_class_name(tb_char_t const* descriptor, tb_string_ref_t result)
{
    // check
    tb_assert_and_check_return_val(descriptor && result, tb_null);

    // clear result first
    tb_string_clear(result);

    // must be class
    tb_check_return_val(descriptor[0] == 'L', tb_null);

    // find the position of class name
    tb_char_t const* p = tb_strrchr(descriptor, '/');
    if (!p) p = descriptor + 1;
    else p++;

    // the class name size
    tb_size_t size = tb_strlen(p);
    tb_check_return_val(size > 1, tb_null);

    // strip the ';'
    size--;

    // copy class name
    tb_string_cstrncpy(result, p, size);

    // replace '$' to '.'
    tb_char_t* q = (tb_char_t*)tb_string_cstr(result);
    while (*q)
    {
        if (*q == '$') *q = '.';
        q++;
    }

    // ok?
    return tb_string_cstr(result);
}
tb_char_t const* dx_descriptor_jclass_name(tb_char_t const* descriptor, tb_string_ref_t result)
{
    // check
    tb_assert_and_check_return_val(descriptor && result, tb_null);

    // clear result first
    tb_string_clear(result);

    // must be object or array object
    tb_check_return_val(descriptor[0] == 'L' || (descriptor[0] == '[' && descriptor[1] == 'L'), tb_null);

    // is object? 
    if (descriptor[0] == 'L')
    {
        // the size
        tb_size_t size = tb_strlen(descriptor);
        tb_assert_and_check_return_val(size && descriptor[size - 1] == ';', tb_null);

        // copy it and remove 'L' and ';'
        tb_string_cstrncpy(result, descriptor + 1, size - 2);
    }
    // is array object?
    else
    {
        // copy it directly
        tb_string_cstrcpy(result, descriptor);
    }

    // ok?
    return tb_string_cstr(result);
}
