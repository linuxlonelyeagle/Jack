#include "./include/parser.h"
#include "./include/codewrite.h"
#include "./include/asmdefine.h"


int main(int argc, char* argv[])
{
    Parser p(argv[1]);
    CodeWrite c(argv[1]);
    while (p.hasMoreCommands()) {
        p.advance();
        switch(p.commandType()) {
            case Parser::Command::C_ARITHMETIC:
                c.writeArithmetic(p.arg1());
                break;
            case Parser::Command::C_PUSH:
                c.wirtePushPop(p.command, p.arg1(), p.arg2());
                break;
            case Parser::Command::C_POP:
                c.wirtePushPop(p.command, p.arg1(), p.arg2());
                break;
        }
    }
    return 0;
}
