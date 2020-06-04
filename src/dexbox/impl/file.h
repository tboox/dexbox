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
 * @file        file.h
 *
 */
#ifndef DX_IMPL_FILE_H
#define DX_IMPL_FILE_H

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

// the dex file magic number 
#define DX_MAGIC                    "dex\n"

// the dex file magic version 
#define DX_MAGIC_VERSION            "036\0"

/*! the older dex file magic version 
 *
 * corresponding to Android API levels 13 and earlier
 */
#define DX_MAGIC_VERSION_API_13     "035\0"

// the optimized dex file magic number
#define DX_OPT_MAGIC                "dey\n"

// the optimized dex file magic version
#define DX_OPT_MAGIC_VERSION        "036\0"

/*!
 * 160-bit SHA-1 digest.
 */
#define DX_SHA1_SIZE                (20)
#define DX_SHA1_OUTPUT_SIZE         (DX_SHA1_SIZE * 2 + 1)

// the optimized dex chunk code: CLKP
#define DX_OPT_CHUNK_CLASS_lOOKUP   (0x434c4b50)

// the optimized dex chunk code: RMAP
#define DX_OPT_CHUNK_REGISTER_MAPS  (0x524d4150)

// the optimized dex chunk code: AEND
#define DX_OPT_CHUNK_END            (0x41454e44)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the dex header type
typedef struct __dx_header_t
{
    // the magic includes version number
    tb_uint8_t              magic[8];

    // the adler32 checksum
    tb_uint32_t             checksum;

    // the sha-1 hash
    tb_uint8_t              signature[DX_SHA1_SIZE];

    // the length of the entire file
    tb_uint32_t             file_size;  

    // the offset to start of next section
    tb_uint32_t             header_size;
    tb_uint32_t             endian_tag;
    tb_uint32_t             link_size;
    tb_uint32_t             link_off;
    tb_uint32_t             map_off;
    tb_uint32_t             string_ids_size;
    tb_uint32_t             string_ids_off;
    tb_uint32_t             type_ids_size;
    tb_uint32_t             type_ids_off;
    tb_uint32_t             proto_ids_size;
    tb_uint32_t             proto_ids_off;
    tb_uint32_t             field_ids_size;
    tb_uint32_t             field_ids_off;
    tb_uint32_t             method_ids_size;
    tb_uint32_t             method_ids_off;
    tb_uint32_t             class_defs_size;
    tb_uint32_t             class_defs_off;
    tb_uint32_t             data_size;
    tb_uint32_t             data_off;

}dx_header_t, *dx_header_ref_t;

// the dex header type for the optimized dex file
typedef struct __dx_header_opt_t
{
    // the magic includes version number
    tb_uint8_t              magic[8];

    // the offset of the dex header
    tb_uint32_t             dex_offset;

    // the length of the dex header
    tb_uint32_t             dex_length;

    // the offset of the optimized dex dependency table
    tb_uint32_t             deps_offset;

    // the length of the optimized dex dependency table
    tb_uint32_t             deps_length;

    // the offset of the optimized data tables
    tb_uint32_t             opt_offset;

    // the length of the optimized data tables
    tb_uint32_t             opt_length;

    // the some info flags
    tb_uint32_t             flags;

    // adler32 checksum covering deps/opt
    tb_uint32_t             checksum;

    // pad for 64-bit alignment if necessary

}dx_header_opt_t, *dx_header_opt_ref_t;

// the dex string id type
typedef struct __dx_string_id_t
{
    // file offset to string_data_item
    tb_uint32_t             string_data_off;

}dx_string_id_t, *dx_string_id_ref_t;

// the dex type id type
typedef struct __dx_type_id_t
{
    // index into string_ids list for type descriptor
    tb_uint32_t             descriptor_idx; 

}dx_type_id_t, *dx_type_id_ref_t;

// the dex field id type
typedef struct __dx_field_id_t
{
    // index into type_ids list for defining class
    tb_uint16_t             class_idx;

    // index into type_ids for field type
    tb_uint16_t             type_idx;

    // index into string_ids for field name
    tb_uint32_t             name_idx; 

}dx_field_id_t, *dx_field_id_ref_t;

// the dex method id type
typedef struct __dx_method_id_t
{
    // index into type_ids list for defining class
    tb_uint16_t             class_idx; 

    // index into proto_ids for method prototype
    tb_uint16_t             proto_idx; 

    // index into string_ids for method name
    tb_uint32_t             name_idx;

}dx_method_id_t, *dx_method_id_ref_t;

// the dex proto id type
typedef struct __dx_proto_id_t
{
    // index into string_ids for shorty descriptor
    tb_uint32_t             shorty_idx;

    // index into type_ids list for return type
    tb_uint32_t             return_type_idx;

    // file offset to type_list for parameter types
    tb_uint32_t             parameters_off;

}dx_proto_id_t, *dx_proto_id_ref_t;

