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
 * @file        class.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "class"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
tb_char_t const* dx_class_filename(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass && dexclass->dexfile && dexclass->class_def, tb_null);

    // get it
    return (dexclass->class_def->source_file_idx == 0xffffffff)? tb_null : dx_file_get_string(dexclass->dexfile, dexclass->class_def->source_file_idx);
}
tb_char_t const* dx_class_descriptor(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass && dexclass->dexfile && dexclass->class_def, tb_null);

    // get the class descriptor
    return dx_file_get_string_by_type_idx(dexclass->dexfile, dexclass->class_def->class_idx);
}
tb_char_t const* dx_class_descriptor_super(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass && dexclass->dexfile && dexclass->class_def, tb_null);

    // get it
    return dx_file_get_string_by_type_idx(dexclass->dexfile, dexclass->class_def->superclass_idx);
}
dx_method_ref_t dx_class_method_direct(dx_class_ref_t clasz, tb_size_t method_idx)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, tb_null);

    // out of range?
    tb_assert(method_idx < dexclass->header.direct_methods_size);

    // get it
    return (dx_method_ref_t)&dexclass->direct_methods[method_idx];
}
dx_method_ref_t dx_class_method_virtual(dx_class_ref_t clasz, tb_size_t method_idx)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, tb_null);

    // out of range?
    tb_assert(method_idx < dexclass->header.virtual_methods_size);

    // get it
    return (dx_method_ref_t)&dexclass->virtual_methods[method_idx];
}
tb_size_t dx_class_method_direct_size(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, 0);

    // get it
    return dexclass->header.direct_methods_size;
}
tb_size_t dx_class_method_virtual_size(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, 0);

    // get it
    return dexclass->header.virtual_methods_size;
}
dx_field_ref_t dx_class_field_static(dx_class_ref_t clasz, tb_size_t field_idx)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, tb_null);

    // out of range?
    tb_assert(field_idx < dexclass->header.static_fields_size);

    // get it
    return (dx_field_ref_t)&dexclass->static_fields[field_idx];
}
dx_field_ref_t dx_class_field_instance(dx_class_ref_t clasz, tb_size_t field_idx)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, tb_null);

    // out of range?
    tb_assert(field_idx < dexclass->header.instance_fields_size);

    // get it
    return (dx_field_ref_t)&dexclass->instance_fields[field_idx];
}
tb_size_t dx_class_field_static_size(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, 0);

    // get it
    return dexclass->header.static_fields_size;
}
tb_size_t dx_class_field_instance_size(dx_class_ref_t clasz)
{
    // check
    dx_class_t* dexclass = (dx_class_t*)clasz;
    tb_assert_and_check_return_val(dexclass, 0);

    // get it
    return dexclass->header.instance_fields_size;
}
#ifdef DX_DUMP_ENABLE
tb_void_t dx_class_dump(dx_class_ref_t clasz)
{
    // init classname
    tb_string_t classname;
    tb_string_init(&classname);

    // trace
    tb_printf(".file %s\n", dx_class_filename(clasz));
    tb_printf(".class %s\n", dx_descriptor_class_name(dx_class_descriptor(clasz), &classname));
    tb_printf(".super %s\n", dx_descriptor_class_name(dx_class_descriptor_super(clasz), &classname));

    // exit classname 
    tb_string_exit(&classname);

    // dump static fields
    tb_size_t field_idx    = 0;
    tb_size_t field_size   = dx_class_field_static_size(clasz);
    for (field_idx = 0; field_idx < field_size; field_idx++)
    {
        // get field
        dx_field_ref_t field = dx_class_field_static(clasz, field_idx);
        if (field)
        {
            // dump field
            dx_field_dump(field);
        }
    }

    // dump instance fields
    field_size = dx_class_field_instance_size(clasz);
    for (field_idx = 0; field_idx < field_size; field_idx++)
    {
        // get field
        dx_field_ref_t field = dx_class_field_instance(clasz, field_idx);
        if (field)
        {
            // dump field
            dx_field_dump(field);
        }
    }

    // dump direct methods
    tb_size_t method_idx    = 0;
    tb_size_t method_size   = dx_class_method_direct_size(clasz);
    for (method_idx = 0; method_idx < method_size; method_idx++)
    {
        // get method
        dx_method_ref_t method = dx_class_method_direct(clasz, method_idx);
        if (method)
        {
            // dump method
            dx_method_dump(method);
        }
    }

    // dump virtual methods
    method_size = dx_class_method_virtual_size(clasz);
    for (method_idx = 0; method_idx < method_size; method_idx++)
    {
        // get method
        dx_method_ref_t method = dx_class_method_virtual(clasz, method_idx);
        if (method)
        {
            // dump method
            dx_method_dump(method);
        }
    }

    // trace
    tb_printf("\n");
}
#endif
