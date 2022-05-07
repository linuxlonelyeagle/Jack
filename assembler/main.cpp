#include "./include/parser.h"
#include "./include/code.h"

std::string symbol2number(std::string str, parser& par)
{ 
    auto temp = str;
    if (!(temp[0] >= '0' && temp[0] <= '9')) {     // 如果是符号就需要去符号表里面查找
        if (par.symbol_table.is_contains(temp)) {
            int addr = par.symbol_table.get_addr(temp);
            temp = std::to_string(addr);
        } 
    }
    std::size_t addr = atoi(temp.c_str());
    std::string number;
    while (addr) {
        if (addr % 2 ) 
            number = "1" + number;
        else 
            number = "0" + number;
        addr = addr / 2;
    }
    while (number.size() < 15) 
        number.insert(number.begin(),'0');
    
    return number;
}


int main(int argc, char* argv[]) 
{
    parser p(argv[1]);
    std::string file_name = argv[1];
    auto i = file_name.find(".asm");
    file_name = file_name.substr(0,i);
    file_name += ".hack";
    std::ofstream out_put(file_name);    
    code c; 
    for (auto i = 0; i < p.file_content.size(); i++) {
        std::string temp;
        p.advence();
        switch(p.instructionType()) {
            case parser::command::C_COMMAND:
                temp = "111" + c.comp(p.comp()) + c.dest(p.dest()) + c.jump(p.jump()) + "\n";
                if (temp.size() != 17)
                    p.look();
                break;
            case parser::command::A_COMMAND:
                std::string number = symbol2number(p.symbol(), p);
                temp = "0" + number + "\n";
                break;
        }
        out_put.write(temp.c_str(), temp.size());   
    }
    return 0;
}
