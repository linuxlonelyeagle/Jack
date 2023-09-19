#include "./include/SymbolTable.h"

void SymbolTable::initVarCount() {
    _varcount["argument"] = 0;
    _varcount["local"] = 0;
    _varcount["static"] = 0;
    _varcount["field"] = 0;
}

SymbolTable::SymbolTable() {
    initVarCount();
}

SymbolTable::~SymbolTable() {

}


void SymbolTable::startSubroutine() {
    _symboltable.clear();
    initVarCount();
}

void SymbolTable::define(std::string name, std::string type, std::string kind) {  
    struct Val tmp = {type, kind, _varcount[kind]++};
    _symboltable[name] = tmp;    
}

std::size_t SymbolTable::varCount(std::string kind) {
    return _varcount[kind];
}

std::string SymbolTable::typeOf(std::string name) {
    if (_symboltable.find(name) == _symboltable.end())
    return "null";
    return _symboltable[name].type;
}

std::size_t SymbolTable::indexOf(std::string name) {
    if (_symboltable.find(name) ==  _symboltable.end())
           return -1;
    return _symboltable[name].index;
}

void SymbolTable::printSymbolTable() {
    std::cout << "print var count" << std::endl;
    for (auto i : _varcount) {
        std::cout <<  i.first <<  " " << i.second << std::endl;   
    }
    
    std::cout << "print symboltable" << std::endl;
    for (auto i : _symboltable) {
        std::cout << "name " << i.first << std::endl;
        std::cout << "kind " << i.second.kind << std::endl;
        std::cout << "type " << i.second.type << std::endl;
        std::cout << "index "      << i.second.index << std::endl;
    }   
}

std::string SymbolTable::kindOf(std::string name) {
    if (_symboltable.find(name) == _symboltable.end()) {
        return "null";
    }
    return _symboltable[name].kind;
}
