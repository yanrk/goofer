#include "test.h"
#include "config/json.h"

USING_NAMESPACE_GOOFER

struct image_t
{
    std::string         url;
    std::string         type;
    std::string         width;
    std::string         height;
};

struct file_t
{
    std::string         code;
    std::string         msg;
    std::string         filename;
    std::string         filesize;
    std::string         width;
    std::string         height;
    std::list<image_t>  images;
};

struct upload_t
{
    std::string         uploadid;
    std::string         code;
    std::string         msg;
    std::list<file_t>   files;
};

static bool test_read()
{
    const char * load_filename = "./x.txt";
    Json json_reader;
    if (!json_reader.load(load_filename))
    {
        return(false);
    }

    upload_t upload;

    if (!json_reader.find_element("code"))
    {
        return(false);
    }
    if (!json_reader.get_element("uploadid", upload.uploadid))
    {
        return(false);
    }
    if (!json_reader.get_element("code", upload.code))
    {
        return(false);
    }
    if (!json_reader.get_element("msg", upload.msg))
    {
        return(false);
    }

    if (!json_reader.into_element("files"))
    {
        return(false);
    }

    for (size_t file_index = 0, file_count = json_reader.get_size(); file_index < file_count; ++file_index)
    {
        file_t file;

        if (!json_reader.into_element(file_index))
        {
            return(false);
        }

        if (!json_reader.get_element("code", file.code))
        {
            return(false);
        }
        if (!json_reader.get_element("msg", file.msg))
        {
            return(false);
        }
        if (!json_reader.get_element("filename", file.filename))
        {
            return(false);
        }
        if (!json_reader.get_element("filesize", file.filesize))
        {
            return(false);
        }
        if (!json_reader.get_element("width", file.width))
        {
            return(false);
        }
        if (!json_reader.get_element("height", file.height))
        {
            return(false);
        }

        if (!json_reader.into_element("images"))
        {
            return(false);
        }

        for (size_t image_index = 0, image_count = json_reader.get_size(); image_index < image_count; ++image_index)
        {
            image_t image;

            if (!json_reader.into_element(image_index))
            {
                return(false);
            }

            if (!json_reader.get_element("url", image.url))
            {
                return(false);
            }
            if (!json_reader.get_element("type", image.type))
            {
                return(false);
            }
            if (!json_reader.get_element("width", image.width))
            {
                return(false);
            }
            if (!json_reader.get_element("height", image.height))
            {
                return(false);
            }

            if (!json_reader.outof_element())
            {
                return(false);
            }

            file.images.push_back(image);
        }

        if (!json_reader.outof_element())
        {
            return(false);
        }

        if (!json_reader.outof_element())
        {
            return(false);
        }

        upload.files.push_back(file);
    }

    if (!json_reader.outof_element())
    {
        return(false);
    }

    return(true);
}

static bool test_write()
{
    Json json_writer;

    if (!json_writer.add_element("uploadid", "UP000000"))
    {
        return(false);
    }
    if (!json_writer.add_element("code", "10"))
    {
        return(false);
    }
    if (!json_writer.add_element("msg", ""))
    {
        return(false);
    }
    if (!json_writer.add_element("files"))
    {
        return(false);
    }

    if (!json_writer.into_element("files"))
    {
        return(false);
    }

    for (size_t file_index = 0, file_count = 1; file_index < file_count; ++file_index)
    {
        if (!json_writer.add_element(file_index))
        {
            return(false);
        }

        if (!json_writer.into_element(file_index))
        {
            return(false);
        }

        if (!json_writer.add_element("code", "0"))
        {
            return(false);
        }
        if (!json_writer.add_element("msg", ""))
        {
            return(false);
        }
        if (!json_writer.add_element("filename", "1D_16-35_1.jpg"))
        {
            return(false);
        }
        if (!json_writer.add_element("filesize", "196690"))
        {
            return(false);
        }
        if (!json_writer.add_element("width", "1024"))
        {
            return(false);
        }
        if (!json_writer.add_element("height", "682"))
        {
            return(false);
        }
        if (!json_writer.add_element("images"))
        {
            return(false);
        }

        if (!json_writer.into_element("images"))
        {
            return(false);
        }

        for (size_t image_index = 0, image_count = 2; image_index < image_count; ++image_index)
        {
            if (!json_writer.add_element(image_index))
            {
                return(false);
            }

            if (!json_writer.into_element(image_index))
            {
                return(false);
            }

            if (!json_writer.set_element("url", 0 == image_index ? "fmn061/20111118" : "fmn061\\20111118"))
            {
                return(false);
            }
            if (!json_writer.set_element("type", 0 == image_index ? "large" : "main"))
            {
                return(false);
            }
            if (!json_writer.set_element("width", 0 == image_index ? "720" : "200"))
            {
                return(false);
            }
            if (!json_writer.set_element("height", 0 == image_index ? "479" : "133"))
            {
                return(false);
            }

            if (!json_writer.outof_element())
            {
                return(false);
            }
        }

        if (!json_writer.outof_element())
        {
            return(false);
        }

        if (!json_writer.outof_element())
        {
            return(false);
        }
    }

    if (!json_writer.outof_element())
    {
        return(false);
    }

    std::string document;
    if (!json_writer.get_document(document, true))
    {
        return(false);
    }

    const char * save_filename = "./y.txt";
    if (!json_writer.save(save_filename))
    {
        return(false);
    }

    return(true);
}

