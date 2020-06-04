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
 * @file        instr.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "instr"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros 
 */

// identifying pseudo-opcode format
#define DX_INSTR_IDENT_PACKED_SWITCH_PAYLOAD        (0x0100)
#define DX_INSTR_IDENT_SPARSE_SWITCH_PAYLOAD        (0x0200)
#define DX_INSTR_IDENT_FILL_ARRAY_DATA              (0x0300)

// get vA from the instruction code
#define DX_INSTR_A(instr_unit)                      (((tb_uint16_t)(instr_unit) >> 8) & 0x0f)

// get vB from the instruction code
#define DX_INSTR_B(instr_unit)                      ((tb_uint16_t)(instr_unit) >> 12)

// get vAA from the instruction code
#define DX_INSTR_AA(instr_unit)                     ((instr_unit) >> 8)

// fetch two bytes from the instruction offset
#define DX_FETCH_u2(instr, offset)                  ((instr)[(offset)])

// fetch four bytes from the instruction offset
#define DX_FETCH_u4(instr, offset)                  ((instr)[(offset)] | ((tb_uint32_t)((instr)[(offset) + 1]) << 16))

/* //////////////////////////////////////////////////////////////////////////////////////
 * types 
 */

// the instruction info table type
typedef struct __dx_instr_info_table_t
{
    // the instruction formats
    tb_uint8_t const*       formats;

    // the instruction index types
    tb_uint8_t const*       index_types;

    // the instruction flags
    tb_uint8_t const*       flags;

    // the instruction widths
    tb_uint8_t const*       widths;

}dx_instr_info_table_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals 
 */

// the instruction width table
static tb_uint8_t g_instr_width_table[] = 
{
    // BEGIN(dexbox-widths)
    1, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 3, 2, 2, 3, 5, 2, 2, 3, 2, 1, 1, 2,
    2, 1, 2, 2, 3, 3, 3, 1, 1, 2, 3, 3, 3, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
    0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3,
    3, 3, 3, 0, 3, 3, 3, 3, 3, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 3, 3,
    3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 4, 4, 3, 3, 2, 2,
    // END(dexbox-widths)
};

// the instruction flags table
static tb_uint8_t g_instr_flags_table[] = 
{
    // BEGIN(dexbox-flags)
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_RETURN,
    DX_INSTR_FLAGS_CAN_RETURN,
    DX_INSTR_FLAGS_CAN_RETURN,
    DX_INSTR_FLAGS_CAN_RETURN,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_SWITCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_SWITCH,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_BRANCH,
    0,
    0,
    0,
    0,
    0,
    0,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    0,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    0,
    0,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    0,
    DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_RETURN,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW|DX_INSTR_FLAGS_INVOKE,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    DX_INSTR_FLAGS_CAN_CONTINUE|DX_INSTR_FLAGS_CAN_THROW,
    // END(dexbox-flags)
};

