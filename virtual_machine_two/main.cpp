#include "./include/parser.h"
#include "./include/codewrite.h"
#include "./include/asmdefine.h"
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

//如果path是.vm文件直接加入vector
//如果path是一个目录，遍历目录，然后把所有.vm文件加入vector

template<typename T>
std::vector<std::string> Get_file(std::string path, T&& f)
{
    struct stat st;
    if (stat(path.c_str(), &st)) {
        std::cout << "get file stat faild!" << std::endl;
        return {};       
    } 
    std::vector<std::string> files;
    if (S_ISDIR(st.st_mode)) {    // 如果是目录
        DIR* dir = opendir(path.c_str());
        while (dirent* info = readdir(dir)) {
            if (strlen(info->d_name) >3 &&  f(info->d_name)) {
                files.push_back(path + "/"+ info->d_name);
            }
        }
    }  
    // 如果是普通文件
    if (strlen(path.c_str()) > 3 && f(path)) 
    files.push_back(path);
    return files;
}

int main(int argc, char* argv[])
{
    //获取需要处理的vm代码文件
    std::vector<std::string> files = Get_file(argv[1],
    [](std::string name)-> bool{
        if (name.substr(name.size()-3) == ".vm") {
            return true;
        }
        return false;
    });


    std::string gen_file_name("my_Prog.asm");  // 默认生成的汇编文件的名字
    if (argc == 3) 
        gen_file_name = argv[2];
    CodeWrite c(gen_file_name);

    for (auto file : files)  {
        c.set_File_Name(file);
        Parser p(file);
        while (p.hasMoreCommands()) {
            p.advance();
            switch(p.commandType()) {
                case Parser::Command::C_PUSH:
                    c.wirtePushPop(p.command, p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_POP:
                    c.wirtePushPop(p.command, p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_ARITHMETIC:
                    c.writeArithmetic(p.arg1());
                    break;
                case Parser::Command::C_CALL:
                    c.writeCall(p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_FUNCTION:
                    c.writeFunction(p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_RETURN:
                    c.writeReturn();
                    break;
                case Parser::Command::C_GOTO:
                    c.writeGoto(p.arg1());
                    break;
                case Parser::Command::C_IF:
                    c.writeIf(p.arg1());
                    break;
                case Parser::Command::C_LABLE:
                    c.writeLabel(p.arg1());
                    break;
            }
        }           
    }
    return 0;
}