static bool test_read_and_write()
{
    Json json_rw;

    const char * r_filename = "./x.txt";
    if (!json_rw.load(r_filename))
    {
        return(false);
    }

    if (!json_rw.into_element("files"))
    {
        return(false);
    }

    for (size_t file_index = 0, file_count = json_rw.get_size(); file_index < file_count; ++file_index)
    {
        if (!json_rw.into_element(file_index))
        {
            return(false);
        }

        if (!json_rw.set_element("code", "200"))
        {
            return(false);
        }
        if (!json_rw.set_element("msg", "test"))
        {
            return(false);
        }
        if (!json_rw.set_element("filename", "abc\\def"))
        {
            return(false);
        }
        if (!json_rw.set_element("filesize", "xxx/xyz"))
        {
            return(false);
        }
        if (!json_rw.set_element("width", "1024"))
        {
            return(false);
        }
        if (!json_rw.set_element("height", "768"))
        {
            return(false);
        }

        if (!json_rw.outof_element())
        {
            return(false);
        }
    }

    for (size_t index = 0, count = 2; index < count; ++index)
    {
        size_t element_index = json_rw.get_size();

        if (!json_rw.add_element(element_index))
        {
            return(false);
        }

        if (!json_rw.into_element(element_index))
        {
            return(false);
        }

        if (!json_rw.add_element("111", "444"))
        {
            return(false);
        }
        if (!json_rw.add_element("222", "333"))
        {
            return(false);
        }
        if (!json_rw.add_element("333", "222"))
        {
            return(false);
        }
        if (!json_rw.add_element("444", "111"))
        {
            return(false);
        }

        if (!json_rw.outof_element())
        {
            return(false);
        }
    }

    if (!json_rw.outof_element())
    {
        return(false);
    }

    const char * w_filename = "./z.txt";
    if (!json_rw.save(w_filename, false))
    {
        return(false);
    }

    return(true);
}