/* the dex encoded array ref type
 *
 * encoded_value
 * {
 *     type : 5
 *     size : 3
 *     data
 * }
 *
 * encoded_array
 * {
 *     uleb128          size;
 *     encoded_value    values;
 * }
 *
 */
typedef tb_byte_t*          dx_encoded_array_ref_t;

// the dex class def type
typedef struct __dx_class_def_t
{
    // index into type_ids for this class
    tb_uint32_t             class_idx; 

    // the access flags
    tb_uint32_t             access_flags;

    // index into type_ids for superclass
    tb_uint32_t             superclass_idx; 

    // file offset to dex_type_list 
    tb_uint32_t             interfaces_off; 

    // index into string_ids for source file name
    tb_uint32_t             source_file_idx; 

    // file offset to annotations_directory_item
    tb_uint32_t             annotations_off; 

    // file offset to class_data_item
    tb_uint32_t             class_data_off; 

    // file offset to dex_encoded_array
    tb_uint32_t             static_values_off; 

}dx_class_def_t, *dx_class_def_ref_t;

// the dex link type
typedef struct __dx_link_t
{
    // currently undefined
    tb_uint8_t              bleargh;

}dx_link_t, *dx_link_ref_t;

// the dex class lookup type
typedef struct __dx_class_lookup_t
{
    // total size, including "size"
    tb_int_t                size; 

    // size of table[]; always power of 2
    tb_int_t                num_entries; 

    struct 
    {
        // class descriptor hash code
        tb_uint32_t         class_descriptor_hash; 

        // in bytes, from start of dex
        tb_int_t            class_descriptor_offset;  

        // in bytes, from start of dex
        tb_int_t            class_def_offset; 

    } table[1];

}dx_class_lookup_t, *dx_class_lookup_ref_t;

// the dex type item: type_item
typedef struct __dx_type_item_t
{
    // index into type_ids
    tb_uint16_t             type_idx;

}dx_type_item_t, *dx_type_item_ref_t;

// the dex type list: type_list
typedef struct __dx_type_list_t
{
    // the list size
    tb_uint32_t             size;

    // the list
    dx_type_item_t          list[1];

}dx_type_list_t, *dx_type_list_ref_t;

// the dex file type
typedef struct __dx_file_t
{
    // the base data
    tb_byte_t const*        base;

    // the dex data
    tb_byte_t const*        data;

    // the dex size
    tb_size_t               size;

    // is owner of the file data?
    tb_bool_t               owner;

    // the dex header
    dx_header_ref_t         header;

    // the dex header for the optimized dex file
    dx_header_opt_ref_t     header_opt;

    // the string indexes
    dx_string_id_ref_t      string_ids;

    // the type indexes
    dx_type_id_ref_t        type_ids;

    // the field indexes
    dx_field_id_ref_t       field_ids;

    // the method indexes
    dx_method_id_ref_t      method_ids;

    // the proto indexes
    dx_proto_id_ref_t       proto_ids;

    // the class defines
    dx_class_def_ref_t      class_defs;

    // the link data
    dx_link_ref_t           link_data;

    /* these are mapped out of the "auxillary" section, 
     * and may not be included in the file.
     */
    dx_class_lookup_ref_t   class_lookup;
    tb_cpointer_t           register_map_pool;

    // the classes 
    tb_pointer_t*           classes;

    // the methods 
    tb_pointer_t*           methods;

    // the fields 
    tb_pointer_t*           fields;

}dx_file_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/* return the class_def with the given index 
 *
 * @param dexfile   the dex file
 * @param idx       the class index
 *
 * @return          the class_def
 */
static __tb_inline__ dx_class_def_ref_t dx_file_get_class_def(dx_file_t* dexfile, tb_size_t idx)
{
    // check
    tb_assert(idx < dexfile->header->class_defs_size);

    // get it
    return &dexfile->class_defs[idx];
}

/* get class_data_item pointer
 *
 * @param dexfile       the dex file
 * @param class_def     the class_def
 *
 * @return              the class_data_item
 */
static __tb_inline__ tb_byte_t const* dx_file_get_class_data(dx_file_t* dexfile, dx_class_def_ref_t class_def)
{
    // check
    tb_check_return_val(class_def->class_data_off, tb_null);

    // get it
    return (tb_byte_t const*)(dexfile->data + class_def->class_data_off);
}

/* return the type_id with the given index 
 *
 * @param dexfile   the dex file
 * @param idx       the type index
 *
 * @return          the type id
 */
static __tb_inline__ dx_type_id_ref_t dx_file_get_type_id(dx_file_t* dexfile, tb_size_t idx) 
{
    // check
    tb_assert(idx < dexfile->header->type_ids_size);

    // get it
    return &dexfile->type_ids[idx];
}

/* return the proto_id with the given index 
 *
 * @param dexfile   the dex file
 * @param idx       the proto index
 *
 * @return          the proto id
 */
static __tb_inline__ dx_proto_id_ref_t dx_file_get_proto_id(dx_file_t* dexfile, tb_size_t idx) 
{
    // check
    tb_assert(idx < dexfile->header->proto_ids_size);

    // get it
    return &dexfile->proto_ids[idx];
}

