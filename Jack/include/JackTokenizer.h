#ifndef __JACKTOKENIZER__H
#define __JACKTOKENIZER__H
#include <variant>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>


class Tokenizer
{ public: 
        enum tokentype{
            KEYWORD,
            SYMBOL,
            IDENTIFIER,
            INT_CONST,
            STRING_CONST
        };
        Tokenizer(std::string);
        ~Tokenizer();
        bool hasMoreTokens();
        void advance();
        tokentype tokenType();
        std::string keyWord();
        std::string symbol();    
        std::string identifier();
        std::size_t intVal();
        std::string stringVal();
    private:
       
        struct Token {
            std::variant<std::string, int> val;
            tokentype type;
            std::size_t line;           
        };
        std::ifstream _srcFile;   // 用来记录源文件
        std::string _srcCode;     // 用来记录源代码
        Token _token;             // 用来记录当前的token
        std::size_t _index;   // 用来标记当前在_srcCode的位置
        std::string _symbols;     // 用来判断是不是symbol
        std::vector<std::string> _keywords;  // 用来判断是不是keyword
        std::size_t _nowLine;                // 这个变量用来记录当前的在源文件第几行
        void initKeywordsAndSymbols();
        bool isIdentifierChar(char);
        bool isKeyword(const std::string&);
   
};


#endif 