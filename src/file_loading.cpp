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

        void ExceptCurrent(std::string message) {
            std::string position = "[" + std::to_string(m_line_nr) + ":" + std::to_string(m_column_nr) + "] ";
            throw std::runtime_error(position + message);
        }

        void Forward() {
            if (!HasCurrent())
                return;

            const char current = GetCurrent();
            if (current == '\n') {
                m_line_nr++;
                m_column_nr = 1;
            } else
                m_column_nr++;

            m_content_index++;
        }

        char GetCurrent() {
            return HasCurrent() ? m_content.at(m_content_index) : 0;
        }

        bool CurrentWhiteSpace() {
            const char current = GetCurrent();
            return current == ' ' || current == '\n' || current == '\r' || current == '\t';
        }

        void SkipWhitespace() {
            while (HasCurrent() && CurrentWhiteSpace())
                Forward();
        }

        void SkipLine() {
            while (HasCurrent()) {
                const bool break_skip = GetCurrent() == '\n';
                // Go to the char after \n
                Forward();
                
                if (break_skip)
                    break;
            }
        }

        bool CurrentAlphaNum() {
            const char current = GetCurrent();
            return (current >= 'A' && current <= 'Z') || (current >= 'a' && current <= 'z') ||
                (current >= '0' && current <= '9') || current == '_';
        }
    public:
        FileSource(std::string content) {
            m_content = content;
            m_content_index = 0;
            m_line_nr = 1;
            m_column_nr = 1;
        }

        /*void PrintHighlightError(size_t line, size_t column, size_t length) {
            size_t index = 0;
            size_t index_line = 1;
            
            while (index_line < line) {
                const char current = m_content.at(index);
                if (current == '\n')
                    index_line++;
                
                index++;
                if (index >= m_content.length())
                    return; // TODO Handle this error case?
            }

            index += column - 1;
            size_t count = 0;
            while (index < m_content.length() && count < length) {
                std::cout << m_content.at(index);
                index++;
                count++;
            }
        }*/

        bool HasCurrent() {
            return m_content_index < m_content.size();
        }

        std::string ReadToken() {
            SkipWhitespace();
            if (!HasCurrent())
                return ""; 

            if (CurrentAlphaNum()) {
                std::string token = "";
                while (HasCurrent() && CurrentAlphaNum()) {
                    token += GetCurrent();
                    Forward();
                }

                return token;
            }

            const char first_char = GetCurrent();
            switch (first_char) {
                case '#':
                    SkipLine();
                    return ReadToken();
                default:
                    Forward();
                    return std::string(1, first_char);
            }
        }
};

bool smpl::LoadFile(smpl::File *file) {
    if (!util::FileExists(file->GetName()))
        return false;

    std::string file_content = util::LoadTextFile(file->GetName());
    FileSource file_source(file_content);

    while (file_source.HasCurrent())
        std::cout << "Token: " << file_source.ReadToken() << std::endl;
    return true;
}