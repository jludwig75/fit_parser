#include "fit/parser.h"

#include <string>
#include <vector>

#include <stdio.h>


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You must specify one or more FIT files to parse\n");
        return 1;
    }

    std::vector<std::string> fit_file_names(argv + 1, argv + argc);

    FitParser parser;
    for (const auto& fit_file_name : fit_file_names)
    {
        if (!parser.parse_file(fit_file_name))
        {
            fprintf(stderr, "Failed to parse FIT file %s\n", fit_file_name.c_str());
        }
    }

    return 0;
}