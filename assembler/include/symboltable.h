#ifndef __ASSEMBLER__SYMBOLTABLE__H
#define __ASSEMBLER__SYMBOLTABLE__H
#include <map>
#include <string>
#include <iostream>
class symboltable{
    private:
    std::map<std::string, int> symbol_table;
    public:
    symboltable();
    ~symboltable();
    bool is_contains(std::string&);
    void add_symbol(std::string, std::size_t);
    int get_addr(std::string&);
    void print_symboltable();

};

#endif