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
 * @file        file.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "file"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dexbox.h"
#include "impl/impl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation 
 */
static tb_bool_t dx_file_check_magic(dx_header_ref_t header)
{
    // check magic
    tb_byte_t* magic = header->magic;
    if (tb_memcmp(magic, DX_MAGIC, 4)) 
    {
        // trace
        tb_trace_e("invalid magic number: %02x %02x %02x %02x", magic[0], magic[1], magic[2], magic[3]);
        return tb_false;
    }

    // check version
    tb_byte_t* version = magic + 4;
    if (tb_memcmp(version, DX_MAGIC_VERSION, 4) && tb_memcmp(version, DX_MAGIC_VERSION_API_13, 4))
    {
        // trace
        tb_trace_e("unsupported dex version: %02x %02x %02x %02x", version[0], version[1], version[2], version[3]);
        return tb_false;
    }

    // ok
    return tb_true;
}
static tb_bool_t dx_file_check_adler(dx_header_ref_t header)
{
    // check
    tb_assert_and_check_return_val(header, tb_false);

    // the skip bytes
    tb_size_t skip = sizeof(header->magic) + sizeof(header->checksum);

    // calculate the checksum
    tb_uint32_t checksum = tb_adler32_make((tb_byte_t const*)header + skip, header->file_size - skip, tb_adler32_make(tb_null, 0, 0));
    if (checksum != header->checksum)
    {
        // trace
        tb_trace_e("bad checksum: %#08x != %#08x", checksum, header->checksum);
        return tb_false;
    }

    // ok?
    return tb_true;
}
static tb_bool_t dx_file_opt_check_ptr(tb_cpointer_t p, tb_cpointer_t b, tb_cpointer_t e)
{
    return (p >= b) && (p < e) && (!(((tb_size_t)p) & 7));
}
static tb_bool_t dx_file_opt_check_adler(dx_header_opt_ref_t header)
{
    // check
    tb_assert_and_check_return_val(header, tb_false);

    // the start data
    tb_byte_t const* start = (tb_byte_t const*)header + header->deps_offset;

    // the end data
    tb_byte_t const* end = (tb_byte_t const*)header + header->opt_offset + header->opt_length;

    // calculate the checksum
    tb_uint32_t checksum = tb_adler32_make(tb_adler32_make(0, tb_null, 0), start, end - start);
    if (checksum != header->checksum)
    {
        // trace
        tb_trace_e("bad checksum: %#08x != %#08x", checksum, header->checksum);
        return tb_false;
    }

    // ok?
    return tb_true;
}
static tb_bool_t dx_file_opt_load(dx_file_t* dexfile, tb_byte_t const* data, tb_size_t size)
{
    // check
    tb_assert_and_check_return_val(dexfile, tb_false);
    tb_assert_and_check_return_val(data && size, tb_false);

    // the optimized header
    dx_header_opt_ref_t header_opt = (dx_header_opt_ref_t)data;
    
    // check the version
    tb_byte_t const* magic = header_opt->magic;
    if (tb_memcmp(magic + 4, DX_OPT_MAGIC_VERSION, 4))
    {
        // trace
        tb_trace_d("unsupported opt version: %02x %02x %02x %02x", magic[4], magic[5], magic[6], magic[7]);
        return tb_false;
    }

    // attach the optimized header
    dexfile->header_opt = header_opt;
    
    // trace
    tb_trace_d("optimized dex offset: %d, flags: 0x%02x", header_opt->dex_offset, header_opt->flags);

    // the optimized data position and size
    tb_byte_t const*    opt_head = data + header_opt->opt_offset;
    tb_byte_t const*    opt_tail = data + size;
    tb_size_t           opt_size = opt_tail - opt_head;
    tb_uint32_t const*  opt = (tb_uint32_t const*)opt_head;

    /* make sure the opt data start is in range and aligned. 
     * this may seem like a superfluous check, but
     *
     * (a) if the file got truncated, it might turn out that opt >= opt_tail
     * (b) if the opt data header got corrupted, opt might not be properly aligned. 
     *     this test will catch both of these cases.
     */
    if (!dx_file_opt_check_ptr(opt, opt_head, opt_tail)) 
    {
        // trace
        tb_trace_e("invalid start pointer %p for the optimized data", opt);
        return tb_false;
    }

    // make sure that the opt data length is a whole number of words.
    if (opt_size & 3) 
    {
        // trace
        tb_trace_e("unaligned length %lu for the optimized data", opt_size);
        return tb_false;
    }

    // make sure that the opt data area is large enough to have at least one chunk header.
    if (opt_size < 8) 
    {
        // trace
        tb_trace_e("the optimized data length %lu is too small", opt_size);
        return tb_false;
    }

    // done
    while (*opt != DX_OPT_CHUNK_END)
    {
        // check pointer
        if (!dx_file_opt_check_ptr(opt + 2, opt_head, opt_tail)) 
        {
            // trace
            tb_trace_e("invalid pointer %p at offset: %lu for the optimized data", opt, (tb_byte_t const*)opt - data);
            return tb_false;
        }

        // the chunk size
        tb_uint32_t chunk_size = *(opt + 1);

        // the chunk data
        tb_byte_t const* chunk_data = (tb_byte_t const*)(opt + 2);

        /* the rounded size is 64-bit aligned and includes + 8 for the
         * type/size header (which was extracted immediately above).
         */
        tb_uint32_t rounded_size = (chunk_size + 8 + 7) & ~7;

        // the next chunk
        tb_uint32_t const* opt_next = opt + (rounded_size / sizeof(tb_uint32_t));

        // check pointer
        if (!dx_file_opt_check_ptr(opt_next, opt_head, opt_tail)) 
        {
            // trace
            tb_trace_e("invalid chunk size %u at offset %lu for the optimized data", chunk_size, (tb_byte_t const*)opt - data);
            return tb_false;
        }

        // done
        switch (*opt) 
        {
        case DX_OPT_CHUNK_CLASS_lOOKUP:
            {
                // trace
                tb_trace_d("class lookup, size: %u", chunk_size);

                // init class lookup
                dexfile->class_lookup = (dx_class_lookup_ref_t)chunk_data;
            }
            break;
        case DX_OPT_CHUNK_REGISTER_MAPS:
            {
                // trace
                tb_trace_d("register map pool, size: %u", chunk_size);

                // init register map pool
                dexfile->register_map_pool = chunk_data;
            }
            break;
        default:
            {
                // trace
                tb_trace_d("unknown chunk %#08x, size: %u for optimized data", *opt, chunk_size);
            }
            break;
        }

        // next
        opt = opt_next;
    }
        
    // ok
    return tb_true;
}
static tb_bool_t dx_file_class_read_header(tb_static_stream_ref_t stream, dx_class_header_ref_t header)
{
    // check
    tb_assert(stream && header);

    // read it
    if (!dx_uleb128_read(stream, &header->static_fields_size)) return tb_false;
    if (!dx_uleb128_read(stream, &header->instance_fields_size)) return tb_false;
    if (!dx_uleb128_read(stream, &header->direct_methods_size)) return tb_false;
    if (!dx_uleb128_read(stream, &header->virtual_methods_size)) return tb_false;

    // ok
    return tb_true;
}
static tb_bool_t dx_file_class_read_field(tb_static_stream_ref_t stream, tb_static_stream_ref_t values_stream, dx_field_t* field, dx_file_t* dexfile, tb_uint32_t* pindex)
{
    // check
    tb_assert(stream && field);

    // read it
    if (!dx_uleb128_read(stream, &field->field_idx)) return tb_false;
    if (!dx_uleb128_read(stream, &field->access_flags)) return tb_false;
    field->field_idx += *pindex;

    // update index
    *pindex = field->field_idx;

    // save dex file
    field->dexfile = dexfile;

    // read value
    if (values_stream) dx_annotation_array_next(values_stream, &field->value);
    else field->value.type = DX_VALUE_NONE;

    // check
    tb_assert(field->field_idx < dexfile->header->field_ids_size);
    tb_assert(!dexfile->fields[field->field_idx]);

    // save this field
    if (field->field_idx < dexfile->header->field_ids_size)
        dexfile->fields[field->field_idx] = (tb_pointer_t)field;

    // ok
    return tb_true;
}
static tb_bool_t dx_file_class_read_method(tb_static_stream_ref_t stream, dx_method_t* method, dx_file_t* dexfile, tb_uint32_t* pindex)
{
    // check
    tb_assert(stream && method);

    // read it
    if (!dx_uleb128_read(stream, &method->method_idx)) return tb_false;
    if (!dx_uleb128_read(stream, &method->access_flags)) return tb_false;
    if (!dx_uleb128_read(stream, &method->code_off)) return tb_false;
    method->method_idx += *pindex;

    // update index
    *pindex = method->method_idx;

    // save dex file
    method->dexfile = dexfile;

    // check
    tb_assert(method->method_idx < dexfile->header->method_ids_size);
    tb_assert(!dexfile->methods[method->method_idx]);

    // save this method
    if (method->method_idx < dexfile->header->method_ids_size)
        dexfile->methods[method->method_idx] = (tb_pointer_t)method;

    // ok
    return tb_true;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation 
 */
dx_file_ref_t dx_file_load_from_url(tb_char_t const* url, tb_bool_t checksum)
{
    // check
    tb_assert_and_check_return_val(url, tb_null);

    // done
    tb_size_t           size = 0;
    tb_byte_t*          data = tb_null;
    tb_stream_ref_t     stream = tb_null;
    dx_file_ref_t   dexfile = tb_null;
    do
    {
        // init stream
        stream = tb_stream_init_from_url(url);
        tb_assert_and_check_break(stream);

        // open stream
        if (!tb_stream_open(stream)) break;

        // the stream size
        size = (tb_size_t)tb_stream_size(stream);
        tb_assert_and_check_break(size);

        // make data
        data = (tb_byte_t*)tb_align_malloc(size, 8);
        tb_assert_and_check_break(data);

        // read data from the stream
        if (!tb_stream_bread(stream, data, size)) break;

        // load dex from data 
        dexfile = dx_file_load_from_data(data, size, checksum);
        tb_check_break(dexfile);

        // mark as owner of the data
        ((dx_file_t*)dexfile)->owner = tb_true;

    } while (0);

    // exit stream
    if (stream) tb_stream_exit(stream);
    stream = tb_null;

    // failed?
    if (!dexfile)
    {
        // exit it
        if (data) tb_free(data);
        data = tb_null;
    }

    // ok?
    return dexfile;
}
dx_file_ref_t dx_file_load_from_data(tb_byte_t const* data, tb_size_t size, tb_bool_t checksum)
{
    // check
    tb_assert_and_check_return_val(data && size, tb_null);

    // done
    tb_bool_t           ok = tb_false;
    dx_file_t*      dexfile = tb_null;
    dx_header_ref_t header = tb_null;
    do
    {
        // check the data size
        tb_assert_and_check_break(size > sizeof(dx_header_t));

        // make dexfile
        dexfile = tb_malloc0_type(dx_file_t);
        tb_assert_and_check_break(dexfile);

        // save base 
        dexfile->base       = data;
        dexfile->owner      = tb_false;

        // is the optimized header?
        if (!tb_memcmp(data, DX_OPT_MAGIC, 4))
        {
            // the optimized header
            dx_header_opt_ref_t header_opt = (dx_header_opt_ref_t)data;

            // load the optimized data
            if (!dx_file_opt_load(dexfile, data, size)) break;

            // check adler for the optimized dex data
            if (checksum) if (!dx_file_opt_check_adler(header_opt)) break;

            // skip the optimized header
            data += header_opt->dex_offset;
            size -= header_opt->dex_offset;
            if (size < header_opt->dex_length)
            {
                // trace
                tb_trace_e("file truncated? filesize: %lu, realsize: %u", size, header_opt->dex_length);
                break;
            }
            size = header_opt->dex_length;
        }

        // save dex data and size
        dexfile->data = data;
        dexfile->size = size;

        // the dex header
        header = (dx_header_ref_t)data;

        // check the dex magic
        if (!dx_file_check_magic(header)) break;

        // check adler for the dex data
        if (checksum) if (!dx_file_check_adler(header)) break;

        // check file size
        if (header->file_size != size) 
        {
            // trace
            tb_trace_e("stored file size: %u != %lu", header->file_size, size);
            break;
        }

        // check classes
        if (!header->class_defs_size) 
        {
            //trace
            tb_trace_e("dex file has no classes!");
            break;
        }

        // attach some dex file structures
        dexfile->header     = header;
        dexfile->string_ids = (dx_string_id_ref_t)  (data + header->string_ids_off);
        dexfile->type_ids   = (dx_type_id_ref_t)    (data + header->type_ids_off);
        dexfile->field_ids  = (dx_field_id_ref_t)   (data + header->field_ids_off);
        dexfile->method_ids = (dx_method_id_ref_t)  (data + header->method_ids_off);
        dexfile->proto_ids  = (dx_proto_id_ref_t)   (data + header->proto_ids_off);
        dexfile->class_defs = (dx_class_def_ref_t)  (data + header->class_defs_off);
        dexfile->link_data  = (dx_link_ref_t)       (data + header->link_off);

        // init classes
        dexfile->classes = (tb_pointer_t*)tb_nalloc0_type(header->class_defs_size, dx_class_t*);
        tb_assert_and_check_break(dexfile->classes);

        // init fields
        dexfile->fields = (tb_pointer_t*)tb_nalloc0_type(header->field_ids_size, dx_field_t*);
        tb_assert_and_check_break(dexfile->fields);

        // init methods
        dexfile->methods = (tb_pointer_t*)tb_nalloc0_type(header->method_ids_size, dx_method_t*);
        tb_assert_and_check_break(dexfile->methods);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (dexfile) dx_file_exit((dx_file_ref_t)dexfile);
        dexfile = tb_null;
    }

    // trace
    tb_trace_d("load dex: %s", ok? "ok" : "no");

    // ok?
    return (dx_file_ref_t)dexfile;
}
tb_void_t dx_file_exit(dx_file_ref_t file)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return(dexfile);

    // exit fields
    if (dexfile->fields) tb_free(dexfile->fields);
    dexfile->fields = tb_null;

    // exit methods
    if (dexfile->methods) tb_free(dexfile->methods);
    dexfile->methods = tb_null;

    // exit classes
    if (dexfile->classes)
    {
        // exit all classes
        tb_size_t i = 0;
        for (i = 0; i < dexfile->header->class_defs_size; i++)
        {
            if (dexfile->classes[i]) tb_free(dexfile->classes[i]);
            dexfile->classes[i] = tb_null;
        }

        // exit class cache
        tb_free(dexfile->classes);
        dexfile->classes = tb_null;
    }

    // exit the file data
    if (dexfile->owner && dexfile->base) tb_align_free(dexfile->base);
    dexfile->base = tb_null;
    dexfile->data = tb_null;
    dexfile->size = 0;

    // exit it
    tb_free(dexfile);
}
tb_byte_t const* dx_file_data(dx_file_ref_t file)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get it
    return dexfile->data;
}
tb_size_t dx_file_size(dx_file_ref_t file)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, 0);

    // get it
    return dexfile->size;
}
tb_char_t const* dx_file_type(dx_file_ref_t file, tb_size_t type_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get it
    return dx_file_get_string_by_type_idx(dexfile, type_idx);
}
tb_char_t const* dx_file_string(dx_file_ref_t file, tb_size_t string_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get it
    return dx_file_get_string(dexfile, string_idx);
}
dx_class_ref_t dx_file_class(dx_file_ref_t file, tb_size_t class_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // done
    tb_bool_t       ok = tb_false;
    dx_class_t* dexclass = tb_null;
    do
    {
        // check range
        tb_check_break(class_idx < dexfile->header->class_defs_size);

        // this class have been cached? return it directly
        if (dexfile->classes[class_idx]) return (dx_class_ref_t)dexfile->classes[class_idx];

        // the class count
        tb_size_t class_size = dx_file_class_size(file);
        tb_assert_and_check_break(class_size && class_idx < class_size);

        // get class_def
        dx_class_def_ref_t class_def = dx_file_get_class_def(dexfile, class_idx);
        tb_assert_and_check_break(class_def);

        // get class data
        tb_byte_t const* class_data = dx_file_get_class_data(dexfile, class_def);
        tb_check_break(class_data);

        // init stream for class data
        tb_static_stream_t stream;
        if (!tb_static_stream_init(&stream, (tb_byte_t*)class_data, dexfile->data + dexfile->size - class_data)) break;

        // read class header
        dx_class_header_t header;
        if (!dx_file_class_read_header(&stream, &header)) break;

        // the dex class data size
        tb_size_t dexclass_size =   sizeof(dx_class_t) 
                                +   header.static_fields_size * sizeof(dx_field_t)
                                +   header.instance_fields_size * sizeof(dx_field_t)
                                +   header.direct_methods_size * sizeof(dx_method_t)
                                +   header.virtual_methods_size * sizeof(dx_method_t);

        // make dex class
        dexclass = (dx_class_t*)tb_malloc0(dexclass_size);
        tb_assert_and_check_break(dexclass);

        // init dex class header
        dexclass->header = header;

        // save file
        dexclass->dexfile = dexfile;

        // save class_def
        dexclass->class_def = dx_file_get_class_def(dexfile, class_idx);
        tb_assert_and_check_break(dexclass->class_def);

        // init dex static fields
        tb_byte_t* p = (tb_byte_t*)&dexclass[1];
        if (header.static_fields_size)
        {
            dexclass->static_fields = (dx_field_t*)p;
            p += header.static_fields_size * sizeof(dx_field_t);
        }

        // init dex instance fields
        if (header.instance_fields_size)
        {
            dexclass->instance_fields = (dx_field_t*)p;
            p += header.instance_fields_size * sizeof(dx_field_t);
        }

        // init dex direct methods
        if (header.direct_methods_size)
        {
            dexclass->direct_methods = (dx_method_t*)p;
            p += header.direct_methods_size * sizeof(dx_method_t);
        }

        // init dex virtual methods
        if (header.virtual_methods_size) dexclass->virtual_methods = (dx_method_t*)p;

        // get the static values
        tb_size_t           values_count = 0;
        tb_static_stream_t  values_stream;
        if (dx_file_get_static_field_values(dexfile, dexclass->class_def, &values_stream))
            values_count = dx_annotation_array_size(&values_stream);

        // read dex static fields
        tb_size_t   i = 0; 
        tb_uint32_t index = 0;
        for (i = 0; i < header.static_fields_size; i++) 
            if (!dx_file_class_read_field(&stream, i < values_count? &values_stream : tb_null, &dexclass->static_fields[i], dexfile, &index)) break;
        tb_assert_and_check_break(i == header.static_fields_size);
 
        // read dex instance fields
        index = 0;
        for (i = 0; i < header.instance_fields_size; i++) 
            if (!dx_file_class_read_field(&stream, tb_null, &dexclass->instance_fields[i], dexfile, &index)) break;
        tb_assert_and_check_break(i == header.instance_fields_size);
 
        // read dex direct methods
        index = 0;
        for (i = 0; i < header.direct_methods_size; i++) 
            if (!dx_file_class_read_method(&stream, &dexclass->direct_methods[i], dexfile, &index)) break;
        tb_assert_and_check_break(i == header.direct_methods_size);
 
        // read dex virtual methods
        index = 0;
        for (i = 0; i < header.virtual_methods_size; i++) 
            if (!dx_file_class_read_method(&stream, &dexclass->virtual_methods[i], dexfile, &index)) break;
        tb_assert_and_check_break(i == header.virtual_methods_size);

        // save this class
        dexfile->classes[class_idx] = (tb_pointer_t)dexclass;

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (dexclass) tb_free(dexclass);
        dexclass = tb_null;
    }

    // ok?
    return (dx_class_ref_t)dexclass;
}
tb_size_t dx_file_class_size(dx_file_ref_t file)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, 0);

    // get it
    return dexfile->header->class_defs_size;
}
dx_method_ref_t dx_file_method(dx_file_ref_t file, tb_size_t method_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // check range
    tb_assert_and_check_return_val(method_idx < dexfile->header->method_ids_size, tb_null);

    // this method have been cached? return it directly
    if (dexfile->methods[method_idx]) return (dx_method_ref_t)dexfile->methods[method_idx];
    
    // get the method id
    dx_method_id_ref_t method_id = dx_file_get_method_id(dexfile, method_idx);
    tb_assert_and_check_return_val(method_id, tb_null);

    // load class of method
    if (!dx_file_class(file, method_id->class_idx)) return tb_null;

    // this method have been cached? return it directly
    return (dx_method_ref_t)dexfile->methods[method_idx];    
}
tb_char_t const* dx_file_method_name(dx_file_ref_t file, tb_size_t method_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get the method id
    dx_method_id_ref_t method_id = dx_file_get_method_id(dexfile, method_idx);
    tb_assert_and_check_return_val(method_id, tb_null);

    // get the method name
    return dx_file_get_string(dexfile, method_id->name_idx);
}
tb_char_t const* dx_file_method_descriptor(dx_file_ref_t file, tb_size_t method_idx, tb_string_ref_t descriptor)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile && descriptor, tb_null);

    // get the method id
    dx_method_id_ref_t method_id = dx_file_get_method_id(dexfile, method_idx);
    tb_assert_and_check_return_val(method_id, tb_null);
   
    // get the proto id
    dx_proto_id_ref_t proto_id = dx_file_get_proto_id(dexfile, method_id->proto_idx);
    tb_assert_and_check_return_val(proto_id, tb_null);

    // init descriptor
    tb_string_cstrcpy(descriptor, "(");

    // get parameters
    dx_type_list_ref_t parameters = dx_file_get_proto_parameters(dexfile, proto_id);
    if (parameters)
    {
        // make the descriptor of parameters
        tb_size_t param_idx = 0;
        tb_size_t param_size = parameters->size;
        for (param_idx = 0; param_idx < param_size; param_idx++)
            tb_string_cstrcat(descriptor, dx_file_get_string_by_type_idx(dexfile, dx_file_get_type_idx(parameters, param_idx)));
    }

    // make the descriptor of the return value
    tb_string_cstrcat(descriptor, ")");
    tb_string_cstrcat(descriptor, dx_file_get_string_by_type_idx(dexfile, proto_id->return_type_idx));

    // ok?
    return tb_string_size(descriptor)? tb_string_cstr(descriptor) : tb_null;
}
tb_char_t const* dx_file_method_class_descriptor(dx_file_ref_t file, tb_size_t method_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get the method id
    dx_method_id_ref_t method_id = dx_file_get_method_id(dexfile, method_idx);
    tb_assert_and_check_return_val(method_id, tb_null);

    // get the class descriptor
    return dx_file_get_string_by_type_idx(dexfile, method_id->class_idx);
}
dx_field_ref_t dx_file_field(dx_file_ref_t file, tb_size_t field_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // check range
    tb_assert_and_check_return_val(field_idx < dexfile->header->field_ids_size, tb_null);

    // this field have been cached? return it directly
    if (dexfile->fields[field_idx]) return (dx_field_ref_t)dexfile->fields[field_idx];
    
    // get the field id
    dx_field_id_ref_t field_id = dx_file_get_field_id(dexfile, field_idx);
    tb_assert_and_check_return_val(field_id, tb_null);

    // load class of field
    if (!dx_file_class(file, field_id->class_idx)) return tb_null;

    // this field have been cached? return it directly
    return (dx_field_ref_t)dexfile->fields[field_idx];  
}
tb_char_t const* dx_file_field_name(dx_file_ref_t file, tb_size_t field_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get the field id
    dx_field_id_ref_t field_id = dx_file_get_field_id(dexfile, field_idx);
    tb_assert_and_check_return_val(field_id, tb_null);

    // get the field name
    return dx_file_get_string(dexfile, field_id->name_idx);
}
tb_char_t const* dx_file_field_descriptor(dx_file_ref_t file, tb_size_t field_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get the field id
    dx_field_id_ref_t field_id = dx_file_get_field_id(dexfile, field_idx);
    tb_assert_and_check_return_val(field_id, tb_null);

    // get the field descriptor
    return dx_file_get_string_by_type_idx(dexfile, field_id->type_idx);
}
tb_char_t const* dx_file_field_class_descriptor(dx_file_ref_t file, tb_size_t field_idx)
{
    // check
    dx_file_t* dexfile = (dx_file_t*)file;
    tb_assert_and_check_return_val(dexfile, tb_null);

    // get the field id
    dx_field_id_ref_t field_id = dx_file_get_field_id(dexfile, field_idx);
    tb_assert_and_check_return_val(field_id, tb_null);

    // get the class descriptor
    return dx_file_get_string_by_type_idx(dexfile, field_id->class_idx);
}
tb_bool_t dx_file_catch(dx_file_ref_t file, dx_code_ref_t code, tb_uint32_t* pc, tb_char_t const* descriptor)
{
    // check
    tb_assert_and_check_return_val(file && code && pc && descriptor, tb_false);

    // find catch
    dx_catch_t trycatch;
    if (!dx_catch_find(&trycatch, file, code, *pc)) return tb_false;

    // find handler
    dx_catch_handler_ref_t handler = tb_null;
    while ((handler = dx_catch_next(&trycatch)))
    {
        // get the exception descriptor
        tb_char_t const* exp_descriptor = handler->type_idx != 0xffffffff? dx_file_type(file, handler->type_idx) : "<any>";
        tb_assert_and_check_break(exp_descriptor);

        // catch it?
        if (!tb_strcmp(exp_descriptor, descriptor))
        {
            // ok
            *pc = handler->address;
            return tb_true;
        }
    }

    // failed
    return tb_false;
}
#ifdef DX_DUMP_ENABLE
tb_void_t dx_file_dump(dx_file_ref_t file)
{
    // dump classes
    tb_size_t class_idx;
    tb_size_t class_size = dx_file_class_size(file);
    for (class_idx = 0; class_idx < class_size; class_idx++)
    {
        // dump class
        dx_class_ref_t clazz = dx_file_class(file, class_idx);
        if (clazz)
        {
            // dump class
            dx_class_dump(clazz);
        }
    }
}
#endif
