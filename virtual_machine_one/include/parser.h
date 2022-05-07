#ifndef __INCLUDE__PARSER__H
#define __INCLUDE__PARSER__H

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class Parser{
    public:
    class Command {
            public:
            enum CMDtype{
                C_ARITHMETIC,
                C_PUSH,
                C_POP,
                C_LABLE,
                C_GOTO,
                C_IF,
                C_FUNCTION,
                C_RETURN,
                C_CALL  
            };
            CMDtype type;
            std::string arg1;
            std::size_t arg2;
            std::string cmd;
    
    };
    Parser(std::string file_name);
    ~Parser(); 
    bool hasMoreCommands();
    void print();
    void advance();
    Command::CMDtype commandType();
    std::string arg1();
    int arg2();   
    Command command;   
    private:
    std::size_t addr;  // 用来记录指令在vector中的地址
    std::vector<std::string> file_content;
    std::ifstream file_in;
    std::string file_name;
  
    std::string removecomment(std::string);
    std::string removespace(std::string);
    std::string removecommentandspace(std::string);
    
};

#endif 

