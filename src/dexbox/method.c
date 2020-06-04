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
 * @file        method.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "method"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
dx_code_ref_t dx_method_code(dx_method_ref_t method)
{
    // check
    dx_method_t* dexmethod = (dx_method_t*)method;
    tb_assert_and_check_return_val(dexmethod && dexmethod->dexfile, tb_null);

    // get it
    return (dx_code_ref_t)dx_method_get_code(dexmethod->dexfile, dexmethod);
}
tb_char_t const* dx_method_name(dx_method_ref_t method)
{
    // check
    dx_method_t* dexmethod = (dx_method_t*)method;
    tb_assert_and_check_return_val(dexmethod && dexmethod->dexfile, tb_null);

    // get the method id
    dx_method_id_ref_t method_id = dx_file_get_method_id(dexmethod->dexfile, dexmethod->method_idx);
    tb_assert_and_check_return_val(method_id, tb_null);

    // get the method name
    return dx_file_get_string(dexmethod->dexfile, method_id->name_idx);
}
dx_proto_ref_t dx_method_proto(dx_method_ref_t method)
{
    // check
    dx_method_t* dexmethod = (dx_method_t*)method;
    tb_assert_and_check_return_val(dexmethod && dexmethod->dexfile, tb_null);

    // the proto hasn't been initialized?
    if (!dexmethod->proto.dexfile)
    {
        // get the method id
        dx_method_id_ref_t method_id = dx_file_get_method_id(dexmethod->dexfile, dexmethod->method_idx);
        tb_assert_and_check_return_val(method_id, tb_null);
       
        // init proto
        dexmethod->proto.dexfile = dexmethod->dexfile;
        dexmethod->proto.proto_idx = method_id->proto_idx;
    }

    // get it
    return (dx_proto_ref_t)&dexmethod->proto;
}
tb_size_t dx_method_access(dx_method_ref_t method)
{
    // check
    dx_method_t* dexmethod = (dx_method_t*)method;
    tb_assert_and_check_return_val(dexmethod, 0);

    // get it
    return dexmethod->access_flags;
}
dx_file_ref_t dx_method_dexfile(dx_method_ref_t method)
{
    // check
    dx_method_t* dexmethod = (dx_method_t*)method;
    tb_assert_and_check_return_val(dexmethod, tb_null);

    // get it
    return (dx_file_ref_t)dexmethod->dexfile;
}
tb_char_t const* dx_method_descriptor(dx_method_ref_t method, tb_string_ref_t descriptor)
{
    // get the proto of method
    dx_proto_ref_t proto = dx_method_proto(method);
    tb_assert_and_check_return_val(proto && descriptor, tb_null);

    // init descriptor
    tb_string_cstrcpy(descriptor, "(");

    // make the descriptor of parameters
    tb_size_t param_idx = 0;
    tb_size_t param_size = dx_proto_param_size(proto);
    for (param_idx = 0; param_idx < param_size; param_idx++)
    {
        tb_string_cstrcat(descriptor, dx_proto_param_descriptor(proto, param_idx));
    }

    // make the descriptor of the return value
    tb_string_cstrcat(descriptor, ")");
    tb_string_cstrcat(descriptor, dx_proto_retval_descriptor(proto));

    // ok?
    return tb_string_size(descriptor)? tb_string_cstr(descriptor) : tb_null;
}
tb_char_t const* dx_method_class_descriptor(dx_method_ref_t method)
{
    // check
    dx_method_t* dexmethod = (dx_method_t*)method;
    tb_assert_and_check_return_val(dexmethod && dexmethod->dexfile, tb_null);

    // get the method id
    dx_method_id_ref_t method_id = dx_file_get_method_id(dexmethod->dexfile, dexmethod->method_idx);
    tb_assert_and_check_return_val(method_id, tb_null);

    // get the class descriptor
    return dx_file_get_string_by_type_idx(dexmethod->dexfile, method_id->class_idx);
}
#ifdef DX_DUMP_ENABLE
tb_void_t dx_method_dump(dx_method_ref_t method)
{
    // get proto
    dx_proto_ref_t proto = dx_method_proto(method);
    tb_assert_and_check_return(proto);

    // the access flags
    tb_char_t const*    access_str = tb_null;
    tb_size_t           access_flags = dx_method_access(method);
    if (access_flags & DX_ACCESS_PUBLIC) access_str = "public";
    else if (access_flags & DX_ACCESS_PRIVATE) access_str = "private";
    else if (access_flags & DX_ACCESS_STATIC) access_str = "static";
    else if (access_flags & DX_ACCESS_FINAL) access_str = "final";

    // init typename
    tb_string_t type_name;
    tb_string_init(&type_name);

    // trace
    tb_printf("    .method %s %s %s(", access_str, dx_descriptor_type_short(dx_proto_retval_descriptor(proto), &type_name), dx_method_name(method));

    // dump the descriptor of parameters
    tb_size_t param_idx = 0;
    tb_size_t param_size = dx_proto_param_size(proto);
    for (param_idx = 0; param_idx < param_size; param_idx++)
    {
        // trace
        if (param_idx) tb_printf(", ");
        tb_printf("%s", dx_descriptor_type_short(dx_proto_param_descriptor(proto, param_idx), &type_name));
    }

    // dump end
    tb_printf(")\n");

    // exit typename
    tb_string_exit(&type_name);

    // dump code
    dx_code_ref_t code = dx_method_code(method);
    if (code) dx_code_dump(code, dx_method_dexfile(method));
}
#endif
