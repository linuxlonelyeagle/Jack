#ifndef __INCLUDE__CODEWRITE__H
#define __INCLUDE__CODEWRITE__H

#include <fstream>
#include <map>
#include "./parser.h"


class CodeWrite{
    private:
    void map_init();
    void writeInit();
    std::string function_name;
    void setFunctionName(std::string);
    std::string pushconstantval(std::size_t);
    std::map<std::string, std::size_t> returnAddr;   //这个map用来生菜函数的返回的地址的符号
    public:
    CodeWrite(std::string);                     // 一个参数的构造函数用来设置要生产的汇编文件的名字
    CodeWrite(std::string, std::string);     // 这个构造函数第一个参数用来指定生产的汇编代码的文件的名字
    ~CodeWrite();
    std::map<std::string, std::string> segment;
    std::map<std::string, std::pair<std::string,std::string>> op;
    std::ofstream file;   // 这个是输出的文件
    std::string file_name;    //  这个文件是当前正在处理的文件
    void writeArithmetic(std::string);
    void wirtePushPop(Parser::Command &, std::string, int);
    std::size_t index;    // 处理逻辑计算的时候要用到
    void set_File_Name(std::string);   // 这个函数是用来切换要处理的文件的
    void writeLabel(std::string);
    void writeGoto(std::string);
    void writeIf(std::string);
    void writeFunction(std::string, std::size_t);
    void writeCall(std::string, std::size_t);
    void writeReturn();
}; 

#endif