/* return the field_id with the given index 
 *
 * @param dexfile   the dex file
 * @param idx       the field index
 *
 * @return          the field id
 */
static __tb_inline__ dx_field_id_ref_t dx_file_get_field_id(dx_file_t* dexfile, tb_size_t idx) 
{
    // check
    tb_assert(idx < dexfile->header->field_ids_size);

    // get it
    return &dexfile->field_ids[idx];
}

/* return the method_id with the given index 
 *
 * @param dexfile   the dex file
 * @param idx       the method index
 *
 * @return          the method id
 */
static __tb_inline__ dx_method_id_ref_t dx_file_get_method_id(dx_file_t* dexfile, tb_size_t idx) 
{
    // check
    tb_assert(idx < dexfile->header->method_ids_size);

    // get it
    return &dexfile->method_ids[idx];
}

/* return the string_id with the given index 
 *
 * @param dexfile   the dex file
 * @param idx       the string index
 *
 * @return          the string id
 */
static __tb_inline__ dx_string_id_ref_t dx_file_get_string_id(dx_file_t* dexfile, tb_size_t idx) 
{
    // check
    tb_assert(idx < dexfile->header->string_ids_size);

    // get it
    return &dexfile->string_ids[idx];
}

/* return the const char* string data referred to by the given string_id 
 *
 * @param dexfile   the dex file
 * @param string_id the string id
 *
 * @return          the string data
 */
static __tb_inline__ tb_char_t const* dx_file_get_string_data(dx_file_t* dexfile, dx_string_id_ref_t string_id) 
{
    // get data pointer
    tb_byte_t const* p = dexfile->data + string_id->string_data_off;

    // skip the uleb128 length.
    while (*(p++) > 0x7f) ;

    // ok?
    return (tb_char_t const*)p;
}

/* return the string data with the given string_id index 
 *
 * @param dexfile   the dex file
 * @param idx       the string index
 *
 * @return          the string data
 */
static __tb_inline__ tb_char_t const* dx_file_get_string(dx_file_t* dexfile, tb_size_t idx) 
{
    // get string id
    dx_string_id_ref_t string_id = dx_file_get_string_id(dexfile, idx);

    // get string data
    return dx_file_get_string_data(dexfile, string_id);
}

/* return the string data with the given type_id index 
 *
 * @param dexfile   the dex file
 * @param idx       the string index
 *
 * @return          the string data
 */
static __tb_inline__ tb_char_t const* dx_file_get_string_by_type_idx(dx_file_t* dexfile, tb_size_t type_idx) 
{
    // get type_id
    dx_type_id_ref_t type_id = dx_file_get_type_id(dexfile, type_idx);

    // get string for type_id
    return dx_file_get_string(dexfile, type_id->descriptor_idx);
}

/* get the parameter list from a proto_id
 *
 * @param dexfile   the dex file
 * @param proto_id  the proto_id
 *
 * @return          the dex type list
 */
static __tb_inline__ dx_type_list_ref_t dx_file_get_proto_parameters(dx_file_t* dexfile, dx_proto_id_ref_t proto_id) 
{
    // check
    tb_check_return_val(proto_id->parameters_off, tb_null);
    
    // get it
    return (dx_type_list_ref_t)(dexfile->data + proto_id->parameters_off);
}

/* get the static field values from a class
 *
 * @param dexfile   the dex file
 * @param class_def the class_def
 * @param stream    the stream
 *
 * @return          the values 
 */
static __tb_inline__ dx_encoded_array_ref_t dx_file_get_static_field_values(dx_file_t* dexfile, dx_class_def_ref_t class_def, tb_static_stream_ref_t stream)
{
    // check
    tb_check_return_val(class_def->static_values_off, tb_null);
    
    // get it
    dx_encoded_array_ref_t array = (dx_encoded_array_ref_t)(dexfile->data + class_def->static_values_off);
     
    // init stream
    if (stream) tb_static_stream_init(stream, array, dexfile->data + dexfile->size - array);

    // ok?
    return array;
}

/* get a type item from the type list with the given index
 *
 * @param list      the type list
 * @param item_idx  the item index
 *
 * @return          the type item
 */
static __tb_inline__ dx_type_item_ref_t dx_file_get_type_item(dx_type_list_ref_t list, tb_uint32_t item_idx)
{
    // check
    tb_assert(item_idx < list->size);

    // get it
    return &list->list[item_idx];
}

/* get a type_idx from the type list with the given index
 * 
 * @param list      the type list
 * @param item_idx  the item index
 *
 * @return          the type_idx
 */
static __tb_inline__ tb_uint32_t dx_file_get_type_idx(dx_type_list_ref_t list, tb_uint32_t item_idx)
{
    // get item
    dx_type_item_ref_t item = dx_file_get_type_item(list, item_idx);

    // get type idx
    return item->type_idx;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


