#ifndef __INCLUDE__CODEWRITE__H
#define __INCLUDE__CODEWRITE__H

#include <fstream>
#include <map>
#include "./parser.h"


class CodeWrite{
    private:
    void map_init();
   void set_file(std::string);
   std::string pushconstantval(std::size_t);
    public:
    CodeWrite(std::string);
    ~CodeWrite();
    std::map<std::string, std::string> segment;
    std::map<std::string, std::pair<std::string,std::string>> op;
    std::ofstream file;
    std::string file_name;    
    void writeArithmetic(std::string);
    void wirtePushPop(Parser::Command &, std::string, int);
    std::size_t index;    // 处理逻辑计算的时候要用到
};






#endif