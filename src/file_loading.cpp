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

        void SetLineNr(size_t line_nr) {
            m_line_nr = line_nr;
        }

        size_t GetLineNr() {
            return m_line_nr;
        }

        void SetColumnNr(size_t column_nr) {
            m_column_nr = column_nr;
        }

        size_t GetColumnNr() {
            return m_column_nr;
        }

        void SetValue(std::string value) {
            m_value = value;
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
                (current >= '0' && current <= '9') || current == '_' || current == '-';
        }
    public:
        FileSource(std::string content) {
            m_content = content;
            m_content_index = 0;
            m_line_nr = 1;
            m_column_nr = 1;
        }

        void PrintHighlightError(size_t line, size_t column, size_t length) {
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
        }

        bool HasCurrent() {
            return m_content_index < m_content.size();
        }

        std::string ReadLine() {
            std::string result = "";

            while (HasCurrent()) {
                const char current = GetCurrent();
                const bool break_read = current == '\n';
                // Go to the char after \n
                Forward();
                
                if (break_read)
                    break;
                else
                    result += current;
            }

            return result;
        }

        bool ReadToken(Token *result) {
            SkipWhitespace();
            if (!HasCurrent())
                return false;

            result->SetLineNr(m_line_nr);
            result->SetColumnNr(m_column_nr);

            if (CurrentAlphaNum()) {
                std::string value = "";
                while (HasCurrent() && CurrentAlphaNum()) {
                    value += GetCurrent();
                    Forward();
                }

                result->SetValue(value);
                return true;
            }

            const char first_char = GetCurrent();
            switch (first_char) {
                case '#':
                    SkipLine();
                    return ReadToken(result);
                default:
                    Forward();
                    result->SetValue(std::string(1, first_char));
                    return true;
            }
        }

        bool ReadTokenExpect(std::string value) {
            Token token;
            if (!ReadToken(&token))
                return false;
            
            return token.GetValue() == value;
        }

        void ExceptCurrent(std::string message) {
            std::string position = "[" + std::to_string(m_line_nr) + ":" + std::to_string(m_column_nr) + "] ";
            throw std::runtime_error(position + message);
        }
};

bool smpl::LoadFile(smpl::File *file) {
    if (!util::FileExists(file->GetName()))
        return false;

    std::string file_content = util::LoadTextFile(file->GetName());
    FileSource file_source(file_content);

    while (file_source.HasCurrent()) {
        Token type;
        if (!file_source.ReadToken(&type))
            return true;
        
        Token name;
        if (!file_source.ReadToken(&name))
            file_source.ExceptCurrent("Expected element name");
        
        if (type.GetValue() == "var") {
            // TODO Test case
            if (!file_source.ReadTokenExpect("="))
                file_source.ExceptCurrent("Expected '='");
        
            file_source.ReadLine();
        } else if (type.GetValue() == "goal") {
            // TODO Test case
            if (!file_source.ReadTokenExpect("{"))
                file_source.ExceptCurrent("Expected '{'");
        
            while (file_source.HasCurrent()) {
                Token token;
                if (!file_source.ReadToken(&token))
                    file_source.ExceptCurrent("Expected '}'");
                
                if (token.GetValue() == "}")
                    break;
            }
        } else
            file_source.PrintHighlightError(type.GetLineNr(), type.GetColumnNr(), type.GetValue().length());
    }
 
    return true;
}