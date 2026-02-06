#include <stdexcept>
#include <iostream>
#include <file_loading.hpp>
#include <core_classes.hpp>
#include <util/file.hpp>

class Token {
    private:
        size_t m_line_nr;
        size_t m_column_nr;
        std::string m_value;
    public:
        Token(size_t line_nr = 0, size_t column_nr = 0, std::string value = "") {
            m_line_nr = line_nr;
            m_column_nr = column_nr;
            m_value = value;
        }

        size_t GetLineNr() {
            return m_line_nr;
        }

        size_t GetColumnNr() {
            return m_column_nr;
        }

        std::string GetValue() {
            return m_value;
        }
};

class FileSource {
    private:
        std::string m_content;
        size_t m_content_index;
        size_t m_line_nr;
        size_t m_column_nr;
    public:
        FileSource(std::string content) {
            m_content = content;
            m_content_index = 0;
            m_line_nr = 0;
            m_column_nr = 0;
        }

        bool HasMore() {
            return m_content_index < m_content.size();
        }

        char GetCurrent() {
            return m_content.at(m_content_index);
        }
};

bool LoadFile(smpl::File *file) {
    if (!util::FileExists(file->GetName()))
        return false;

    std::string file_content = util::LoadTextFile(file->GetName());
    
    // TODO Parsing

    return true;
}