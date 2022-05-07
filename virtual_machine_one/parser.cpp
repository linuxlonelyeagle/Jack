#include "./include/parser.h"

Parser::Parser(std::string file_name):
file_name(file_name), 
file_in(file_name, std::ios::in),
addr(0)
{
    if(!file_in.is_open()) {
        std::cout << "open file failed!" << std::endl;
    }  
    std::string temp;
    while (std::getline(file_in, temp)) {
        temp = removecomment(temp);
      /*  std::cout << "temp:" << std::endl;
        for (auto i:temp) {
            int j = i;
            printf("%d ",j);
        }
        std::cout << std::endl;
        */
        if (temp.find('\r') != -1)
            temp = temp.substr(0, temp.size()-1);
        if(temp.size() != 0 && temp.size() != 1)  // 还要处理只有一行的情况   
        file_content.push_back(temp);
    }
}

Parser::~Parser()
{
    file_in.close();
}

bool Parser::hasMoreCommands()
{
    if (addr < file_content.size())
        return true;
    return false;
}

void Parser::print()
{
    for (auto i : file_content) {
        
        std::cout << i  << std::endl;
    }
}

std::string Parser::removecomment(std::string str)
{
    auto temp = str.find_first_of("//");
    str = str.substr(0,temp);
    return str;
}

std::string Parser::removespace(std::string str)
{
    std::string temp = str;
    auto iter = std::remove_if(temp.begin(), temp.end(), ::isspace);
    temp.erase(iter, temp.end());
    return temp;
}


std::string Parser::removecommentandspace(std::string str)
{
    return removespace(removecomment(str));
}

void Parser::advance()
{
    std::string current_cmd = file_content[addr++];
    //std::cout << current_cmd << std::endl;
    std::string token1, token2, token3;
    if (current_cmd.find("push") != -1 || current_cmd.find("pop")!= -1) {
        auto index1 = current_cmd.find_first_of(' ');
        auto index2 = current_cmd.find(' ',index1+1);
        token1 = current_cmd.substr(0,index1);
        token2 = current_cmd.substr(index1+1, index2 - index1 - 1);
        token3 = current_cmd.substr(index2+1);
        //std::cout << "token1:" << token1 << " " << "token2:" << token2  << " " << "token3:" << token3   << std::endl; 
        //std::cout << token1.size() << " " << token2.size() << " " << token3.size() << std::endl;
        if (token1 == "push") {
            command.type = Parser::Command::C_PUSH;
            command.cmd = token1;
            command.arg1  = token2;
            command.arg2  = std::stoi(token3.c_str());
        } else if (token1 == "pop") {
            command.type = Parser::Command::C_POP;
            command.cmd = token1;
            command.arg1 = token2;
            command.arg2 = std::stoi(token3.c_str());
        }           
    } else {
        token1 = current_cmd;
    //    std::cout << "token1:"  << token1 <<  std::endl;
    //    std::cout << token1.size() << std::endl;
        command.type = Parser::Command::C_ARITHMETIC;
        command.cmd = token1;
    }          
}

Parser::Command::CMDtype Parser::commandType()
{
    return command.type;
}

std::string Parser::arg1()
{
    if (command.type == Command::C_ARITHMETIC)
        return command.cmd;
    return command.arg1;
}

int Parser::arg2()
{
    if (command.type == Command::C_PUSH ||
        command.type == Command::C_POP   || 
        command.type == Command::C_FUNCTION || 
        command.type == Command::C_CALL)
        return command.arg2;                                            
}






