#include "./include/parser.h"

parser::parser(std::string file_name):
file_name(file_name), 
file_in(file_name),
var_addr(16),
current_instruction_idx(0){
    if (!file_in)  {   // 如果打开文件失败
        std::cout << "open file faild!" << std::endl;
        exit(0);
    }
    first_pass();
}


std::string parser::remove_space_commit(std::string str) 
{
    // 先处理注释
    auto index = str.find_first_of("//");
    str = str.substr(0,index);
    // 再处理空白
    auto iter = std::remove_if(str.begin(), str.end(), isspace);
    str.erase(iter, str.end());
    return str;
}

void parser::first_pass() 
{
    std::size_t var_addr = 16;
    std::size_t current_instruction_idx = 0;
    std::string temp;
    while (std::getline(file_in, temp)) {
        auto code = remove_space_commit(temp);  
        if (code == "")            
        continue;
        if (code[0] == '(') {
            symbol_table.add_symbol(code.substr(1, code.size()-2), current_instruction_idx);
            continue;
        }
        file_content.push_back(code);
        current_instruction_idx++;
    }
}

parser::~parser(){    
    file_in.close();
}

bool parser::hasMoreLines() {
    if (current_instruction_idx < file_content.size())
        return true;
    return false;
}

void parser::advence() 
{
    if(!hasMoreLines())   
        return;
    std::string new_line = file_content[current_instruction_idx++];
    bool is_true_syntax = true; 
    switch (new_line[0])
    {
        case '@':
            if (!is_true_a_instruction(new_line))
                break;
           
            a_command_process(new_line);
            break;
        case '(':
            if (!is_true_l_instruction(new_line))
                break;
            l_command_process(new_line);
            break;
    default:
            if (!is_true_c_instruction(new_line))
                break;
            c_command_process(new_line);
            break;
    }    
}


void parser::print() {
    std::cout << "begin to print file_content" << file_content.size() <<  std::endl;
    for (auto i : file_content) {
        std::cout << i << std::endl;
    }
}

bool parser::is_true_a_instruction(std::string)
{
    return true;
}

bool parser::is_true_c_instruction(std::string)
{
    return true;
}

bool parser::is_true_l_instruction(std::string)
{
    return true;
}

void parser::a_command_process(std::string str)
{
    std::string symbol = str.substr(1); 
    if (!symbol_table.is_contains(symbol) && !(symbol[0] >='0' && symbol[0] <='9'))  {
        symbol_table.add_symbol(symbol, var_addr++);
        std::cout << symbol << " " << var_addr-1 << std::endl;
    }
    current_instruction.val = symbol;
    current_instruction.set_type(parser::command::A_COMMAND);    
}

void parser::l_command_process(std::string str)
{
    std::string symbol = str.substr(1, str.size()-2);
    current_instruction.val = symbol;
    current_instruction.set_type(parser::command::L_COMMAND);
}

void parser::c_command_process(std::string str)
{
    std::string dest, comp, jump;
    auto dest_index = str.find_first_of('=');   // 返回了 = 前面的字符数
    auto jump_index = str.find_first_of(';');
    if (dest_index != -1) {
        dest = str.substr(0,dest_index);
    } else {
        dest = "null";
        dest_index = 0;

    }

    if (jump_index != -1) {
        jump = str.substr(jump_index+1);
    } else {
        jump = "null";
        jump_index = str.size();
    }
    std::size_t comp_index;
    if (dest_index == 0) 
        comp_index = 0;
    else 
        comp_index = dest_index + 1;
    
    comp = str.substr(comp_index, jump_index - dest_index);
    current_instruction.val = parser::command::ccommand{dest, comp, jump};
    current_instruction.set_type(command::C_COMMAND);    
}

parser::command::cmd_type parser::instructionType()
{
    return current_instruction.get_type();
}

std::string parser::symbol()
{
    if (instructionType() == command::A_COMMAND)
    return std::get<std::string>(current_instruction.val);
    else if (instructionType() == command::L_COMMAND)
    return std::get<std::string>(current_instruction.val);
    return "";   
}

std::string parser::dest()
{
    if (current_instruction.get_type() != command::C_COMMAND) {
        return "";
    }  
    return std::get<command::ccommand>(current_instruction.val).dest;    
}

std::string parser::comp()
{
    if (current_instruction.get_type() != command::C_COMMAND) {
        return "";
    }    
    return std::get<command::ccommand>(current_instruction.val).comp;
}

std::string parser::jump()
{
    if (current_instruction.get_type() != command::C_COMMAND) {
        return "";
    }
    return std::get<command::ccommand>(current_instruction.val).jump;
}

void parser::look()
{
    std::cout << file_content[current_instruction_idx-1] << std::endl;
}








