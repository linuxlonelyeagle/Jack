#include "./include/CompilationEngine.h"

// 处理if
const std::string ifTureLable = "IF_TRUE";
const std::string ifFalseLable = "IF_FALSE";
const std::string ifEndLable = "IF_END";

//处理while
const std::string whileStart = "WHILE_START";
const std::string whileEnd = "WHILE_END";


CompilationEngine::CompilationEngine(const std::string& src_file, const std::string& vm_file):
_vmwriter(vm_file),
ifIndex(0)
{
    std::cout << src_file << std::endl;

    auto index1 = src_file.find_last_of('/');
    auto index2 = src_file.find(".jack");
    auto temp = src_file.substr(0, index2);
    temp += ".xml";
    _writer = std::ofstream(temp);    
    initTokens(src_file);
    _tokens_pointer = _tokens.begin(); 
    mapInit();
   _classSymbolTable.startSubroutine();
}

CompilationEngine::~CompilationEngine()
{
    _writer.close();
}


void CompilationEngine::initTokens(std::string filename)
{
    Tokenizer t(filename);
    while (t.hasMoreTokens()) {
        t.advance();
        switch(t.tokenType()) {
            case Tokenizer::KEYWORD:
                _tokens.push_back(Token(t.keyWord(), KEYWORD));
                break;
            case Tokenizer::IDENTIFIER:
                _tokens.push_back(Token(t.identifier(),IDENTIFIER));
                break;
            case Tokenizer::INT_CONST:{
                std::string tmp = std::to_string(t.intVal());
                _tokens.push_back(Token(tmp, INT_CONST));
                break;
            }
            case Tokenizer::STRING_CONST:
                _tokens.push_back(Token(t.stringVal(), STRING_CONST));
                break;
            case Tokenizer::SYMBOL:
                _tokens.push_back(Token(t.symbol(), SYMBOL));
                break;
        }
    }
}

void CompilationEngine::mapInit()
{
    _tokenTypeMap[KEYWORD] = "keyword";
    _tokenTypeMap[INT_CONST] = "integerConstant";
    _tokenTypeMap[SYMBOL] = "symbol";
    _tokenTypeMap[STRING_CONST] = "stringConstant";
    _tokenTypeMap[IDENTIFIER] = "identifier";
    _opMap["+"] = "add";
    _opMap["-"] = "sub";
    _opMap["&"] = "and";
    _opMap["|"] = "or";
    _opMap[">"] = "gt";
    _opMap["<"] = "lt";
    _opMap["="] = "eq";
    _opMap["/"] = "call Math.divide 2";
    _opMap["*"] = "call Math.multiply 2";

}


void CompilationEngine::tokenAdvance()
{
    _tokens_pointer++;
}

void CompilationEngine::printTokens()
{
    for (auto i = _tokens_pointer; i != _tokens.end(); i++) {
        std::cout << i->val << " ";
    }
    std::cout << std::endl;
}


void CompilationEngine::writeXmlLeftTag(const std::string& tag)
{
    std::string temp = "<" + tag + ">\n";
    _writer.write(temp.c_str(), temp.size());  
}

void CompilationEngine::writeXmlRightTag(const std::string& tag)
{
    std::string temp = "</" + tag + ">\n";
    _writer.write(temp.c_str(), temp.size());
}

void CompilationEngine::writeXmlTag(const std::string& val, const std::string& type)
{
    std::string temp = "<" + type + ">" + val + "</" + type + ">\n";
    _writer.write(temp.c_str(), temp.size());
}

void CompilationEngine::error(const std::string& info)
{
    std::cout << info << std::endl;
    _writer.close();
    exit(0);
}