// the instruction format table
static tb_uint8_t g_instr_format_table[] = 
{
    // BEGIN(dexbox-formats)
    DX_INSTR_FMT_10x, DX_INSTR_FMT_12x, DX_INSTR_FMT_22x, DX_INSTR_FMT_32x, DX_INSTR_FMT_12x, DX_INSTR_FMT_22x, DX_INSTR_FMT_32x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_22x, DX_INSTR_FMT_32x, DX_INSTR_FMT_11x, DX_INSTR_FMT_11x, DX_INSTR_FMT_11x, DX_INSTR_FMT_11x,
    DX_INSTR_FMT_10x, DX_INSTR_FMT_11x, DX_INSTR_FMT_11x, DX_INSTR_FMT_11x, DX_INSTR_FMT_11n, DX_INSTR_FMT_21s, DX_INSTR_FMT_31i,
    DX_INSTR_FMT_21h, DX_INSTR_FMT_21s, DX_INSTR_FMT_31i, DX_INSTR_FMT_51l, DX_INSTR_FMT_21h, DX_INSTR_FMT_21c, DX_INSTR_FMT_31c,
    DX_INSTR_FMT_21c, DX_INSTR_FMT_11x, DX_INSTR_FMT_11x, DX_INSTR_FMT_21c, DX_INSTR_FMT_22c, DX_INSTR_FMT_12x, DX_INSTR_FMT_21c,
    DX_INSTR_FMT_22c, DX_INSTR_FMT_35c, DX_INSTR_FMT_3rc, DX_INSTR_FMT_31t, DX_INSTR_FMT_11x, DX_INSTR_FMT_10t, DX_INSTR_FMT_20t,
    DX_INSTR_FMT_30t, DX_INSTR_FMT_31t, DX_INSTR_FMT_31t, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_22t, DX_INSTR_FMT_22t, DX_INSTR_FMT_22t, DX_INSTR_FMT_22t, DX_INSTR_FMT_22t, DX_INSTR_FMT_22t,
    DX_INSTR_FMT_21t, DX_INSTR_FMT_21t, DX_INSTR_FMT_21t, DX_INSTR_FMT_21t, DX_INSTR_FMT_21t, DX_INSTR_FMT_21t, DX_INSTR_FMT_00x,
    DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c,
    DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c,
    DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c,
    DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c,
    DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_35c, DX_INSTR_FMT_35c,
    DX_INSTR_FMT_35c, DX_INSTR_FMT_35c, DX_INSTR_FMT_35c, DX_INSTR_FMT_00x, DX_INSTR_FMT_3rc, DX_INSTR_FMT_3rc, DX_INSTR_FMT_3rc,
    DX_INSTR_FMT_3rc, DX_INSTR_FMT_3rc, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x, DX_INSTR_FMT_23x,
    DX_INSTR_FMT_23x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x,
    DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_12x, DX_INSTR_FMT_22s, DX_INSTR_FMT_22s,
    DX_INSTR_FMT_22s, DX_INSTR_FMT_22s, DX_INSTR_FMT_22s, DX_INSTR_FMT_22s, DX_INSTR_FMT_22s, DX_INSTR_FMT_22s, DX_INSTR_FMT_22b,
    DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22b,
    DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22b, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c,
    DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_22c, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c, DX_INSTR_FMT_00x, DX_INSTR_FMT_20bc,
    DX_INSTR_FMT_35mi, DX_INSTR_FMT_3rmi, DX_INSTR_FMT_35c, DX_INSTR_FMT_10x, DX_INSTR_FMT_22cs, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x,
    DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_00x, DX_INSTR_FMT_45cc, DX_INSTR_FMT_4rcc,
    DX_INSTR_FMT_35c, DX_INSTR_FMT_3rc, DX_INSTR_FMT_21c, DX_INSTR_FMT_21c,
    // END(dexbox-formats)
};

// the instruction index type table
static tb_uint8_t g_instr_index_type_table[] =
{
    // BEGIN(dexbox-index-types)
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_STRING_REF,
    DX_INSTR_INDEX_TYPE_STRING_REF, DX_INSTR_INDEX_TYPE_TYPE_REF, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_TYPE_REF, DX_INSTR_INDEX_TYPE_TYPE_REF,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_TYPE_REF, DX_INSTR_INDEX_TYPE_TYPE_REF,
    DX_INSTR_INDEX_TYPE_TYPE_REF, DX_INSTR_INDEX_TYPE_TYPE_REF, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_UNKNOWN,
    DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN,
    DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_METHOD_REF,
    DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_METHOD_REF,
    DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_METHOD_REF,
    DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_METHOD_REF,
    DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE,
    DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF,
    DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_FIELD_REF, DX_INSTR_INDEX_TYPE_UNKNOWN,
    DX_INSTR_INDEX_TYPE_VARIES, DX_INSTR_INDEX_TYPE_INLINE_METHOD, DX_INSTR_INDEX_TYPE_INLINE_METHOD,
    DX_INSTR_INDEX_TYPE_METHOD_REF, DX_INSTR_INDEX_TYPE_NONE, DX_INSTR_INDEX_TYPE_FIELD_OFFSET,
    DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN,
    DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_UNKNOWN,
    DX_INSTR_INDEX_TYPE_UNKNOWN, DX_INSTR_INDEX_TYPE_METHOD_AND_PROTO_REF, DX_INSTR_INDEX_TYPE_METHOD_AND_PROTO_REF,
    DX_INSTR_INDEX_TYPE_CALL_SITE_REF, DX_INSTR_INDEX_TYPE_CALL_SITE_REF, DX_INSTR_INDEX_TYPE_METHOD_HANDLE_REF,
    DX_INSTR_INDEX_TYPE_PROTO_REF,
    // END(dexbox-index-types)
};

// the instruction info table
static dx_instr_info_table_t g_instr_info = 
{
    g_instr_format_table
,   g_instr_index_type_table
,   g_instr_flags_table
,   g_instr_width_table
};


/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation 
 */
