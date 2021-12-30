#pragma once

#include <iosfwd>
#include <string>


class FitParser
{
public:
    bool parse_file(const std::string& file_name) const;
private:
    bool read_records(std::istream& fit_file, uint32_t record_bytes) const;
    bool read_record(std::istream& fit_file, uint32_t& bytes_read) const;

};