void CompilationEngine::eat(std::string symbol)
{
    if (symbol == _tokens_pointer->val) {
        if (symbol == "<")
        writeXmlTag("&lt;", _tokenTypeMap[_tokens_pointer->type]);
        else if (symbol == ">")
        writeXmlTag("&gt;", _tokenTypeMap[_tokens_pointer->type]);
        else if (symbol == "\"") 
        writeXmlTag("&quot;", _tokenTypeMap[_tokens_pointer->type]) ;
        else if (symbol == "&")
        writeXmlTag("&amp;", _tokenTypeMap[_tokens_pointer->type]);
        else 
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
        tokenAdvance();
    } else {
        std::string temp = "expect " + symbol + ", but the current symbol is " + _tokens_pointer->val;
        error(temp);
    }
}

void CompilationEngine::eatIdentifier()
{
    if (_tokens_pointer->type == IDENTIFIER) {
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);       
        tokenAdvance(); 
    } else {
        std::string temp = "expect indentifier but the current token's  type is " + _tokenTypeMap[_tokens_pointer->type];  
        error(temp);
    }     
}

void CompilationEngine::CompileClass()
{
    writeXmlLeftTag("class");
    eat("class");
    eatIdentifier();
    _class = (_tokens_pointer - 1 ) ->val;
    eat("{");
    CompileClassVarDec();
    CompileSubroutineDec();
    eat("}");
    writeXmlRightTag("class");
}

void CompilationEngine::eatType()
{
    if (_tokens_pointer->val == "int" ||
        _tokens_pointer->val == "void" ||
        _tokens_pointer->val == "char" ||
        _tokens_pointer->val == "Array" ||
        _tokens_pointer->val == "boolean" || _tokens_pointer->type == IDENTIFIER) {
            writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
            tokenAdvance();
        } else {
            std::string temp = "expect int or void or char or Array or boolean the current token is " + _tokens_pointer->val;
            error(temp);
        }
}


bool CompilationEngine::eatOptional(const std::string& symbol)
{
    if (symbol == _tokens_pointer->val) {
        if (symbol == "<")
        writeXmlTag("&lt;", _tokenTypeMap[_tokens_pointer->type]);
        else if (symbol == ">")
        writeXmlTag("&gt;", _tokenTypeMap[_tokens_pointer->type]);
        else if (symbol == "\"") 
        writeXmlTag("&quot;", _tokenTypeMap[_tokens_pointer->type]) ;
        else if (symbol == "&")
        writeXmlTag("&amp;", _tokenTypeMap[_tokens_pointer->type]);
        else 
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
        tokenAdvance();
        return true;
    }
    return false;
}

void CompilationEngine::CompileClassVarDec()
{
    auto kind = _tokens_pointer->val;
    if (_tokens_pointer->val == "static" || _tokens_pointer->val == "field") {
        writeXmlLeftTag("classVarDec");
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
        tokenAdvance();           
    } else   // 不是classvardec,直接返回  
        return;
    auto type = _tokens_pointer->val;
    eatType();
    auto name = _tokens_pointer->val;
    eatIdentifier();
    _classSymbolTable.define(name, type, kind);
    while (eatOptional(",")) {
        name = _tokens_pointer->val;
        eatIdentifier();  
        _classSymbolTable.define(name, type, kind);
    }
    eat(";");
    writeXmlRightTag("classVarDec");
    CompileClassVarDec();   
}


void CompilationEngine::CompileSubroutineDec()
{
    _subruntSymbolTable.startSubroutine();
    bool isMethod = false;
    bool isConstructor = false;
    if (_tokens_pointer->val == "constructor" || 
    _tokens_pointer->val == "function" ||
    _tokens_pointer->val == "method") {
        if (_tokens_pointer->val == "method")
            isMethod = true;
        if (_tokens_pointer->val == "constructor")
            isConstructor = true;
        writeXmlLeftTag("subroutineDec");
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
        tokenAdvance();
    } else    // 如果条件不成立的化，就说明不是subrountinedec
        return;
    
    eatType();

    auto functionname =_class + "." + _tokens_pointer->val;

    eatIdentifier();
    eat("(");
    compileParameterList(isMethod);
    eat(")");
    compileSubroutineBody(functionname, isConstructor, isMethod);
    writeXmlRightTag("subroutineDec");
    //std::cout << "in " << functionname << std::endl;
   // _subruntSymbolTable.printSymbolTable();
   // _subruntSymbolTable.startSubroutine();
    CompileSubroutineDec();
}


