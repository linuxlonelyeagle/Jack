#include "./include/codewrite.h"
#include "./include/asmdefine.h"

void CodeWrite::set_file(std::string  str)
{
    auto index = str.find_first_of(".vm");
    str = str.substr(0, index);
    str = str + ".asm";
    file_name = str;
    file  = std::ofstream(str, std::ios::out);
}

CodeWrite::CodeWrite(std::string file_name):
index(0)
{
    set_file(file_name);
    map_init();
}

CodeWrite::~CodeWrite() {
    file.close();
}


void CodeWrite::map_init()
{
    segment["argument"] = "ARG";
    segment["local"] = "LCL";
    segment["static"] = "16";
    segment["this"] = "THIS";
    segment["that"] = "THAT";
    segment["pointer"] = "R3";
    segment["temp"] = "R5";

    // binary 二元运算符
    // unary  一元运算符
    // logic 逻辑运算
    op["add"] = std::make_pair("+","binary");
    op["sub"] = std::make_pair("-","binary");
    op["neg"] = std::make_pair("-","unary");
    op["eq"]  = std::make_pair("JEQ","logic");
    op["gt"]  = std::make_pair("JGT","logic");
    op["lt"]  = std::make_pair("JLT","logic");
    op["and"] = std::make_pair("&", "binary");
    op["or"]  = std::make_pair("|", "binary");
    op["not"] = std::make_pair("!", "unary");
}


// 处理算数运算
void CodeWrite::writeArithmetic(std::string oper)
{
    std::string  output = "//" + oper + "\n"; 
    file.write(output.c_str(), output.size());
    if (op[oper].second == "binary") {          // 二元运算
        output = BinaryOperation(op[oper].first);
    } else if (op[oper].second == "unary") {     // 一元运算
        output = UnaryOperation(op[oper].first);
    } else if (op[oper].second == "logic") {       // 逻辑运算
        output = LogicOperation(op[oper].first, this->index++);
    }
    file.write(output.c_str(), output.size());
}


std::string CodeWrite::pushconstantval(std::size_t val)
{
    return PushConstantVal(val);
}

// 处理push还有pop
// push
// constant这个段在内存中实际上是不存在的 ,我们直接吧这个值放入栈中就可以了
// pointer 和 temp 这个段在内存中的值是固定的，pointer起始的位置在r3 temp的起始在r5  index 起始就是基于这个的offset
// 关于 static段，我们只需要让file_name.index作为一个变量就可以了，这个变量是分布在从16开始内存中的
// 关于一般的段：直接把段的基地址加上段的偏移量就可以得到具体的地址

// pop

void CodeWrite::wirtePushPop(Parser::Command& command, std::string seg, int index)
{
    if (command.type != Parser::Command::C_PUSH && command.type != Parser::Command::C_POP)  {
        std::cout << "the command's type is not push or pop" << std::endl;
    }

    std::string output = "//" + command.cmd + " " + command.arg1 + " " +std::to_string(command.arg2) + "\n";  
    file.write(output.c_str(),output.size());    
    if (command.cmd == "push") {
        if (seg == "constant") {
            output = pushconstantval(index);
        } else if (seg == "pointer" || seg == "temp") {
            output = "@" + segment[seg] + "\n" +
                     "D=A\n" +
                     "@" + std::to_string(index) + "\n" + 
                     "A=A+D\n" + 
                     "D=M\n"   +
                     SetEspPosValbyD() + 
                     EspAdd();   
        } else if (seg == "static") {     
            output = "@" + file_name + "." + std::to_string(index) + "\n" +
                     "D=M\n" + // 我们得到了这个变量的值,存储在D中
                     "@SP\n" +
                     "A=M\n" + 
                     "M=D\n" +
                     EspAdd();
        } else {
            output = LoadSegValToD(segment[seg], index) + 
                     SetEspPosValbyD() + 
                     EspAdd();
        } 
    } else if (command.cmd == "pop") {

        if (seg == "static") {
            output = EspSub() + 
                     "A=M\n"  + 
                     "D=M\n"  +
                     "@" + file_name + "." + std::to_string(index) + "\n" +
                     "M=D\n";
        } else if (seg == "temp" || seg == "pointer") {
            output = "@" + segment[seg] + "\n" + 
                     "D=A\n" + 
                     "@" + std::to_string(index) + "\n" + 
                     "D=D+A\n" + 
                     "@R13\n"  + 
                     "M=D\n"   +
                     EspSub()  + 
                     SaveStackValToD() + 
                     "@R13\n"  +
                     "A=M\n"   +
                     "M=D\n";    
        }  else {
            output = "@" + segment[seg] + "\n" + 
                     "D=M\n"   +
                     "@" + std::to_string(index) + "\n" +
                     "D=D+A\n" +
                     "@R13\n"  +
                     "M=D\n"   +
                     EspSub()  +
                     SaveStackValToD() +
                     "@R13\n"   +
                     "A=M\n"    +
                     "M=D\n";
        }
    }
    file.write(output.c_str(), output.size());               
}




