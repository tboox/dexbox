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
 * @file        field.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "field"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
tb_char_t const* dx_field_name(dx_field_ref_t field)
{
    // check
    dx_field_t* dexfield = (dx_field_t*)field;
    tb_assert_and_check_return_val(dexfield && dexfield->dexfile, tb_null);

    // get the field id
    dx_field_id_ref_t field_id = dx_file_get_field_id(dexfield->dexfile, dexfield->field_idx);
    tb_assert_and_check_return_val(field_id, tb_null);

    // get the field name
    return dx_file_get_string(dexfield->dexfile, field_id->name_idx);
}
tb_char_t const* dx_field_descriptor(dx_field_ref_t field)
{
    // check
    dx_field_t* dexfield = (dx_field_t*)field;
    tb_assert_and_check_return_val(dexfield && dexfield->dexfile, tb_null);

    // get the field id
    dx_field_id_ref_t field_id = dx_file_get_field_id(dexfield->dexfile, dexfield->field_idx);
    tb_assert_and_check_return_val(field_id, tb_null);

    // get the field descriptor
    return dx_file_get_string_by_type_idx(dexfield->dexfile, field_id->type_idx);
}
tb_size_t dx_field_access(dx_field_ref_t field)
{
    // check
    dx_field_t* dexfield = (dx_field_t*)field;
    tb_assert_and_check_return_val(dexfield, 0);

    // get the field name
    return dexfield->access_flags;
}
dx_value_ref_t dx_field_value(dx_field_ref_t field)
{
    // check
    dx_field_t* dexfield = (dx_field_t*)field;
    tb_assert_and_check_return_val(dexfield, tb_null);

    // no value? uses the default value
    if (!dexfield->value.type)
    {
        // get the descriptor
        tb_char_t const* descriptor = dx_field_descriptor(field);
        tb_assert_and_check_return_val(descriptor, tb_null);

        // init it
        dx_value_init(&dexfield->value, descriptor);
    }

    // get the field value
    return dexfield->value.type != DX_VALUE_NONE? (dx_value_ref_t)&dexfield->value : tb_null;
}
tb_bool_t dx_field_value_set(dx_field_ref_t field, dx_value_ref_t value)
{
    // check
    dx_field_t* dexfield = (dx_field_t*)field;
    tb_assert_and_check_return_val(dexfield, tb_false);

    // set the field value
    if (value) dexfield->value = *value;
    else value->type = DX_VALUE_NONE;

    // ok
    return tb_true;
}
#ifdef DX_DUMP_ENABLE
tb_void_t dx_field_dump(dx_field_ref_t field)
{
    // the access flags
    tb_char_t const*    access_str = tb_null;
    tb_size_t           access_flags = dx_field_access(field);
    if (access_flags & DX_ACCESS_PUBLIC) access_str = "public";
    else if (access_flags & DX_ACCESS_PRIVATE) access_str = "private";
    else if (access_flags & DX_ACCESS_STATIC) access_str = "static";
    else if (access_flags & DX_ACCESS_FINAL) access_str = "final";

    // init typename
    tb_string_t type_name;
    tb_string_init(&type_name);

    // trace
    tb_printf("    .field %s %s %s", access_str, dx_descriptor_type_short(dx_field_descriptor(field), &type_name), dx_field_name(field));

    // dump value
    dx_value_ref_t value = dx_field_value(field);
    if (value) 
    {
        tb_char_t data[256] = {0};
        tb_printf(" = %s", dx_value_cstr(value, data, sizeof(data)));
    }
    
    // trace end
    tb_printf("\n");

    // exit type
    tb_string_exit(&type_name);
}
#endif