void CompilationEngine::compileParameterList(bool isMethod)
{   // 必须先写lefttag
    if (isMethod) 
        _subruntSymbolTable.define("this", _class, "argument");
    writeXmlLeftTag("parameterList");  
    if (_tokens_pointer->val == ")") {   //如果参数列表为空的话
        writeXmlRightTag("parameterList");
        return;
    }
    // 下面的情况就不是参数列表为空的情况了
    do
    {
        auto type = _tokens_pointer->val;
        eatType();
        auto name = _tokens_pointer->val;
        eatIdentifier();
        _subruntSymbolTable.define(name, type, "argument");
    }while(eatOptional(","));
    writeXmlRightTag("parameterList");   
}

void CompilationEngine::CompileExpressionList(const std::string& objname, bool ismethod)
{
    writeXmlLeftTag("expressionList");

    auto argcount = 0;
    if (ismethod)
        argcount++;


    if (_tokens_pointer->val == ")") {
        writeXmlRightTag("expressionList");
        _vmwriter.writeCall(objname, argcount);
        return;
    }
    do{
        CompileExpression();
        argcount++;
    }while(eatOptional(","));
        _vmwriter.writeCall(objname, argcount);
    writeXmlRightTag("expressionList");
}



void CompilationEngine::compileSubroutineBody(std::string functionname, bool isconstructor, bool ismethod)
{
    std::size_t varcount = 0;
    writeXmlLeftTag("subroutineBody");
    eat("{");
    while (_tokens_pointer->val == "var") 
    compileVarDec(varcount);

    _vmwriter.writeFunction(functionname, varcount);
    setPointer(isconstructor, ismethod);

    compileStatements();
    eat("}");
    writeXmlRightTag("subroutineBody");
}

void CompilationEngine::compileVarDec(std::size_t& varcount)
{
    writeXmlLeftTag("varDec");
    eat("var");
    auto type = _tokens_pointer->val;
    eatType();
    auto name = _tokens_pointer->val;
    eatIdentifier(); 
    _subruntSymbolTable.define(name, type, "local");
    varcount++;
    while (eatOptional(",")) {
        name = _tokens_pointer->val;
        eatIdentifier();
        _subruntSymbolTable.define(name, type, "local");
        varcount++;
    }       
    eat(";");
    writeXmlRightTag("varDec");
}

void CompilationEngine::compileStatements()
{
    writeXmlLeftTag("statements");
    compileStatement();
    writeXmlRightTag("statements");
}

void CompilationEngine::compileStatement()
{
    if (_tokens_pointer->val == "let") 
        compileLet();
    else if (_tokens_pointer->val == "if")
        compileIf();
    else if (_tokens_pointer->val == "while")
        compileWhile();
    else if (_tokens_pointer->val == "do")
        compileDo();
    else if (_tokens_pointer->val == "return")
        compileReturn();
    else 
        return;

    compileStatement();
}

void CompilationEngine::compileLet()
{
    writeXmlLeftTag("letStatement");
    eat("let");
    auto varname = _tokens_pointer->val;
    eatIdentifier();
    if (eatOptional("[")){
        CompileExpression();
        eat("]");
        eat("=");
        pushVar(varname);
        _vmwriter.writeArithmetic("add");
        CompileExpression();
        _vmwriter.writePop("temp", 0);
        _vmwriter.writePop("pointer", 1);
        _vmwriter.writePush("temp", 0);
        _vmwriter.writePop("that", 0);

    } else {
        eat("=");
        CompileExpression();
        popVar(varname);
    }
    eat(";");
    writeXmlRightTag("letStatement");
}

