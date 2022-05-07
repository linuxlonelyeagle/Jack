#ifndef __COMPILATIONENGINE__H
#define __COMPILATIONENGINE__H
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"
#include <map>


class CompilationEngine{
    public:
        enum tokenType{
            KEYWORD,
            SYMBOL,
            IDENTIFIER,
            INT_CONST,
            STRING_CONST
        };
    CompilationEngine(const std::string&, const std::string&);    
    ~CompilationEngine();
    void printTokens();
    void CompileClass();     // ok
    void CompileClassVarDec();   // ok 
    void CompileSubroutineDec();   // ok
    void compileParameterList(bool);   // 如果是methed,参数的数量就要在原来的基础上加1，必须要传入this参数  //ok
    void compileSubroutineBody(std::string, bool, bool); //ok
    void compileVarDec(std::size_t&);                  // ok
    void compileStatements();                           // ok
    void compileLet();                                  // ok
    void compileIf();                                   // ok
    void compileWhile();                                // ok
    void compileDo();                                   // ok
    void compileReturn();                               // ok   
    void CompileExpression();                           // ok 
    void CompileTerm();                                 //  ok 
    void CompileExpressionList(const std::string&, bool);  // ok 
    void compileSubrountineCall();                         
    void compileStatement();                             //ok   
    void printsymboltable();
    
    private:
        void initTokens(std::string);
        class Token{
            public:
            std::string val;
            tokenType type;
            Token(std::string v, tokenType t):
            val(v),
            type(t)
            { }
        };
        std::vector<Token> _tokens;
        std::vector<Token>::iterator _tokens_pointer;
        std::ofstream _writer;
        std::map<tokenType, std::string> _tokenTypeMap;
        std::map<std::string, std::string> _opMap;
        std::string _class;   // 这个用来记录当前class 的name  
        void writeXmlLeftTag(const std::string&);
        void writeXmlRightTag(const std::string&);
        void writeXmlTag(const std::string&, const std::string&);
        void eat(std::string);
        void eatIdentifier();
        void eatType();              // int boolean Array char void
        void mapInit();
        void tokenAdvance();
        void pushVar(std::string);
        void popVar(std::string);
        void error(const std::string&);
        bool eatOptional(const std::string&);
        void setPointer(bool, bool);
        std::string getType(const std::string&);
        VMWriter _vmwriter;
        SymbolTable _classSymbolTable, _subruntSymbolTable;
        std::size_t ifIndex;

};


#endif