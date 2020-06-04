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
 * @file        code.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "code"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
dx_try_ref_t dx_code_tries(dx_code_ref_t code) 
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, tb_null);
    
    // get the instruction end
    tb_uint16_t const* insns_end = &dexcode->insns[dexcode->insns_size];

    // round it
    if ((((tb_size_t)insns_end) & 3)) insns_end++;

    // get it
    return (dx_try_ref_t)insns_end;
}
tb_size_t dx_code_tries_size(dx_code_ref_t code)
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, 0);

    // get it
    return dexcode->tries_size;
}
tb_size_t dx_code_incoming_size(dx_code_ref_t code)
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, 0);

    // get it
    return dexcode->ins_size;
}
tb_size_t dx_code_outgoing_size(dx_code_ref_t code)
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, 0);

    // get it
    return dexcode->outs_size;
}
tb_size_t dx_code_register_size(dx_code_ref_t code)
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, 0);

    // get it
    return dexcode->registers_size;
}
tb_size_t dx_code_instr_size(dx_code_ref_t code)
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, 0);

    // get it
    return dexcode->insns_size;
}
tb_uint16_t const* dx_code_instr_data(dx_code_ref_t code)
{
    // check
    dx_code_t* dexcode = (dx_code_t*)code;
    tb_assert_and_check_return_val(dexcode, tb_null);

    // get it
    return (tb_uint16_t const*)dexcode->insns;
}
#ifdef DX_DUMP_ENABLE
tb_void_t dx_code_dump(dx_code_ref_t code, dx_file_ref_t file)
{
    // check
    tb_assert_and_check_return(code && file);
   
    // trace
    tb_printf("        .register %lu\n", dx_code_register_size(code));
    tb_printf("        .argument %lu\n", dx_code_incoming_size(code));
    tb_printf("        .catches %lu\n", dx_code_tries_size(code));

    // dump tries
    tb_size_t           i = 0;
    dx_try_ref_t    tries_data = dx_code_tries(code);
    tb_size_t           tries_size = dx_code_tries_size(code);
    for (i = 0; i < tries_size; i++)
    {
        // get the try
        dx_try_ref_t ptry = &tries_data[i];

        // trace
        tb_printf("            0x%04x - 0x%04x\n", ptry->start_addr, ptry->start_addr + ptry->insn_count);

        // dump catches
        dx_catch_t trycatch;
        if (dx_catch_init(&trycatch, file, code, ptry->handler_off))
        {
            // done
            dx_catch_handler_ref_t handler = tb_null;
            while ((handler = dx_catch_next(&trycatch)))
            {
                // get the descriptor
                tb_char_t const* descriptor = handler->type_idx != 0xffffffff? dx_file_type(file, handler->type_idx) : "<any>";

                // trace
                tb_printf("                %s -> 0x%04x\n", descriptor, handler->address);
            }
        }
    }

    // enter code 
    tb_printf("        .prologue\n");

    // dump code
    tb_size_t           instr_idx = 0;
    tb_size_t           instr_size = dx_code_instr_size(code);
    tb_uint16_t const*  instr_data = dx_code_instr_data(code);
    while (instr_idx < instr_size)
    {
        // get instruction width
        tb_size_t instr_width = dx_instr_width(instr_data);
        tb_assert(instr_width);

        // decode instruction
        dx_instruction_t instruction = {0};
        if (!dx_instr_decode(instr_data, &instruction)) break ;

        // dump instruction
        dx_instr_dump(&instruction, instr_idx, file);

        // the next instruction
        instr_idx   += instr_width;
        instr_data  += instr_width;
    }

    // dump end
    tb_printf("\n");
}
#endif