void CompilationEngine::compileIf()
{
    writeXmlLeftTag("ifStatement");
    std::string ifindexstr = std::to_string(ifIndex++);
    eat("if");
    eat("(");
    CompileExpression();
    eat(")");
    _vmwriter.writeIf(ifTureLable + ifindexstr);
    _vmwriter.writeGoto(ifFalseLable + ifindexstr);
    _vmwriter.writeLabel(ifTureLable + ifindexstr);
    eat("{");
    compileStatements();
    eat("}");
    _vmwriter.writeGoto(ifEndLable + ifindexstr);
    _vmwriter.writeLabel(ifFalseLable + ifindexstr);
    if(eatOptional("else")) {
        eat("{");
        compileStatements();
        eat("}");
    }
    _vmwriter.writeLabel(ifEndLable + ifindexstr);
    writeXmlRightTag("ifStatement");
}

void CompilationEngine::compileWhile()
{
    writeXmlLeftTag("whileStatement");
    auto index = std::to_string(ifIndex++);
    _vmwriter.writeLabel(whileStart + index);
    eat("while");
    eat("(");
    CompileExpression();

    eat(")");
    _vmwriter.writeArithmetic("not");
    _vmwriter.writeIf(whileEnd + index);
    eat("{");
    compileStatements();
    _vmwriter.writeGoto(whileStart + index);
    eat("}");
    _vmwriter.writeLabel(whileEnd + index);
    writeXmlRightTag("whileStatement"); 
}

void CompilationEngine::compileDo()
{
   writeXmlLeftTag("doStatement");
   eat("do");
   compileSubrountineCall();
   _vmwriter.writePop("temp", 0);
   eat(";");     
   writeXmlRightTag("doStatement"); 
}

void CompilationEngine::compileSubrountineCall()
{
    auto subname = _tokens_pointer->val;
    // 如果变量定义过，就不会返回null
    auto subtype = getType(subname);   // 如果是对象的化,获取这个对象的类型

    bool ismethod = (_subruntSymbolTable.kindOf(subname) != "null" || 
                    _classSymbolTable.kindOf(subname) != "null");

    if (ismethod) {
        pushVar(subname);
    }
    eatIdentifier();
    if (eatOptional("(")) {
        ismethod = true;

        _vmwriter.writePush("pointer", 0);
        subname = _class + "." + subname;
        CompileExpressionList(subname, ismethod);
        eat(")");
    } else if (eatOptional(".")) {
        auto callobj = (subtype == "null" ? subname : subtype);
        auto functionname = _tokens_pointer->val;
        eatIdentifier();
        eat("(");
        CompileExpressionList(callobj + "." + functionname, ismethod);
        eat(")");
    } else 
        error("compilesubrountinecall error!");
}

void CompilationEngine::compileReturn()
{
    writeXmlLeftTag("returnStatement");
    eat("return");
    if (!eatOptional(";")) {
        CompileExpression();
        eat(";");
        _vmwriter.writeReturn();
        writeXmlRightTag("returnStatement");
        return;
    } else {
        _vmwriter.writePush("constant", 0);
        _vmwriter.writeReturn();
    }
    writeXmlRightTag("returnStatement");
}

void CompilationEngine::CompileExpression()
{
    writeXmlLeftTag("expression");
    CompileTerm();
    std::string op = "+-*/&|<>=";
    auto o = _tokens_pointer->val;
    while (op.find(_tokens_pointer->val) != -1) {
        eat(_tokens_pointer->val);        
        CompileTerm();       
        _vmwriter.writeArithmetic(_opMap[o]);
    }
    writeXmlRightTag("expression");
}

