#ifndef MACRO_HPP
#define MACRO_HPP

#include <string>

namespace smpl {
    
    class Macro {
        public:
            virtual ~Macro() = default;
            virtual std::string Run() = 0;
    };

    class AllRecursiveMacro : public Macro {
        private:
            std::string m_regex_string;
        public:
            AllRecursiveMacro(std::string regex_string = "");
            std::string Run() override;
    };
}

#endif