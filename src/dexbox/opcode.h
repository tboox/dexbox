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
 * @file        opcode.h
 *
 */
#ifndef DX_OPCODE_H
#define DX_OPCODE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/*
 * DX_OPCODE_MAX_VALUE:     the highest possible raw (unpacked) opcode value
 * DX_OPCODE_PACKED_MAXN:   the highest possible packed opcode value of a valid dalvik opcode, plus one
 */
// BEGIN(dexbox-maximum-values)
#define DX_OPCODE_VALUE_MAX     (0xffff)
#define DX_OPCODE_PACKED_MAXN   (0x100)
// END(dexbox-maximum-values)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the dex opcode enum
typedef enum dx_opcode_e 
{
    // BEGIN(dexbox-opcode-enum)
    DX_OPCODE_NOP                          = 0x00,
    DX_OPCODE_MOVE                         = 0x01,
    DX_OPCODE_MOVE_FROM16                  = 0x02,
    DX_OPCODE_MOVE_16                      = 0x03,
    DX_OPCODE_MOVE_WIDE                    = 0x04,
    DX_OPCODE_MOVE_WIDE_FROM16             = 0x05,
    DX_OPCODE_MOVE_WIDE_16                 = 0x06,
    DX_OPCODE_MOVE_OBJECT                  = 0x07,
    DX_OPCODE_MOVE_OBJECT_FROM16           = 0x08,
    DX_OPCODE_MOVE_OBJECT_16               = 0x09,
    DX_OPCODE_MOVE_RESULT                  = 0x0a,
    DX_OPCODE_MOVE_RESULT_WIDE             = 0x0b,
    DX_OPCODE_MOVE_RESULT_OBJECT           = 0x0c,
    DX_OPCODE_MOVE_EXCEPTION               = 0x0d,
    DX_OPCODE_RETURN_VOID                  = 0x0e,
    DX_OPCODE_RETURN                       = 0x0f,
    DX_OPCODE_RETURN_WIDE                  = 0x10,
    DX_OPCODE_RETURN_OBJECT                = 0x11,
    DX_OPCODE_CONST_4                      = 0x12,
    DX_OPCODE_CONST_16                     = 0x13,
    DX_OPCODE_CONST                        = 0x14,
    DX_OPCODE_CONST_HIGH16                 = 0x15,
    DX_OPCODE_CONST_WIDE_16                = 0x16,
    DX_OPCODE_CONST_WIDE_32                = 0x17,
    DX_OPCODE_CONST_WIDE                   = 0x18,
    DX_OPCODE_CONST_WIDE_HIGH16            = 0x19,
    DX_OPCODE_CONST_STRING                 = 0x1a,
    DX_OPCODE_CONST_STRING_JUMBO           = 0x1b,
    DX_OPCODE_CONST_CLASS                  = 0x1c,
    DX_OPCODE_MONITOR_ENTER                = 0x1d,
    DX_OPCODE_MONITOR_EXIT                 = 0x1e,
    DX_OPCODE_CHECK_CAST                   = 0x1f,
    DX_OPCODE_INSTANCE_OF                  = 0x20,
    DX_OPCODE_ARRAY_LENGTH                 = 0x21,
    DX_OPCODE_NEW_INSTANCE                 = 0x22,
    DX_OPCODE_NEW_ARRAY                    = 0x23,
    DX_OPCODE_FILLED_NEW_ARRAY             = 0x24,
    DX_OPCODE_FILLED_NEW_ARRAY_RANGE       = 0x25,
    DX_OPCODE_FILL_ARRAY_DATA              = 0x26,
    DX_OPCODE_THROW                        = 0x27,
    DX_OPCODE_GOTO                         = 0x28,
    DX_OPCODE_GOTO_16                      = 0x29,
    DX_OPCODE_GOTO_32                      = 0x2a,
    DX_OPCODE_PACKED_SWITCH                = 0x2b,
    DX_OPCODE_SPARSE_SWITCH                = 0x2c,
    DX_OPCODE_CMPL_FLOAT                   = 0x2d,
    DX_OPCODE_CMPG_FLOAT                   = 0x2e,
    DX_OPCODE_CMPL_DOUBLE                  = 0x2f,
    DX_OPCODE_CMPG_DOUBLE                  = 0x30,
    DX_OPCODE_CMP_LONG                     = 0x31,
    DX_OPCODE_IF_EQ                        = 0x32,
    DX_OPCODE_IF_NE                        = 0x33,
    DX_OPCODE_IF_LT                        = 0x34,
    DX_OPCODE_IF_GE                        = 0x35,
    DX_OPCODE_IF_GT                        = 0x36,
    DX_OPCODE_IF_LE                        = 0x37,
    DX_OPCODE_IF_EQZ                       = 0x38,
    DX_OPCODE_IF_NEZ                       = 0x39,
    DX_OPCODE_IF_LTZ                       = 0x3a,
    DX_OPCODE_IF_GEZ                       = 0x3b,
    DX_OPCODE_IF_GTZ                       = 0x3c,
    DX_OPCODE_IF_LEZ                       = 0x3d,
    DX_OPCODE_UNUSED_3E                    = 0x3e,
    DX_OPCODE_UNUSED_3F                    = 0x3f,
    DX_OPCODE_UNUSED_40                    = 0x40,
    DX_OPCODE_UNUSED_41                    = 0x41,
    DX_OPCODE_UNUSED_42                    = 0x42,
    DX_OPCODE_UNUSED_43                    = 0x43,
    DX_OPCODE_AGET                         = 0x44,
    DX_OPCODE_AGET_WIDE                    = 0x45,
    DX_OPCODE_AGET_OBJECT                  = 0x46,
    DX_OPCODE_AGET_BOOLEAN                 = 0x47,
    DX_OPCODE_AGET_BYTE                    = 0x48,
    DX_OPCODE_AGET_CHAR                    = 0x49,
    DX_OPCODE_AGET_SHORT                   = 0x4a,
    DX_OPCODE_APUT                         = 0x4b,
    DX_OPCODE_APUT_WIDE                    = 0x4c,
    DX_OPCODE_APUT_OBJECT                  = 0x4d,
    DX_OPCODE_APUT_BOOLEAN                 = 0x4e,
    DX_OPCODE_APUT_BYTE                    = 0x4f,
    DX_OPCODE_APUT_CHAR                    = 0x50,
    DX_OPCODE_APUT_SHORT                   = 0x51,
    DX_OPCODE_IGET                         = 0x52,
    DX_OPCODE_IGET_WIDE                    = 0x53,
    DX_OPCODE_IGET_OBJECT                  = 0x54,
    DX_OPCODE_IGET_BOOLEAN                 = 0x55,
    DX_OPCODE_IGET_BYTE                    = 0x56,
    DX_OPCODE_IGET_CHAR                    = 0x57,
    DX_OPCODE_IGET_SHORT                   = 0x58,
    DX_OPCODE_IPUT                         = 0x59,
    DX_OPCODE_IPUT_WIDE                    = 0x5a,
    DX_OPCODE_IPUT_OBJECT                  = 0x5b,
    DX_OPCODE_IPUT_BOOLEAN                 = 0x5c,
    DX_OPCODE_IPUT_BYTE                    = 0x5d,
    DX_OPCODE_IPUT_CHAR                    = 0x5e,
    DX_OPCODE_IPUT_SHORT                   = 0x5f,
    DX_OPCODE_SGET                         = 0x60,
    DX_OPCODE_SGET_WIDE                    = 0x61,
    DX_OPCODE_SGET_OBJECT                  = 0x62,
    DX_OPCODE_SGET_BOOLEAN                 = 0x63,
    DX_OPCODE_SGET_BYTE                    = 0x64,
    DX_OPCODE_SGET_CHAR                    = 0x65,
    DX_OPCODE_SGET_SHORT                   = 0x66,
    DX_OPCODE_SPUT                         = 0x67,
    DX_OPCODE_SPUT_WIDE                    = 0x68,
    DX_OPCODE_SPUT_OBJECT                  = 0x69,
    DX_OPCODE_SPUT_BOOLEAN                 = 0x6a,
    DX_OPCODE_SPUT_BYTE                    = 0x6b,
    DX_OPCODE_SPUT_CHAR                    = 0x6c,
    DX_OPCODE_SPUT_SHORT                   = 0x6d,
    DX_OPCODE_INVOKE_VIRTUAL               = 0x6e,
    DX_OPCODE_INVOKE_SUPER                 = 0x6f,
    DX_OPCODE_INVOKE_DIRECT                = 0x70,
    DX_OPCODE_INVOKE_STATIC                = 0x71,
    DX_OPCODE_INVOKE_INTERFACE             = 0x72,
    DX_OPCODE_UNUSED_73                    = 0x73,
    DX_OPCODE_INVOKE_VIRTUAL_RANGE         = 0x74,
    DX_OPCODE_INVOKE_SUPER_RANGE           = 0x75,
    DX_OPCODE_INVOKE_DIRECT_RANGE          = 0x76,
    DX_OPCODE_INVOKE_STATIC_RANGE          = 0x77,
    DX_OPCODE_INVOKE_INTERFACE_RANGE       = 0x78,
    DX_OPCODE_UNUSED_79                    = 0x79,
    DX_OPCODE_UNUSED_7A                    = 0x7a,
    DX_OPCODE_NEG_INT                      = 0x7b,
    DX_OPCODE_NOT_INT                      = 0x7c,
    DX_OPCODE_NEG_LONG                     = 0x7d,
    DX_OPCODE_NOT_LONG                     = 0x7e,
    DX_OPCODE_NEG_FLOAT                    = 0x7f,
    DX_OPCODE_NEG_DOUBLE                   = 0x80,
    DX_OPCODE_INT_TO_LONG                  = 0x81,
    DX_OPCODE_INT_TO_FLOAT                 = 0x82,
    DX_OPCODE_INT_TO_DOUBLE                = 0x83,
    DX_OPCODE_LONG_TO_INT                  = 0x84,
    DX_OPCODE_LONG_TO_FLOAT                = 0x85,
    DX_OPCODE_LONG_TO_DOUBLE               = 0x86,
    DX_OPCODE_FLOAT_TO_INT                 = 0x87,
    DX_OPCODE_FLOAT_TO_LONG                = 0x88,
    DX_OPCODE_FLOAT_TO_DOUBLE              = 0x89,
    DX_OPCODE_DOUBLE_TO_INT                = 0x8a,
    DX_OPCODE_DOUBLE_TO_LONG               = 0x8b,
    DX_OPCODE_DOUBLE_TO_FLOAT              = 0x8c,
    DX_OPCODE_INT_TO_BYTE                  = 0x8d,
    DX_OPCODE_INT_TO_CHAR                  = 0x8e,
    DX_OPCODE_INT_TO_SHORT                 = 0x8f,
    DX_OPCODE_ADD_INT                      = 0x90,
    DX_OPCODE_SUB_INT                      = 0x91,
    DX_OPCODE_MUL_INT                      = 0x92,
    DX_OPCODE_DIV_INT                      = 0x93,
    DX_OPCODE_REM_INT                      = 0x94,
    DX_OPCODE_AND_INT                      = 0x95,
    DX_OPCODE_OR_INT                       = 0x96,
    DX_OPCODE_XOR_INT                      = 0x97,
    DX_OPCODE_SHL_INT                      = 0x98,
    DX_OPCODE_SHR_INT                      = 0x99,
    DX_OPCODE_USHR_INT                     = 0x9a,
    DX_OPCODE_ADD_LONG                     = 0x9b,
    DX_OPCODE_SUB_LONG                     = 0x9c,
    DX_OPCODE_MUL_LONG                     = 0x9d,
    DX_OPCODE_DIV_LONG                     = 0x9e,
    DX_OPCODE_REM_LONG                     = 0x9f,
    DX_OPCODE_AND_LONG                     = 0xa0,
    DX_OPCODE_OR_LONG                      = 0xa1,
    DX_OPCODE_XOR_LONG                     = 0xa2,
    DX_OPCODE_SHL_LONG                     = 0xa3,
    DX_OPCODE_SHR_LONG                     = 0xa4,
    DX_OPCODE_USHR_LONG                    = 0xa5,
    DX_OPCODE_ADD_FLOAT                    = 0xa6,
    DX_OPCODE_SUB_FLOAT                    = 0xa7,
    DX_OPCODE_MUL_FLOAT                    = 0xa8,
    DX_OPCODE_DIV_FLOAT                    = 0xa9,
    DX_OPCODE_REM_FLOAT                    = 0xaa,
    DX_OPCODE_ADD_DOUBLE                   = 0xab,
    DX_OPCODE_SUB_DOUBLE                   = 0xac,
    DX_OPCODE_MUL_DOUBLE                   = 0xad,
    DX_OPCODE_DIV_DOUBLE                   = 0xae,
    DX_OPCODE_REM_DOUBLE                   = 0xaf,
    DX_OPCODE_ADD_INT_2ADDR                = 0xb0,
    DX_OPCODE_SUB_INT_2ADDR                = 0xb1,
    DX_OPCODE_MUL_INT_2ADDR                = 0xb2,
    DX_OPCODE_DIV_INT_2ADDR                = 0xb3,
    DX_OPCODE_REM_INT_2ADDR                = 0xb4,
    DX_OPCODE_AND_INT_2ADDR                = 0xb5,
    DX_OPCODE_OR_INT_2ADDR                 = 0xb6,
    DX_OPCODE_XOR_INT_2ADDR                = 0xb7,
    DX_OPCODE_SHL_INT_2ADDR                = 0xb8,
    DX_OPCODE_SHR_INT_2ADDR                = 0xb9,
    DX_OPCODE_USHR_INT_2ADDR               = 0xba,
    DX_OPCODE_ADD_LONG_2ADDR               = 0xbb,
    DX_OPCODE_SUB_LONG_2ADDR               = 0xbc,
    DX_OPCODE_MUL_LONG_2ADDR               = 0xbd,
    DX_OPCODE_DIV_LONG_2ADDR               = 0xbe,
    DX_OPCODE_REM_LONG_2ADDR               = 0xbf,
    DX_OPCODE_AND_LONG_2ADDR               = 0xc0,
    DX_OPCODE_OR_LONG_2ADDR                = 0xc1,
    DX_OPCODE_XOR_LONG_2ADDR               = 0xc2,
    DX_OPCODE_SHL_LONG_2ADDR               = 0xc3,
    DX_OPCODE_SHR_LONG_2ADDR               = 0xc4,
    DX_OPCODE_USHR_LONG_2ADDR              = 0xc5,
    DX_OPCODE_ADD_FLOAT_2ADDR              = 0xc6,
    DX_OPCODE_SUB_FLOAT_2ADDR              = 0xc7,
    DX_OPCODE_MUL_FLOAT_2ADDR              = 0xc8,
    DX_OPCODE_DIV_FLOAT_2ADDR              = 0xc9,
    DX_OPCODE_REM_FLOAT_2ADDR              = 0xca,
    DX_OPCODE_ADD_DOUBLE_2ADDR             = 0xcb,
    DX_OPCODE_SUB_DOUBLE_2ADDR             = 0xcc,
    DX_OPCODE_MUL_DOUBLE_2ADDR             = 0xcd,
    DX_OPCODE_DIV_DOUBLE_2ADDR             = 0xce,
    DX_OPCODE_REM_DOUBLE_2ADDR             = 0xcf,
    DX_OPCODE_ADD_INT_LIT16                = 0xd0,
    DX_OPCODE_RSUB_INT                     = 0xd1,
    DX_OPCODE_MUL_INT_LIT16                = 0xd2,
    DX_OPCODE_DIV_INT_LIT16                = 0xd3,
    DX_OPCODE_REM_INT_LIT16                = 0xd4,
    DX_OPCODE_AND_INT_LIT16                = 0xd5,
    DX_OPCODE_OR_INT_LIT16                 = 0xd6,
    DX_OPCODE_XOR_INT_LIT16                = 0xd7,
    DX_OPCODE_ADD_INT_LIT8                 = 0xd8,
    DX_OPCODE_RSUB_INT_LIT8                = 0xd9,
    DX_OPCODE_MUL_INT_LIT8                 = 0xda,
    DX_OPCODE_DIV_INT_LIT8                 = 0xdb,
    DX_OPCODE_REM_INT_LIT8                 = 0xdc,
    DX_OPCODE_AND_INT_LIT8                 = 0xdd,
    DX_OPCODE_OR_INT_LIT8                  = 0xde,
    DX_OPCODE_XOR_INT_LIT8                 = 0xdf,
    DX_OPCODE_SHL_INT_LIT8                 = 0xe0,
    DX_OPCODE_SHR_INT_LIT8                 = 0xe1,
    DX_OPCODE_USHR_INT_LIT8                = 0xe2,
    DX_OPCODE_IGET_VOLATILE                = 0xe3,
    DX_OPCODE_IPUT_VOLATILE                = 0xe4,
    DX_OPCODE_SGET_VOLATILE                = 0xe5,
    DX_OPCODE_SPUT_VOLATILE                = 0xe6,
    DX_OPCODE_IGET_OBJECT_VOLATILE         = 0xe7,
    DX_OPCODE_IGET_WIDE_VOLATILE           = 0xe8,
    DX_OPCODE_IPUT_WIDE_VOLATILE           = 0xe9,
    DX_OPCODE_SGET_WIDE_VOLATILE           = 0xea,
    DX_OPCODE_SPUT_WIDE_VOLATILE           = 0xeb,
    DX_OPCODE_BREAKPOINT                   = 0xec,
    DX_OPCODE_THROW_VERIFICATION_ERROR     = 0xed,
    DX_OPCODE_EXECUTE_INLINE               = 0xee,
    DX_OPCODE_EXECUTE_INLINE_RANGE         = 0xef,
    DX_OPCODE_INVOKE_OBJECT_INIT_RANGE     = 0xf0,
    DX_OPCODE_RETURN_VOID_BARRIER          = 0xf1,
    DX_OPCODE_IGET_QUICK                   = 0xf2,
    DX_OPCODE_UNUSED_F3                    = 0xf3,
    DX_OPCODE_UNUSED_F4                    = 0xf4,
    DX_OPCODE_UNUSED_F5                    = 0xf5,
    DX_OPCODE_UNUSED_F6                    = 0xf6,
    DX_OPCODE_UNUSED_F7                    = 0xf7,
    DX_OPCODE_UNUSED_F8                    = 0xf8,
    DX_OPCODE_UNUSED_F9                    = 0xf9,
    DX_OPCODE_INVOKE_POLYMORPHIC           = 0xfa,
    DX_OPCODE_INVOKE_POLYMORPHIC_RANGE     = 0xfb,
    DX_OPCODE_INVOKE_CUSTOM                = 0xfc,
    DX_OPCODE_INVOKE_CUSTOM_RANGE          = 0xfd,
    DX_OPCODE_CONST_METHOD_HANDLE          = 0xfe,
    DX_OPCODE_CONST_METHOD_TYPE            = 0xff,
    // END(dexbox-opcode-enum)

}dx_opcode_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get the opcode name
 *
 * @param opcode        the dex instruction opcode
 *
 * @return              the opcode name
 */
tb_char_t const*        dx_opcode_name(tb_uint16_t opcode);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


