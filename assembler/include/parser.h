
#if !defined(__ASSEMBLER__PARSER__H)
#define __ASSEMBLER_PARSER__H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "symboltable.h"
#include <algorithm>
#include <variant>
#include <initializer_list>

class parser
{
private:
    std::string remove_space_commit(std::string);
    bool is_true_a_instruction(std::string);
    bool is_true_c_instruction(std::string);
    bool is_true_l_instruction(std::string);
    void a_command_process(std::string);
    void c_command_process(std::string);
    void l_command_process(std::string);


public:
    class command{
        public:
            enum cmd_type {
                A_COMMAND,
                C_COMMAND,
                L_COMMAND
            };
            struct ccommand{
                std::string dest, comp, jump;
            };
            cmd_type get_type() {
                return _type;
            }
            void set_type(cmd_type type) {
                _type = type;
            }  
            std::variant<ccommand, std::string> val;
        private:
            cmd_type _type;
    };
    parser(std::string file_name);
    ~parser();
    void first_pass();
    bool hasMoreLines();
    void advence();
    parser::command::cmd_type instructionType();
    void print();
    std::string file_name;
    std::fstream file_in;
    std::vector<std::string> file_content;
    command current_instruction;
    std::size_t var_addr;  // 用来定义变量的时候将要赋予的地址
    std::size_t current_instruction_idx;  //也就是在file_content中的下标
    symboltable symbol_table;
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();
    void look();
};



#endif 
