#ifndef __INCLUDE__ASMDEFINE_H
#define __INCLUDE__ASMDEFINE_H

#include <string>

constexpr auto TRUE = 0XFFFF;
constexpr auto FALSE = 0X0000;


// esp++  , A寄存器中的内容还是sp的值
inline std::string EspAdd()     
{
    return std::string("@SP\n") + 
                       "M=M+1\n";
}

// esp--   , A寄存器中的内容还是sp的值
inline std::string EspSub()
{
    return std::string("@SP\n") + 
                       "M=M-1\n";
}


//把内存段seg 中 偏移量为 offset 的值带入D寄存器
inline std::string LoadSegValToD(const std::string& seg, std::size_t offset)
{
    return std::string("@") + seg + "\n" + 
                "A=M\n" + 
                "D=A\n" + 
                "@" + std::to_string(offset) + "\n" + 
                "A=A+D\n" + 
                "D=M\n"; 
}
 

// 把 val直接赋值到D寄存器
//注意:D寄存器不能直接赋值，我们可以利用A寄存器
inline std::string LoadValToD(std::size_t val)
{
    return std::string("@") + std::to_string(val) + "\n" + 
                       "D=A\n";   
}

//通过D寄存器设置esp的位置
inline std::string SetEspPosValbyD()
{
    return std::string("@SP\n")  + 
                       "A=M\n"   +
                       "M=D\n";  
}


//把esp指向的内存中的值存储到D寄存器中
//前提是现在A = @sp   
inline std::string SaveStackValToD()    
{
    return std::string("A=M\n") + 
                       "D=M\n";
}


inline std::string BinaryOperation(const std::string& op)
{
    return EspSub() +  
           SaveStackValToD() +
           EspSub() + 
           "A=M\n"  +
           "M=M" + op + "D\n" +
            EspAdd();
}


inline std::string UnaryOperation(const std::string& op)
{
    return EspSub() + 
           "A=M\n"  +
           "M=" + op + "M\n" +
           EspAdd();
}

inline std::string LogicOperation(const std::string& op, std::size_t i) 
{
    return EspSub() + 
           "A=M\n"  + 
           "D=M\n"  +
           EspSub() + 
           "A=M\n"  +
           "D=M-D\n"+     // D现在的值为栈中两个操作数相减后的值
           "@TRUE"  + std::to_string(i) +"\n"  +
           "D;"     + op +"\n" + 
           "@SP\n"  +
           "A=M\n"  +
           "M=0\n"  +
           "@END" + std::to_string(i) + "\n" + 
           "0;JMP\n"
           "(TRUE"  + std::to_string(i) +")\n" +   // 这个是比较成功了的情况
           "@SP\n"  +
           "A=M\n"  +
           "M=-1\n"   +
           "(END" + std::to_string(i) + ")\n" +
           EspAdd();           
}

inline std::string PushConstantVal(std::size_t i) 
{
    return LoadValToD(i) + 
           SetEspPosValbyD() + 
           EspAdd();
          
}


#endif
