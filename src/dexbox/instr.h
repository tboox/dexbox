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
 * @file        instr.h
 *
 */
#ifndef DX_INSTR_H
#define DX_INSTR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "file.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the instruction formats for dalvik opcodes 
typedef enum __dx_instr_format_e 
{
    DX_INSTR_FMT_00x = 0        //!< unknown format (also used for "breakpoint" opcode)
,   DX_INSTR_FMT_10x            //!< op
,   DX_INSTR_FMT_12x            //!< op vA, vB
,   DX_INSTR_FMT_11n            //!< op vA, #+B
,   DX_INSTR_FMT_11x            //!< op vAA
,   DX_INSTR_FMT_10t            //!< op +AA
,   DX_INSTR_FMT_20bc           //!< [opt] op AA, thing@BBBB
,   DX_INSTR_FMT_20t            //!< op +AAAA
,   DX_INSTR_FMT_22x            //!< op vAA, vBBBB
,   DX_INSTR_FMT_21t            //!< op vAA, +BBBB
,   DX_INSTR_FMT_21s            //!< op vAA, #+BBBB
,   DX_INSTR_FMT_21h            //!< op vAA, #+BBBB00000[00000000]
,   DX_INSTR_FMT_21c            //!< op vAA, thing@BBBB
,   DX_INSTR_FMT_23x            //!< op vAA, vBB, vCC
,   DX_INSTR_FMT_22b            //!< op vAA, vBB, #+CC
,   DX_INSTR_FMT_22t            //!< op vA, vB, +CCCC
,   DX_INSTR_FMT_22s            //!< op vA, vB, #+CCCC
,   DX_INSTR_FMT_22c            //!< op vA, vB, thing@CCCC
,   DX_INSTR_FMT_22cs           //!< [opt] op vA, vB, field offset CCCC
,   DX_INSTR_FMT_30t            //!< op +AAAAAAAA
,   DX_INSTR_FMT_32x            //!< op vAAAA, vBBBB
,   DX_INSTR_FMT_31i            //!< op vAA, #+BBBBBBBB
,   DX_INSTR_FMT_31t            //!< op vAA, +BBBBBBBB
,   DX_INSTR_FMT_31c            //!< op vAA, string@BBBBBBBB
,   DX_INSTR_FMT_35c            //!< op {vC,vD,vE,vF,vG}, thing@BBBB
,   DX_INSTR_FMT_35ms           //!< [opt] invoke-virtual+super
,   DX_INSTR_FMT_3rc            //!< op {vCCCC .. v(CCCC+AA-1)}, thing@BBBB
,   DX_INSTR_FMT_3rms           //!< [opt] invoke-virtual+super/range
,   DX_INSTR_FMT_51l            //!< op vAA, #+BBBBBBBBBBBBBBBB
,   DX_INSTR_FMT_35mi           //!< [opt] inline invoke
,   DX_INSTR_FMT_3rmi           //!< [opt] inline invoke/range

    // TODO
,   DX_INSTR_FMT_45cc           //!< new, unknown
,   DX_INSTR_FMT_4rcc           //!< new, unknown

}dx_instr_format_e;

/// the types of indexed reference that are associated with opcodes 
typedef enum __dx_instr_index_type_e 
{
    DX_INSTR_INDEX_TYPE_UNKNOWN = 0
,   DX_INSTR_INDEX_TYPE_NONE                //!< has no index
,   DX_INSTR_INDEX_TYPE_VARIES              //!< "It depends." Used for throw-verification-error
,   DX_INSTR_INDEX_TYPE_TYPE_REF            //!< type reference index
,   DX_INSTR_INDEX_TYPE_STRING_REF          //!< string reference index
,   DX_INSTR_INDEX_TYPE_METHOD_REF          //!< method reference index
,   DX_INSTR_INDEX_TYPE_FIELD_REF           //!< field reference index
,   DX_INSTR_INDEX_TYPE_INLINE_METHOD       //!< inline method index (for inline linked methods)
,   DX_INSTR_INDEX_TYPE_VTABLE_OFFSET       //!< vtable offset (for static linked methods)
,   DX_INSTR_INDEX_TYPE_FIELD_OFFSET        //!< field offset (for static linked fields)

    // TODO
,   DX_INSTR_INDEX_TYPE_METHOD_AND_PROTO_REF
,   DX_INSTR_INDEX_TYPE_CALL_SITE_REF
,   DX_INSTR_INDEX_TYPE_METHOD_HANDLE_REF
,   DX_INSTR_INDEX_TYPE_PROTO_REF

}dx_instr_index_type_e;

/// the instruction control flow flags, used by the verifier and jit.
typedef enum __dx_instr_flags_e 
{
    DX_INSTR_FLAGS_CAN_BRANCH       = 1         //!< conditional or unconditional branch
,   DX_INSTR_FLAGS_CAN_CONTINUE     = 1 << 1    //!< flow can continue to next statement
,   DX_INSTR_FLAGS_CAN_SWITCH       = 1 << 2    //!< switch statement
,   DX_INSTR_FLAGS_CAN_THROW        = 1 << 3    //!< could cause an exception to be thrown
,   DX_INSTR_FLAGS_CAN_RETURN       = 1 << 4    //!< returns, no additional statements
,   DX_INSTR_FLAGS_INVOKE           = 1 << 5    //!< a flavor of invoke

}dx_instr_flags_e;

/// the instruction type
typedef struct __dx_instruction_t
{
    // vA/B/C/D
    tb_uint32_t         vA;
    tb_uint32_t         vB;
    tb_uint64_t         vB_wide;    //!< for format: 51l
    tb_uint32_t         vC;
    
    // vC/D/E/F/G in invoke or filled-new-array
    tb_uint32_t         arg[5];

    // the instruction data
    tb_uint16_t const*  instr;

    // the opcode
    tb_uint16_t         opcode;

    // the width
    tb_uint8_t          width;

    // the format
    tb_uint8_t          format;

    // the index type
    tb_uint8_t          index_type;

}dx_instruction_t, *dx_instruction_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get the instruction width
 *
 * @param instr         the dex instruction 
 *
 * @return              the instruction width
 */
tb_uint8_t              dx_instr_width(tb_uint16_t const* instr);

/*! decode the instruction 
 *
 * @param instr         the dex instruction 
 * @param instruction   the decoded instruction 
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               dx_instr_decode(tb_uint16_t const* instr, dx_instruction_ref_t instruction);

/*! packed-switch the instruction 
 *
 * @param instr         the dex instruction 
 * @param value         the test value
 *
 * @return              the frame pc offset, failed: the instruction size(3)
 */
tb_long_t               dx_instr_packed_switch(tb_uint16_t const* instr, tb_sint32_t value);

#ifdef DX_DUMP_ENABLE
/*! dump the instruction
 *
 * @param instruction   the dex instruction 
 * @param instr_idx     the dex instruction index
 * @param file          the dex file
 */
tb_void_t               dx_instr_dump(dx_instruction_ref_t instruction, tb_size_t instr_idx, dx_file_ref_t file);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


