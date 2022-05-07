#include "./include/codewrite.h"
#include "./include/asmdefine.h"

void CodeWrite::set_File_Name(std::string  str)
{
    file_name = str;
   // index = 0;
  //  std::string temp = "set_file:" + str + "\n";
  //  file.write(temp.c_str(), temp.size());    
}



//gen_file_name是要生产的文件的名字
CodeWrite::CodeWrite(std::string gen_file_name):
index(0),   // 这个index用来生成逻辑指令的label
file(gen_file_name, std::ios::out)
{
    writeInit();
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
            std::string var_name = file_name;   
            auto  i = file_name.find_last_of('/');
            if (i != -1)
            var_name = file_name.substr(i+1);
            output = "@" + var_name + "." + std::to_string(index) + "\n" +
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
            std::string var_name = file_name;
            auto i = file_name.find_last_of('/');
            if (i != -1)
            var_name = file_name.substr(i+1);
            output = EspSub() + 
                     "A=M\n"  + 
                     "D=M\n"  +
                     "@" + var_name + "." + std::to_string(index) + "\n" +
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
void CodeWrite::writeInit()
{
    std::string temp("@261\n"
                     "D=A\n"
                     "@SP\n"
                     "M=D\n"
                     "@Sys.init\n"
                     "0;JMP\n");    
    file.write(temp.c_str(), temp.size());                     
}

// function$label
// 在生成label 的时候需要functionanme,我们每次进入函数的时候，就切换函数的名字

void CodeWrite::writeLabel(std::string lable)
{   

    std::string  temp("//lable ");
    temp = temp + lable + "\n";
    file.write(temp.c_str(), temp.size()); 
    temp = function_name + "$" + lable;
    temp = GenerateSymbol(temp);
    file.write(temp.c_str(), temp.size());
}

// function$label
void CodeWrite::writeGoto(std::string lable)
{
    std::string temp("//goto ");
    temp = temp + lable + "\n";
    file.write(temp.c_str(), temp.size());  // 注释
    temp = function_name + "$" + lable;
    temp = GoToFun(temp);
    file.write(temp.c_str(), temp.size());
}

// function$label
void CodeWrite::writeIf(std::string label) 
{
    std::string output("//if-goto ");
    output = output + label + "\n";
    file.write(output.c_str(), output.size());
    label = function_name + "$" + label;
    output = EspSub() + 
             SaveStackValToD() + 
             LoadSymbolToA(label) + 
             "D;JNE\n" ;
    file.write(output.c_str(), output.size());              
}

// symbol : functionName$ret.i   
void CodeWrite::writeCall(std::string functionName, std::size_t numArg)
{
    std::string comment("//call ");
    comment = comment + functionName + " " + std::to_string(numArg) + "\n";
    file.write(comment.c_str(), comment.size());
   if (returnAddr.find(functionName) != returnAddr.end()) {
       returnAddr[functionName] +=1;
   } else {
       returnAddr[functionName] = 0;   
   }
   std::string genReturnSymbol = functionName + "$ret." + std::to_string(returnAddr[functionName]);    // 这个是生成函数的返回的label
   file.write("//push returnAddr\n", std::string("//push returnAddr\n").size());
   std::string outPut = PushSymbolVal(genReturnSymbol) + // 这一步把标签的地址压入栈  
                        "//push LCL\n" +
                        PushSegVal("LCL") + 
                        "//push AGR\n"    +
                        PushSegVal("ARG") +
                        "//push THIS\n"   + 
                        PushSegVal("THIS") +
                        "//push THAT\n"    + 
                        PushSegVal("THAT") +
                        "//ARG = SP - 5 -nArgs\n" +
                        "@SP\n"  + 
                        "D=M\n"  +
                         "@5\n"  +
                         "D=D-A\n" +   //sp = sp -5
                         "@" + std::to_string(numArg) + "\n" +
                         "D=D-A\n"  + 
                         "@ARG\n" +        // ARG = SP - 5 - nArgs
                         "M=D\n"  + 
                         "//LCL = SP\n" + 
                         "@SP\n"  + 
                         "D=M\n"  +
                         "@LCL\n" +
                         "M=D\n"  +    // LCL = SP
                         "//goto functionName\n" + 
                         GoToFun(functionName) + 
                         "//generateSymbol\n" + 
                         GenerateSymbol(genReturnSymbol);
   file.write(outPut.c_str(), outPut.size());
}


// 关于生成label
void CodeWrite::writeFunction(std::string functionName, std::size_t nArg)
{
    
    function_name = functionName;
    std::string output("//");
    output = output + "function " + functionName + " " + std::to_string(nArg) + "\n";
    file.write(output.c_str(), output.size());   // 生成注释 
    output = GenerateSymbol(functionName);
    for (auto i = 0; i < nArg; i++) 
        output += pushconstantval(0);
    file.write(output.c_str(), output.size());
}

void CodeWrite::writeReturn()
{   
    //file_name = "";
    file.write("//return\n", std::string("//return\n").size());
    auto popSeg = [](std::string seg)->std::string  
    {   
        std::string temp;
        temp = temp + "@R13\n" + 
                      "D=M-1\n" + 
                      "AM=D\n"    +
                      "D=M\n"     +
                      "@" + seg + "\n" +
                      "M=D\n";
        return temp;
    };
    std::string output( "//endFrame=LCL\n" 
                        "@LCL\n"  
                        "D=M\n"   
                        "@R13\n"
                        "M=D\n"   // endFrame = LCL
                        "//retAddr=*(endFrame-5)\n"
                        "@5\n" 
                        "A=D-A\n"
                        "D=M\n"   // 得到了retaddr , retaddr = endFrame - 5, endFrame = LCL 
                        "//R14=retAddr\n"
                        "@R14\n"
                        "M=D\n"
                      );  
                output = output + "//*ARG=pop()\n" +
                                   EspSub() + 
                                  "A=M\n"  + 
                                  "D=M\n"  + // get return value
                                  "@ARG\n" + 
                                  "A=M\n"  +
                                  "M=D\n"  +    // *ARG=pop()   
                                  "//SP=ARG+1\n"  + 
                                  "@ARG\n" + 
                                  "D=M+1\n"+
                                  "@SP\n"  +
                                  "M=D\n"  +
                                  "//TAHT=*(endFrame-1)\n" +
                                  popSeg("THAT") + 
                                  "//THIS=*(endFrame-2)\n" +
                                  popSeg("THIS") +
                                  "//ARG=*(endFrame-3)\n" + 
                                  popSeg("ARG") +
                                  "//LCL=*(endFrame-4)\n" +
                                  popSeg("LCL") +
                                  "//goto retAddr\n"      +
                                  "@R14\n" +
                                  "A=M\n"  + 
                                  "0;JMP\n";
        file.write(output.c_str(), output.size());                          
}

