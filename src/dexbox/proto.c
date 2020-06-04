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
 * @file        proto.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "proto"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation 
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
tb_size_t dx_proto_param_size(dx_proto_ref_t proto)
{
    // check
    dx_proto_t* dexproto = (dx_proto_t*)proto;
    tb_assert_and_check_return_val(dexproto && dexproto->dexfile, 0);

    // the parameters have not been initialized?
    if (!dexproto->parameters)
    {
        // get the proto id
        dx_proto_id_ref_t proto_id = dx_file_get_proto_id(dexproto->dexfile, dexproto->proto_idx);
        tb_assert_and_check_return_val(proto_id, 0);

        // get parameters
        dexproto->parameters = dx_file_get_proto_parameters(dexproto->dexfile, proto_id);
    }

    // get count
    return dexproto->parameters? dexproto->parameters->size : 0;
}
tb_char_t const* dx_proto_param_descriptor(dx_proto_ref_t proto, tb_size_t index)
{
    // check
    dx_proto_t* dexproto = (dx_proto_t*)proto;
    tb_assert_and_check_return_val(dexproto && dexproto->dexfile && dexproto->parameters, tb_null);

    // get type_idx
    tb_uint32_t type_idx = dx_file_get_type_idx(dexproto->parameters, index);

    // get descriptor
    return dx_file_get_string_by_type_idx(dexproto->dexfile, type_idx);
}
tb_char_t const* dx_proto_retval_descriptor(dx_proto_ref_t proto)
{
    // check
    dx_proto_t* dexproto = (dx_proto_t*)proto;
    tb_assert_and_check_return_val(dexproto && dexproto->dexfile, tb_null);

    // get the proto id
    dx_proto_id_ref_t proto_id = dx_file_get_proto_id(dexproto->dexfile, dexproto->proto_idx);
    tb_assert_and_check_return_val(proto_id, tb_null);

    // get descriptor
    return dx_file_get_string_by_type_idx(dexproto->dexfile, proto_id->return_type_idx);
}

