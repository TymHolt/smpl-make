#include <stdexcept>
#include <iostream>
#include <file_loading.hpp>
#include <core_classes.hpp>
#include <util/file.hpp>
#include <util/string_parsing.hpp>

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

        void SkipWhitespace() {
            while (HasCurrent() && util::IsWhiteSpace(GetCurrent()))
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
    public:
        FileSource(std::string content) {
            m_content = content;
            m_content_index = 0;
            m_line_nr = 1;
            m_column_nr = 1;
        }

        void PrintErrorCurrent(std::string message) {
            PrintError(m_line_nr, m_column_nr, 1, message);
        }

        void PrintErrorToken(Token *token, std::string message) {
            PrintError(token->GetLineNr(), token->GetColumnNr(), token->GetValue().length(), message);
        }

        void PrintError(size_t line, size_t column, size_t length, std::string message) {
            std::cerr << '[' << std::to_string(line) << ':' << std::to_string(column) << "] " << message << std::endl;

            size_t index = 0;
            size_t index_line = 1;
            
            while (index_line < line) {
                const char current = m_content.at(index);
                if (current == '\n')
                    index_line++;
                
                index++;
            }

            if (index >= m_content.length())
                return;

            size_t count = index;
            while (true) {
                const char current = m_content.at(count);
                std::cerr << current;
                if (current == '\n')
                    break;
                
                count++;
            }

            count = index + 1;
            while (true) {
                std::cerr << (count - index >= column && count - index < column + length ? '^' : ' ');
                if (m_content.at(count) == '\n')
                    break;
                
                count++;
            }

            std::cerr << std::endl;
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

            if (util::IsAlpha(GetCurrent()) || GetCurrent() == '_') {
                std::string value = "";
                while (HasCurrent() && util::IsAlphaNumExtended(GetCurrent())) {
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
};

bool ExpectToken(FileSource *file_source, std::string expected_value) {
    Token token;
    if (!file_source->ReadToken(&token)) {
        file_source->PrintErrorCurrent("End of file reached, expected '" + expected_value + "'");
        return false;
    }

    if (token.GetValue() != expected_value) {
        file_source->PrintErrorToken(&token,
            "Unexpected '" + token.GetValue() + "', expected '" + expected_value + "'");
        return false;
    }

    return true;
}

bool IsValidIdentifier(std::string identifier) {
    if (identifier.length() == 0)
        return false;

    if (!util::IsAlpha(identifier.at(0)))
        return false;

    for (size_t index = 0; index < identifier.length(); index++)
        if (!util::IsAlphaNumExtended(identifier.at(index)))
            return false;

    return true;
}

bool smpl::LoadFile(smpl::File *file) {
    if (!util::FileExists(file->GetName()))
        return false;

    std::string file_content = util::LoadTextFile(file->GetName());
    FileSource file_source(file_content);

    while (file_source.HasCurrent()) {
        Token type;
        if (!file_source.ReadToken(&type))
            return true;
        
        Token identifier;
        if (!file_source.ReadToken(&identifier)) {
            file_source.PrintErrorCurrent("End of file reached, expected element identifier");
            return false;
        }

        if (!IsValidIdentifier(identifier.GetValue())) {
            file_source.PrintErrorToken(&identifier, "Invalid identifier '" + identifier.GetValue() + "'");
            return false;
        }

        if (type.GetValue() == "var") {
            if (!ExpectToken(&file_source, "="))
                return false;
        
            // TODO Continue
            file_source.ReadLine();
        } else if (type.GetValue() == "goal") {
            if (!ExpectToken(&file_source, "{"))
                return false;

            while (file_source.HasCurrent()) {
                Token token;
                if (!file_source.ReadToken(&token)) {
                    file_source.PrintErrorCurrent("End of file reached, expected '}'");
                    return false;
                }

                // TODO Continue

                if (token.GetValue() == "}")
                    break;
            }
        } else {
            file_source.PrintErrorToken(&type, "Unknown type '" + type.GetValue() + "'");
            return false;
        }
    }
 
    return true;
}