void CompilationEngine::CompileTerm()
{

    auto val = _tokens_pointer->val;
    auto type = _tokens_pointer->type;

    // 这里处理常数
    if (type == INT_CONST ) {  
        _vmwriter.writePush("constant", std::stoi(val));
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
        tokenAdvance();
    } else if (type == STRING_CONST) {
        _vmwriter.writePush("constant", val.size());
        _vmwriter.writeCall("String.new", 1);
        for (auto i : val) {
            _vmwriter.writePush("constant", i);
            _vmwriter.writeCall("String.appendChar",2);
        }
        writeXmlTag(_tokens_pointer->val, _tokenTypeMap[_tokens_pointer->type]);
        tokenAdvance();
    } 
    else if (type == IDENTIFIER) {
        auto nexttoken = (_tokens_pointer+1)->val;
        if (nexttoken == "[") {         // 处理数组
            eatIdentifier();
            eat("[");
            CompileExpression();
            eat("]");
            pushVar(val);
            _vmwriter.writeArithmetic("add");
            _vmwriter.writePop("pointer", 1);
            _vmwriter.writePush("that", 0);

        } else if (nexttoken == "(" || nexttoken == ".") {  // 处理函数调用
            compileSubrountineCall();
        } else {           // 这里直接就是处理一个变量
            pushVar(val);
            eatIdentifier();  
        }
    } else {  // 处理一元运算符  
        if (_tokens_pointer->val == "false" ||
            _tokens_pointer->val == "null") {
            _vmwriter.writePush("constant", 0);
            eat(_tokens_pointer->val);
        } else if (_tokens_pointer->val == "true") {
            _vmwriter.writePush("constant", 1);
            _vmwriter.writeArithmetic("neg");
            eat(_tokens_pointer->val);
        } else if (val == "-" || 
                   val == "~") {
            eat(_tokens_pointer->val);
            CompileTerm();
            if (val == "-") 
                _vmwriter.writeArithmetic("neg");
            else if (val == "~") 
                _vmwriter.writeArithmetic("not");  
        } else if (_tokens_pointer->val == "("){
            eat("(");
            CompileExpression();
            eat(")");
        } else if (val == "this") {
            _vmwriter.writePush("pointer", 0);
            eat(_tokens_pointer->val);
        } else {
            std::string temp = "error in CompileTerm the current token is " + _tokens_pointer->val;
            error(temp);
        }
    }
}

void CompilationEngine::printsymboltable()
{
    _classSymbolTable.printSymbolTable();
}

void CompilationEngine::pushVar(std::string var_name)
{
    auto kind = _subruntSymbolTable.kindOf(var_name);
    if (kind == "null") {
        kind = _classSymbolTable.kindOf(var_name);
    }
    if (kind == "field") {
        _vmwriter.writePush("this", _classSymbolTable.indexOf(var_name));
    } else if (kind == "static") {
        _vmwriter.writePush("static",_classSymbolTable.indexOf(var_name));
    } else {
        _vmwriter.writePush(kind, _subruntSymbolTable.indexOf(var_name));
    }
}

void CompilationEngine::popVar(std::string varname)
{
    auto kind = _subruntSymbolTable.kindOf(varname);
    if (kind == "null") {
        kind = _classSymbolTable.kindOf(varname);
    }
    if (kind == "field") {
        _vmwriter.writePop("this", _classSymbolTable.indexOf(varname));
    } else if(kind == "static") {
        _vmwriter.writePop("static", _classSymbolTable.indexOf(varname));
    }else {
        _vmwriter.writePop(kind, _subruntSymbolTable.indexOf(varname));
    }
}


void CompilationEngine::setPointer(bool isConstructor, bool isMethod)
{
    if (isConstructor) {
        auto num = _classSymbolTable.varCount("field");
        _vmwriter.writePush("constant", num);
        _vmwriter.writeCall("Memory.alloc", 1);
        _vmwriter.writePop("pointer", 0);
    } 
    
    if (isMethod) {
        _vmwriter.writePush("argument", 0);
        _vmwriter.writePop("pointer", 0);
    }
}


std::string CompilationEngine::getType(const std::string& name)
{
    auto type = _subruntSymbolTable.typeOf(name);
    if (type == "null") {
        type = _classSymbolTable.typeOf(name);
    }
    return type;
}