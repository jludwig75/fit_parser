#include "parser.h"

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "private/fit_structs.h"


bool FitParser::parse_file(const std::string& file_name) const
{
    std::ifstream fit_file(file_name, std::ios::binary);
    if (!fit_file)
    {
        fprintf(stderr, "Failed ton ope file \"%s\"\n", file_name.c_str());
        return false;
    }

    FitFileHeader header{};

    // Only read the legacy header
    fit_file.read(reinterpret_cast<char*>(&header), sizeof(FitFileHeaderLegacy));
    if (!fit_file)
    {
        fprintf(stderr, "Failed to read from file \"%s\"\n", file_name.c_str());
        return false;
    }

    if (std::string(header.data_type, header.data_type + sizeof(header.data_type)) != ".FIT")
    {
        fprintf(stderr, "Invalid FIT file header\n");
        return false;
    }

    if (header.header_size == 14)   // non-legacyt header
    {
        // Get the CRC too
        fit_file.read(reinterpret_cast<char*>(&header.crc), sizeof(header.crc));
        if (!fit_file)
        {
            fprintf(stderr, "Failed to read from file \"%s\"\n", file_name.c_str());
            return false;
        }
    }

    return read_records(fit_file, header.data_size);
}


bool FitParser::read_records(std::istream& fit_file, uint32_t record_bytes) const
{
    while (record_bytes > 0)
    {
        uint32_t bytes_read;
        if (!read_record(fit_file, bytes_read))
        {
            return false;
        }

        assert(bytes_read <= record_bytes);
        if (bytes_read > record_bytes)
        {
            return false;
        }

        record_bytes -= bytes_read;
    }

    return true;
}


bool FitParser::read_record(std::istream& fit_file, uint32_t& bytes_read) const
{
    bytes_read = 0;

    FitRecordHeader header;
    fit_file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!fit_file)
    {
        fprintf(stderr, "Failed to read from file \n");
        return false;
    }
    bytes_read += sizeof(header);

    if (header.message_type == MESSAGE_TYPE_DEFINITION)
    {
        FitRecordDefinitionHeader definiton_header;
        fit_file.read(reinterpret_cast<char*>(&definiton_header), sizeof(definiton_header));
        if (!fit_file)
        {
            fprintf(stderr, "Failed to read from file \n");
            return false;
        }
        bytes_read += sizeof(definiton_header);

        std::vector<FitFieldDefinition> fields(definiton_header.fields);
        uint32_t bytes_to_read = fields.size() * sizeof(fields[0]);
        fit_file.read(reinterpret_cast<char*>(fields.data()), bytes_to_read);
        if (!fit_file)
        {
            fprintf(stderr, "Failed to read from file \n");
            return false;
        }
        bytes_read += bytes_to_read;

        // TODO: interpret base types

        if (header.message_type_specific == 1)
        {
            fprintf(stderr, "Field contains developer specific definitions which are not yet supported\n");
            return false;
            // uint8_t developer_fields;
            // fit_file.read(reinterpret_cast<char*>(&developer_fields), sizeof(developer_fields));
            // if (!fit_file)
            // {
            //     fprintf(stderr, "Failed to read from file \n");
            //     return false;
            // }
            // bytes_read += sizeof(developer_fields);
        }

        FitRecordHeader data_record;
        fit_file.read(reinterpret_cast<char*>(&data_record), sizeof(data_record));
        if (!fit_file)
        {
            fprintf(stderr, "Failed to read from file \n");
            return false;
        }
        bytes_read += sizeof(data_record);
        if (data_record.message_type != MESSAGE_TYPE_DATA)
        {
            fprintf(stderr, "Unexpected file format. I may have misunderstood something\n");
            return false;
        }

        unsigned data_record_bytes = 0;
        for (const auto& field : fields)
        {
            data_record_bytes += field.size;
        }

        std::vector<uint8_t> record_data(data_record_bytes);
        fit_file.read(reinterpret_cast<char*>(record_data.data()), data_record_bytes);
        if (!fit_file)
        {
            fprintf(stderr, "Failed to read from file \n");
            return false;
        }
        data_record_bytes += data_record_bytes;
    }

    return true;
}
