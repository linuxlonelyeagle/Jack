#ifndef __INCLUDE_SYMBOLTABLE__H
#define __INCLUDE_SYMBOLTABLE__H

#include <map>
#include <string>
#include <iostream>

class SymbolTable{
    private:
        enum kind{
            STATIC,
            FIELD,
            ARG,
            VAR  
        };
        struct Val {
            std::string type;
            std::string kind;
            std::size_t index;
        };
        std::map<std::string, Val> _symboltable;        //  用来记录这些变量的
        std::map<std::string, std::size_t> _varcount;  // 用来记录表中每一种变量的数量的
        void initVarCount();
    public:
        SymbolTable();
        ~SymbolTable();
        void startSubroutine();
        void define(std::string name, std::string type, std::string kind);
        std::size_t varCount(std::string kind);
        std::string typeOf(std::string name);
        std::size_t indexOf(std::string name);
        std::string kindOf(std::string name);
        void printSymbolTable();
};


#endif