static __tb_inline__ tb_uint16_t dx_instr_get_opcode(tb_uint16_t instr_unit) 
{
    // get opcode
    tb_uint16_t low_byte = instr_unit & 0xff;
    return (low_byte != 0xff)? low_byte : ((instr_unit >> 8) | 0x100);
}
static __tb_inline__ tb_uint8_t dx_instr_get_width_from_opcode(tb_uint16_t opcode)
{
    // check
    tb_assert(opcode < tb_arrayn(g_instr_width_table));

    // get it
    return g_instr_info.widths[opcode];
}
static __tb_inline__ tb_uint8_t dx_instr_get_format_from_opcode(tb_uint16_t opcode)
{
    // check
    tb_assert(opcode < tb_arrayn(g_instr_format_table));

    // get it
    return g_instr_info.formats[opcode];
}
static __tb_inline__ tb_uint8_t dx_instr_get_index_type_from_opcode(tb_uint16_t opcode)
{
    // check
    tb_assert(opcode < tb_arrayn(g_instr_index_type_table));

    // get it
    return g_instr_info.index_types[opcode];
}
#ifdef DX_DUMP_ENABLE
static tb_char_t const* dx_instr_get_index_info(dx_file_ref_t file, dx_instruction_ref_t instruction, tb_string_ref_t index_info)
{
    // get index and width
    tb_uint32_t width = 0;
    tb_uint32_t index = 0;
    switch (instruction->format)
    {
    case DX_INSTR_FMT_20bc:
    case DX_INSTR_FMT_21c:
    case DX_INSTR_FMT_35c:
    case DX_INSTR_FMT_35ms:
    case DX_INSTR_FMT_3rc:
    case DX_INSTR_FMT_3rms:
    case DX_INSTR_FMT_35mi:
    case DX_INSTR_FMT_3rmi:
        index = instruction->vB;
        width = 4;
        break;
    case DX_INSTR_FMT_31c:
        index = instruction->vB;
        width = 8;
        break;
    case DX_INSTR_FMT_22c:
    case DX_INSTR_FMT_22cs:
        index = instruction->vC;
        width = 4;
        break;
    default:
        index = 0;
        width = 4;
        break;
    }

    // make index string
    dx_file_t* dexfile = (dx_file_t*)file;
    switch (instruction->index_type)
    {
    case DX_INSTR_INDEX_TYPE_UNKNOWN:
        tb_string_cstrfcat(index_info, "<unknown-index>");
        break;
    case DX_INSTR_INDEX_TYPE_NONE:
        tb_string_cstrfcat(index_info, "<no-index>");
        break;
    case DX_INSTR_INDEX_TYPE_VARIES:
        tb_string_cstrfcat(index_info, "<index-varies>");
        break;
    case DX_INSTR_INDEX_TYPE_TYPE_REF:
        {
            if (index < dexfile->header->type_ids_size) 
            {
                // init type name
                tb_string_t type_name;
                tb_string_init(&type_name);

                // make info
                tb_string_cstrfcat(index_info, "%s", dx_descriptor_type_short(dx_file_get_string_by_type_idx(dexfile, index), &type_name));

                // exit type_name
                tb_string_exit(&type_name);
            }
            else tb_string_cstrfcat(index_info, "<type?>");
        }
        break;
    case DX_INSTR_INDEX_TYPE_STRING_REF:
        {
            if (index < dexfile->header->string_ids_size) 
                tb_string_cstrfcat(index_info, "\"%s\"", dx_file_get_string(dexfile, index));
            else 
                tb_string_cstrfcat(index_info, "<string?>");
        }
        break;
    case DX_INSTR_INDEX_TYPE_METHOD_REF:
        {
            if (index < dexfile->header->method_ids_size) 
            {
                // get the method id
                dx_method_id_ref_t method_id = dx_file_get_method_id(dexfile, index);
                tb_assert(method_id);
 
                // init proto
                dx_proto_t proto = {0};
                proto.dexfile = dexfile;
                proto.proto_idx = method_id->proto_idx;

                // init type name
                tb_string_t type_name;
                tb_string_init(&type_name);

                // init descriptor
                tb_string_t descriptor;
                tb_string_init(&descriptor);

                // make descriptor
                tb_size_t param_idx = 0;
                tb_size_t param_size = dx_proto_param_size((dx_proto_ref_t)&proto);
                tb_string_cstrcpy(&descriptor, "(");
                for (param_idx = 0; param_idx < param_size; param_idx++)
                {
                    if (param_idx) tb_string_cstrfcat(&descriptor, ", ");
                    tb_string_cstrfcat(&descriptor, "%s", dx_descriptor_type_short(dx_proto_param_descriptor((dx_proto_ref_t)&proto, param_idx), &type_name));
                }
                tb_string_cstrfcat(&descriptor, ")%s", dx_descriptor_type_short(dx_proto_retval_descriptor((dx_proto_ref_t)&proto), &type_name));

                // make info
                tb_string_cstrfcat( index_info
                                ,   "%s.%s%s"
                                ,   dx_descriptor_class_name(dx_file_get_string_by_type_idx(dexfile, method_id->class_idx), &type_name)
                                ,   dx_file_get_string(dexfile, method_id->name_idx)
                                ,   tb_string_cstr(&descriptor));

                // exit type_name
                tb_string_exit(&type_name);

                // exit descriptor
                tb_string_exit(&descriptor);
            } 
            else tb_string_cstrfcat(index_info, "<method?>");
        }
        break;
    case DX_INSTR_INDEX_TYPE_FIELD_REF:
        {
            if (index < dexfile->header->field_ids_size) 
            {
                // get the field id
                dx_field_id_ref_t field_id = dx_file_get_field_id(dexfile, index);
                tb_assert(field_id);

                // init names
                tb_string_t type_name, classname;
                tb_string_init(&type_name);
                tb_string_init(&classname);

                // make info
                tb_string_cstrfcat( index_info
                                ,   "%s.%s:%s"
                                ,   dx_descriptor_class_name(dx_file_get_string_by_type_idx(dexfile, field_id->class_idx), &classname)
                                ,   dx_file_get_string(dexfile, field_id->name_idx)
                                ,   dx_descriptor_type_short(dx_file_get_string_by_type_idx(dexfile, field_id->type_idx), &type_name));

                // exit names
                tb_string_exit(&type_name);
                tb_string_exit(&classname);
            } 
            else tb_string_cstrfcat(index_info, "<field?>");
        }
        break;
    case DX_INSTR_INDEX_TYPE_INLINE_METHOD:
        tb_string_cstrfcat(index_info, "[%02x%02x] // inline", width, index);
        break;
    case DX_INSTR_INDEX_TYPE_VTABLE_OFFSET:
        tb_string_cstrfcat(index_info, "[%02x%02x] // vtable", width, index);
        break;
    case DX_INSTR_INDEX_TYPE_FIELD_OFFSET:
        tb_string_cstrfcat(index_info, "[obj+%02x%02x]", width, index);
        break;
    case DX_INSTR_INDEX_TYPE_METHOD_AND_PROTO_REF:
        tb_string_cstrfcat(index_info, "<method-proto-ref?>");
        break;
    case DX_INSTR_INDEX_TYPE_CALL_SITE_REF:
        tb_string_cstrfcat(index_info, "<call-site-ref?>");
        break;
    case DX_INSTR_INDEX_TYPE_METHOD_HANDLE_REF:
        tb_string_cstrfcat(index_info, "<method-handle-ref?>");
        break;
    case DX_INSTR_INDEX_TYPE_PROTO_REF:
        tb_string_cstrfcat(index_info, "<proto-ref?>");
        break;
    default:
        tb_string_cstrfcat(index_info, "<?>");
        break;
    }
    return tb_string_size(index_info)? tb_string_cstr(index_info) : tb_null;
}
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
tb_uint8_t dx_instr_width(tb_uint16_t const* instr)
{
    // check
    tb_assert(instr);

    // get width
    tb_uint8_t  width = 0;
    tb_uint16_t instr_unit = DX_FETCH_u2(instr, 0);
    if (instr_unit == DX_INSTR_IDENT_PACKED_SWITCH_PAYLOAD) 
        width = 4 + (DX_FETCH_u2(instr, 1) << 1);
    else if (instr_unit == DX_INSTR_IDENT_SPARSE_SWITCH_PAYLOAD)
        width = 2 + (DX_FETCH_u2(instr, 1) << 2);
    else if (instr_unit == DX_INSTR_IDENT_FILL_ARRAY_DATA)
    {
        // the plus 1 is to round up for odd size and width.
        tb_size_t w = DX_FETCH_u2(instr, 1);
        tb_size_t n = DX_FETCH_u2(instr, 2) | (DX_FETCH_u2(instr, 3) << 16);
        tb_assert(4 + (((n * w) + 1) >> 1) <= TB_MAXU32);
        width = (tb_uint8_t)(4 + (((n * w) + 1) >> 1));
    }
    // get width from opcode
    else width = dx_instr_get_width_from_opcode(dx_instr_get_opcode(instr_unit));
    return width;
}
tb_bool_t dx_instr_decode(tb_uint16_t const* instr, dx_instruction_ref_t instruction)
{
    // check
    tb_assert(instr && instruction);

    // get instruction data
    instruction->instr = instr;

    // get instruction code
    tb_uint16_t instr_unit = DX_FETCH_u2(instr, 0);

    // get instruction opcode
    tb_uint16_t opcode = dx_instr_get_opcode(instr_unit);
    instruction->opcode = opcode;

    // get instruction width
    instruction->width = dx_instr_width(instr);

    // get instruction index type
    instruction->index_type = dx_instr_get_index_type_from_opcode(opcode);

    // get instruction format
    instruction->format = dx_instr_get_format_from_opcode(opcode);
    switch (instruction->format)
    {
    case DX_INSTR_FMT_10x:      // op
        {
            // nothing to do; copy the AA bits out for the verifier 
            instruction->vA = DX_INSTR_AA(instr_unit);
        }
        break;
    case DX_INSTR_FMT_12x:      // op vA, vB
        {
            instruction->vA = DX_INSTR_A(instr_unit);
            instruction->vB = DX_INSTR_B(instr_unit);
        }
        break;
    case DX_INSTR_FMT_11n:      // op vA, #+B
        {
            instruction->vA = DX_INSTR_A(instr_unit);
            
            // sign extend 4-bit value
            instruction->vB = (tb_sint32_t)(DX_INSTR_B(instr_unit) << 28) >> 28; 
        }
        break;
    case DX_INSTR_FMT_11x:      // op vAA
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
        }
        break;
    case DX_INSTR_FMT_10t:      // op +AA
        {
            // sign-extend 8-bit value
            instruction->vA = (tb_sint8_t)DX_INSTR_AA(instr_unit);              
        }
        break;
    case DX_INSTR_FMT_20t:      // op +AAAA
        {
            // sign-extend 16-bit value
            instruction->vA = (tb_sint16_t)DX_FETCH_u2(instr, 1);
        }
        break;
    case DX_INSTR_FMT_20bc:     // [opt] op AA, thing@BBBB
    case DX_INSTR_FMT_21c:      // op vAA, thing@BBBB
    case DX_INSTR_FMT_22x:      // op vAA, vBBBB
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
            instruction->vB = DX_FETCH_u2(instr, 1);
        }
        break;
    case DX_INSTR_FMT_21s:      // op vAA, #+BBBB
    case DX_INSTR_FMT_21t:      // op vAA, +BBBB
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
            
            // sign-extend 16-bit value
            instruction->vB = (tb_sint16_t)DX_FETCH_u2(instr, 1);                   
        }
        break;
    case DX_INSTR_FMT_21h:      // op vAA, #+BBBB0000[00000000]
        {
            instruction->vA = DX_INSTR_AA(instr_unit);

            /* the value should be treated as right-zero-extended, but we don't actually do that here.
             * among other things, we don't know if it's the top bits of a 32-bit or 64-bit value.
             */
            instruction->vB = DX_FETCH_u2(instr, 1);
        }
        break;
    case DX_INSTR_FMT_23x:      // op vAA, vBB, vCC
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
            instruction->vB = DX_FETCH_u2(instr, 1) & 0xff;
            instruction->vC = DX_FETCH_u2(instr, 1) >> 8;
        }
        break;
    case DX_INSTR_FMT_22b:      // op vAA, vBB, #+CC
        {
            // sign-extend 8-bit value
            instruction->vA = DX_INSTR_AA(instr_unit);
            instruction->vB = DX_FETCH_u2(instr, 1) & 0xff;
            instruction->vC = (tb_sint8_t)(DX_FETCH_u2(instr, 1) >> 8);
        }
        break;
    case DX_INSTR_FMT_22s:      // op vA, vB, #+CCCC
    case DX_INSTR_FMT_22t:      // op vA, vB, +CCCC
        {
            instruction->vA = DX_INSTR_A(instr_unit);
            instruction->vB = DX_INSTR_B(instr_unit); 
            
            // sign-extend 16-bit value
            instruction->vC = (tb_sint16_t) DX_FETCH_u2(instr, 1);
        }
        break;
    case DX_INSTR_FMT_22c:      // op vA, vB, thing@CCCC
    case DX_INSTR_FMT_22cs:     // [opt] op vA, vB, field offset CCCC
        {
            instruction->vA = DX_INSTR_A(instr_unit);
            instruction->vB = DX_INSTR_B(instr_unit);
            instruction->vC = DX_FETCH_u2(instr, 1);
        }
        break;
    case DX_INSTR_FMT_30t:      // op +AAAAAAAA
        {
            // signed 32-bit value
            instruction->vA = DX_FETCH_u4(instr, 1);                     
        }
        break;
    case DX_INSTR_FMT_31t:      // op vAA, +BBBBBBBB
    case DX_INSTR_FMT_31c:      // op vAA, string@BBBBBBBB
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
                     
            // 32-bit value
            instruction->vB = DX_FETCH_u4(instr, 1);
        }
        break;
    case DX_INSTR_FMT_32x:      // op vAAAA, vBBBB
        {
            instruction->vA = DX_FETCH_u2(instr, 1);
            instruction->vB = DX_FETCH_u2(instr, 2);
        }
        break;
    case DX_INSTR_FMT_31i:      // op vAA, #+BBBBBBBB
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
                    
            // signed 32-bit value
            instruction->vB = DX_FETCH_u4(instr, 1);
        }
        break;
    case DX_INSTR_FMT_35c:      // op {vC, vD, vE, vF, vG}, thing@BBBB
    case DX_INSTR_FMT_35ms:     // [opt] invoke-virtual+super
    case DX_INSTR_FMT_35mi:     // [opt] inline invoke
        {
            // get vA/B
            instruction->vA = DX_INSTR_B(instr_unit);
            instruction->vB = DX_FETCH_u2(instr, 1);

            // get registers
            tb_uint16_t regs = DX_FETCH_u2(instr, 2);

            // copy the argument registers into the arg[] array, and also copy the first argument (if any) into vC. 
            tb_uint32_t count = instruction->vA;
            switch (count) 
            {
            case 5: 
                {
                    // a fifth argument is verboten for inline invokes.
                    if (instruction->format == DX_INSTR_FMT_35mi) 
                    {
                        // trace
                        tb_trace_e("Invalid arg count in 35mi (5)");

                        // failed
                        tb_assert(0);
                        return tb_false;
                    }

                    /* per note at the top of this format decoder, 
                     * the fifth argument comes from the A field in the instruction, 
                     * but it's labeled G in the spec.
                     */
                    instruction->arg[4] = DX_INSTR_A(instr_unit);
            }
            case 4: instruction->arg[3] = (regs >> 12) & 0x0f;
            case 3: instruction->arg[2] = (regs >> 8) & 0x0f;
            case 2: instruction->arg[1] = (regs >> 4) & 0x0f;
            case 1: instruction->vC = instruction->arg[0] = regs & 0x0f; break;
            case 0: break; // valid, but no need to do anything.
            default:
                {
                    // trace
                    tb_trace_e("invalid argument count %u in 35c/35ms/35mi", count);

                    // failed
                    tb_assert(0);
                    return tb_false;
                }
            }
        }
        break;
    case DX_INSTR_FMT_3rc:      // op {vCCCC .. v(CCCC+AA-1)}, meth@BBBB
    case DX_INSTR_FMT_3rms:     // [opt] invoke-virtual+super/range
    case DX_INSTR_FMT_3rmi:     // [opt] execute-inline/range
        {
            instruction->vA = DX_INSTR_AA(instr_unit);
            instruction->vB = DX_FETCH_u2(instr, 1);
            instruction->vC = DX_FETCH_u2(instr, 2);
        }
        break;
    case DX_INSTR_FMT_51l:      // op vAA, #+BBBBBBBBBBBBBBBB
        {
            instruction->vA       = DX_INSTR_AA(instr_unit);
            instruction->vB_wide  = DX_FETCH_u4(instr, 1) | ((tb_uint64_t)DX_FETCH_u4(instr, 3) << 32);
        }
        break;
    default:
        {
            // trace
            tb_trace_e("invalid instruction, code: %x, format: %lu, opcode: %u", instr_unit, instruction->format, opcode);

            // failed
            tb_assert(0);
            return tb_false;
        }
    }
    return tb_true;
}
tb_long_t dx_instr_packed_switch(tb_uint16_t const* instr, tb_sint32_t value)
{
    // check
    tb_assert(instr);

    /* done
     *
     * packed-switch-payload format:
     *  u2 ident            magic value(DX_INSTR_IDENT_PACKED_SWITCH_PAYLOAD)
     *  u2 size             number of entries in the table
     *  s4 first_key        first (and lowest) switch case value
     *  s4 targets[size]    branch targets, relative to switch opcode
     *
     * total size is (4 + size * 2) 16-bit code units.
     */
    tb_bool_t ok = tb_false;
    tb_long_t offset = 0;
    do
    {
        // get ident
        tb_uint16_t ident = DX_FETCH_u2(instr, 0);
        tb_assert_and_check_break(ident == DX_INSTR_IDENT_PACKED_SWITCH_PAYLOAD);
        
        // get size
        tb_uint16_t size = DX_FETCH_u2(instr, 1);
        tb_assert_and_check_break(size);

        // get first key
        tb_sint32_t first_key = DX_FETCH_u2(instr, 2);
        first_key |= DX_FETCH_u2(instr, 3) << 16;

        // calculate index
        tb_long_t index = value - first_key;
        tb_check_break(index >= 0 && index < size);

        // get offset
        offset = (tb_sint32_t)DX_FETCH_u4(instr, 4 + (index << 1));

        // ok
        ok = tb_true;
    
    } while (0);

    // failed? return the size of the packed-switch instruction
    if (!ok) offset = 3;

    // ok?
    return offset;
}
#ifdef DX_DUMP_ENABLE
tb_void_t dx_instr_dump(dx_instruction_ref_t instruction, tb_size_t instr_idx, dx_file_ref_t file)
{
    // check
    tb_assert(instruction);

    // get the instruction data
    tb_uint16_t const* instr = instruction->instr;

    // trace the instruction offset
    tb_printf("        %#08x:", (tb_byte_t const*)instr - dx_file_data(file));

    // dump instruction bytes
    tb_size_t i = 0;
    for (i = 0; i < 8; i++) 
    {
        if (i < instruction->width) 
        {
            if (i == 7) 
            {
                tb_printf(" ... ");
            }
            else
            {
                tb_byte_t const* p = (tb_byte_t const*)&instr[i];
                tb_printf(" %02x%02x", p[0], p[1]);
            }
        } 
        else tb_printf("     ");
    }

    // dump opcode name
    if (instruction->opcode == DX_OPCODE_NOP)
    {
        // get instruction code
        tb_uint16_t instr_unit = DX_FETCH_u2(instr, 0);
        if (instr_unit == DX_INSTR_IDENT_PACKED_SWITCH_PAYLOAD)
            tb_printf("|%04x: packed-switch-data (%u bytes)", instr_idx, instruction->width);
        else if (instr_unit == DX_INSTR_IDENT_SPARSE_SWITCH_PAYLOAD)
            tb_printf("|%04x: sparse-switch-data (%u bytes)", instr_idx, instruction->width);
        else if (instr_unit == DX_INSTR_IDENT_FILL_ARRAY_DATA) 
            tb_printf("|%04x: fill-array-data (%u bytes)", instr_idx, instruction->width);
        else tb_printf("|%04x: nop", instr_idx);
    } 
    else tb_printf("|%04x: %s", instr_idx, dx_opcode_name(instruction->opcode));

    // get index info
    tb_string_t         index_data;
    tb_char_t const*    index_info = tb_null;
    tb_string_init(&index_data);
    if (instruction->index_type != DX_INSTR_INDEX_TYPE_NONE)
        index_info = dx_instr_get_index_info(file, instruction, &index_data);

    // dump opcode descriptor
    switch (dx_instr_get_format_from_opcode(instruction->opcode)) 
    {
    case DX_INSTR_FMT_10x:      // op
        break;
    case DX_INSTR_FMT_12x:      // op vA, vB
        tb_printf(" v%d, v%d", instruction->vA, instruction->vB);
        break;
    case DX_INSTR_FMT_11n:      // op vA, #+B
        tb_printf(" v%d, #%d // #%x", instruction->vA, (tb_sint32_t)instruction->vB, (tb_uint8_t)instruction->vB);
        break;
    case DX_INSTR_FMT_11x:      // op vAA
        tb_printf(" v%d", instruction->vA);
        break;
    case DX_INSTR_FMT_10t:      // op +AA
    case DX_INSTR_FMT_20t:      // op +AAAA
        {
            tb_sint32_t targ = (tb_sint32_t) instruction->vA;
            tb_printf(" %04x // %c%04x", instr_idx + targ, (targ < 0) ? '-' : '+', (targ < 0) ? -targ : targ);
        }
        break;
    case DX_INSTR_FMT_22x:      // op vAA, vBBBB
        tb_printf(" v%d, v%d", instruction->vA, instruction->vB);
        break;
    case DX_INSTR_FMT_21t:      // op vAA, +BBBB
        {
            tb_sint32_t targ = (tb_sint32_t) instruction->vB;
            tb_printf(" v%d, %04x // %c%04x", instruction->vA, instr_idx + targ, (targ < 0) ? '-' : '+', (targ < 0) ? -targ : targ);
        }
        break;
    case DX_INSTR_FMT_21s:      // op vAA, #+BBBB
        tb_printf(" v%d, #%d // #%x", instruction->vA, (tb_sint32_t)instruction->vB, (tb_uint16_t)instruction->vB);
        break;
    case DX_INSTR_FMT_21h:      // op vAA, #+BBBB0000[00000000]
        {
            // the printed format varies a bit based on the actual opcode.
            if (instruction->opcode == DX_OPCODE_CONST_HIGH16) 
            {
                tb_uint32_t value = instruction->vB << 16;
                tb_printf(" v%d, #%x // #%x", instruction->vA, value, instruction->vB);
            } 
            else 
            {
                tb_uint64_t value = ((tb_uint64_t)instruction->vB) << 48;
                tb_printf(" v%d, #%llx // #%x", instruction->vA, value, (tb_uint16_t)instruction->vB);
            }
        }
        break;
    case DX_INSTR_FMT_21c:      // op vAA, thing@BBBB
    case DX_INSTR_FMT_31c:      // op vAA, thing@BBBBBBBB
        tb_printf(" v%d, %s", instruction->vA, index_info);
        break;
    case DX_INSTR_FMT_23x:      // op vAA, vBB, vCC
        tb_printf(" v%d, v%d, v%d", instruction->vA, instruction->vB, instruction->vC);
        break;
    case DX_INSTR_FMT_22b:      // op vAA, vBB, #+CC
        tb_printf(" v%d, v%d, #%d // #%02x", instruction->vA, instruction->vB, (tb_sint32_t)instruction->vC, (tb_uint8_t)instruction->vC);
        break;
    case DX_INSTR_FMT_22t:      // op vA, vB, +CCCC
        {
            tb_sint32_t targ = (tb_sint32_t) instruction->vC;
            tb_printf(" v%d, v%d, %04x // %c%04x", instruction->vA, instruction->vB, instr_idx + targ, (targ < 0) ? '-' : '+', (targ < 0) ? -targ : targ);
        }
        break;
    case DX_INSTR_FMT_22s:      // op vA, vB, #+CCCC
        tb_printf(" v%d, v%d, #%d // #%04x", instruction->vA, instruction->vB, (tb_sint32_t)instruction->vC, (tb_uint16_t)instruction->vC);
        break;
    case DX_INSTR_FMT_22c:      // op vA, vB, thing@CCCC
    case DX_INSTR_FMT_22cs:     // [opt] op vA, vB, field offset CCCC
        tb_printf(" v%d, v%d, %s", instruction->vA, instruction->vB, index_info);
        break;
    case DX_INSTR_FMT_30t:
        tb_printf(" #%08x", instruction->vA);
        break;
    case DX_INSTR_FMT_31i:      // op vAA, #+BBBBBBBB
        tb_printf(" v%d, #%08x", instruction->vA, instruction->vB);
        break;
    case DX_INSTR_FMT_31t:      // op vAA, offset +BBBBBBBB
        tb_printf(" v%d, %08x // +%08x", instruction->vA, instr_idx + instruction->vB, instruction->vB);
        break;
    case DX_INSTR_FMT_32x:      // op vAAAA, vBBBB
        tb_printf(" v%d, v%d", instruction->vA, instruction->vB);
        break;
    case DX_INSTR_FMT_35c:      // op {vC, vD, vE, vF, vG}, thing@BBBB
    case DX_INSTR_FMT_35ms:     // [opt] invoke-virtual+super
    case DX_INSTR_FMT_35mi:     // [opt] inline invoke
        {
            tb_printf(" {");
            for (i = 0; i < (tb_int_t)instruction->vA; i++)
            {
                if (i == 0) tb_printf("v%d", instruction->arg[i]);
                else tb_printf(", v%d", instruction->arg[i]);
            }
            tb_printf("}, %s", index_info);
        }
        break;
    case DX_INSTR_FMT_3rc:      // op {vCCCC .. v(CCCC+AA-1)}, thing@BBBB
    case DX_INSTR_FMT_3rms:     // [opt] invoke-virtual+super/range
    case DX_INSTR_FMT_3rmi:     // [opt] execute-inline/range
        {
            /* this doesn't match the "dx" output when some of the args are
             * 64-bit values -- dx only shows the first register.
             */
            tb_printf(" {");
            for (i = 0; i < (tb_int_t) instruction->vA; i++) 
            {
                if (i == 0) tb_printf("v%d", instruction->vC + i);
                else tb_printf(", v%d", instruction->vC + i);
            }
            tb_printf("}, %s", index_info);
        }
        break;
    case DX_INSTR_FMT_51l:      // op vAA, #+BBBBBBBBBBBBBBBB
        tb_printf(" v%d, #%llx", instruction->vA, instruction->vB_wide);
        break;
    case DX_INSTR_FMT_00x:      // unknown op or breakpoint
        break;
    default:
        tb_printf(" ???");
        break;
    }

    // end
    tb_printf("\n");

    // exit it
    tb_string_exit(&index_data);
}
#endif
