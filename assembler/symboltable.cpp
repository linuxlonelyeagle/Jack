#include "./include/symboltable.h"

symboltable::symboltable() 
{
    symbol_table["SP"] = 0;
    symbol_table["LCL"] = 1;
    symbol_table["ARG"] = 2;
    symbol_table["THIS"] = 3;
    symbol_table["THAT"] = 4;
    for (auto i = 1; i < 16; i++) {
        symbol_table["R" + std::to_string(i)] = i;
    }
    symbol_table["SCREEN"] = 16384;
    symbol_table["KDB"] = 24576;
}

symboltable::~symboltable() 
{

}

bool symboltable::is_contains(std::string &str) 
{
    if (symbol_table.find(str) == symbol_table.end()) 
        return false;
        return true;  
}

void symboltable::add_symbol(std::string str, std::size_t addr) 
{
    symbol_table[str] = addr;
}

int symboltable::get_addr(std::string& str) 
{
    return symbol_table[str];
}


void symboltable::print_symboltable()
{
    for (auto i : symbol_table) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}





