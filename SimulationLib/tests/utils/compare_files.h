#pragma once

#include <fstream>
#include <algorithm>

// This function was sourced from the following StackOverflow answer:
// https://stackoverflow.com/questions/15118661/in-c-whats-the-fastest-way-to-tell-whether-two-string-or-binary-files-are-di

bool compare_files(const std::string& filename1, const std::string& filename2)
{
    std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary); //open file at the end
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary); //open file at the end
    const std::ifstream::pos_type fileSize = file1.tellg();

    if (fileSize != file2.tellg()) {
        return false; //different file size
    }

    file1.seekg(0); //rewind
    file2.seekg(0); //rewind

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    return std::equal(begin1,std::istreambuf_iterator<char>(),begin2); //Second argument is end-of-range iterator
}