static bool test_read_all_types()
{
    const char * load_filename = "./111.txt";
    Json json_reader;
    if (!json_reader.load(load_filename))
    {
        return(false);
    }

    bool bool_ele = false;
    int32_t int32_ele = 0;
    uint32_t uint32_ele = 0;
    int64_t int64_ele = 0;
    uint64_t uint64_ele = 0;
    float float_ele = 0.0f;
    double double_ele = 0.0;
    std::string string_ele;
    char char_ele[32] = { 0x00 };
    if (!json_reader.get_element("bool", bool_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("int32", int32_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("uint32", uint32_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("int64", int64_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("uint64", uint64_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("float", float_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("double", double_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("string", string_ele))
    {
        return(false);
    }
    if (!json_reader.get_element("char *", char_ele, sizeof(char_ele)))
    {
        return(false);
    }

    std::list<bool> bool_arr;
    std::list<int32_t> int32_arr;
    std::list<uint32_t> uint32_arr;
    std::list<int64_t> int64_arr;
    std::list<uint64_t> uint64_arr;
    std::list<float> float_arr;
    std::list<double> double_arr;
    std::list<std::string> string_arr;
    if (!json_reader.get_element("bool array", bool_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("int32 array", int32_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("uint32 array", uint32_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("int64 array", int64_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("uint64 array", uint64_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("float array", float_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("double array", double_arr))
    {
        return(false);
    }
    if (!json_reader.get_element("string array", string_arr))
    {
        return(false);
    }

    return(true);
}

static bool test_write_all_types()
{
    Json json_writer;

    bool bool_ele = true;
    int32_t int32_ele = 111;
    uint32_t uint32_ele = 222;
    int64_t int64_ele = 111111111111111111;
    uint64_t uint64_ele = 222222222222222222;
    float float_ele = 111.111f;
    double double_ele = 1111111111.111111111;
    std::string string_ele = "string";
    char char_ele[32] = { "char *" };

    if (!json_writer.add_element("scalar"))
    {
        return(false);
    }
    if (!json_writer.into_element("scalar"))
    {
        return(false);
    }
    if (!json_writer.set_element("bool", bool_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("int32", int32_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("uint32", uint32_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("int64", int64_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("uint64", uint64_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("float", float_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("double", double_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("string", string_ele))
    {
        return(false);
    }
    if (!json_writer.set_element("char *", char_ele))
    {
        return(false);
    }
    if (!json_writer.outof_element())
    {
        return(false);
    }

    if (!json_writer.add_element("string"))
    {
        return(false);
    }
    if (!json_writer.into_element("string"))
    {
        return(false);
    }
    if (!json_writer.set_element("bool", bool_ele, true))
    {
        return(false);
    }
    if (!json_writer.set_element("int32", int32_ele, true))
    {
        return(false);
    }
    if (!json_writer.set_element("uint32", uint32_ele, true))
    {
        return(false);
    }
    if (!json_writer.set_element("int64", int64_ele, true))
    {
        return(false);
    }
    if (!json_writer.set_element("uint64", uint64_ele, true))
    {
        return(false);
    }
    if (!json_writer.set_element("float", float_ele, true))
    {
        return(false);
    }
    if (!json_writer.set_element("double", double_ele, true))
    {
        return(false);
    }
    if (!json_writer.outof_element())
    {
        return(false);
    }

    std::list<bool> bool_arr;
    bool_arr.push_back(true);
    bool_arr.push_back(false);
    std::list<int32_t> int32_arr;
    int32_arr.push_back(111);
    int32_arr.push_back(11111);
    std::list<uint32_t> uint32_arr;
    uint32_arr.push_back(222);
    uint32_arr.push_back(22222);
    std::list<int64_t> int64_arr;
    int64_arr.push_back(111111111111111);
    int64_arr.push_back(1111111111111111111);
    std::list<uint64_t> uint64_arr;
    uint64_arr.push_back(2222222222222222);
    uint64_arr.push_back(2222222222222222222);
    std::list<float> float_arr;
    float_arr.push_back(111.111f);
    float_arr.push_back(222.222f);
    std::list<double> double_arr;
    double_arr.push_back(11111111222.111111111);
    double_arr.push_back(22222222222.222222222);
    std::list<std::string> string_arr;
    string_arr.push_back("string");
    string_arr.push_back("char *");

    if (!json_writer.add_element("scalar array"))
    {
        return(false);
    }
    if (!json_writer.into_element("scalar array"))
    {
        return(false);
    }
    if (!json_writer.set_element("bool array", bool_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("int32 array", int32_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("uint32 array", uint32_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("int64 array", int64_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("uint64 array", uint64_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("float array", float_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("double array", double_arr))
    {
        return(false);
    }
    if (!json_writer.set_element("string array", string_arr))
    {
        return(false);
    }
    if (!json_writer.outof_element())
    {
        return(false);
    }

    if (!json_writer.add_element("string array"))
    {
        return(false);
    }
    if (!json_writer.into_element("string array"))
    {
        return(false);
    }
    if (!json_writer.set_element("bool array", bool_arr, true))
    {
        return(false);
    }
    if (!json_writer.set_element("int32 array", int32_arr, true))
    {
        return(false);
    }
    if (!json_writer.set_element("uint32 array", uint32_arr, true))
    {
        return(false);
    }
    if (!json_writer.set_element("int64 array", int64_arr, true))
    {
        return(false);
    }
    if (!json_writer.set_element("uint64 array", uint64_arr, true))
    {
        return(false);
    }
    if (!json_writer.set_element("float array", float_arr, true))
    {
        return(false);
    }
    if (!json_writer.set_element("double array", double_arr, true))
    {
        return(false);
    }
    if (!json_writer.outof_element())
    {
        return(false);
    }

    const char * save_filename = "./222.txt";
    if (!json_writer.save(save_filename, true))
    {
        return(false);
    }

    return(true);
}

void test_json(void)
{
    test_read();
    test_write();
    test_read_and_write();
    test_read_all_types();
    test_write_all_types();
}
