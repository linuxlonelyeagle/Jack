#include "./include/JackTokenizer.h"

Tokenizer::Tokenizer(std::string filename):
_srcFile(filename), _nowLine(0), _index(0) {
    if (!_srcFile.is_open()) {
        std::cout << "open " << filename << " failed!" << std::endl;
    }
    std::stringstream temp;  
    temp << _srcFile.rdbuf();
    _srcCode = temp.str();
    initKeywordsAndSymbols();
    
}

Tokenizer::~Tokenizer() {
    _srcFile.close();
}

void Tokenizer::initKeywordsAndSymbols() {
    _symbols = "{}()[].,;+-*&|<>=~";
    _keywords.push_back("class");
    _keywords.push_back("constructor");
    _keywords.push_back("function");
    _keywords.push_back("method");
    _keywords.push_back("field");
    _keywords.push_back("static");
    _keywords.push_back("var");
    _keywords.push_back("int");
    _keywords.push_back("char");
    _keywords.push_back("boolean");
    _keywords.push_back("void");
    _keywords.push_back("true");
    _keywords.push_back("false");
    _keywords.push_back("null");
    _keywords.push_back("this");
    _keywords.push_back("let");
    _keywords.push_back("do");
    _keywords.push_back("if");
    _keywords.push_back("else");
    _keywords.push_back("while");
    _keywords.push_back("return");
}

bool Tokenizer::isIdentifierChar(char c) {
    return ((c >= 'A' && c <= 'Z') || 
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            (c == '_'));
}

bool Tokenizer::isKeyword(const std::string& str) {
    for (auto i : _keywords) {
        if (i == str) 
            return true;
    }
    return false;
}

// 把整个文件读取出来， 然后比较大小。
bool Tokenizer::hasMoreTokens() {
    if (_index < _srcCode.size()) 
        return true;
    return false;
}

void Tokenizer::advance() {
    while (true) {
        if (_index >= _srcCode.size())
            break;
        auto nowChar = _srcCode[_index];
        if (nowChar >= '0' && nowChar <= '9') {
            auto index = _index;
            while (_srcCode[_index] >= '0' && _srcCode[_index] <= '9') 
                _index++;
            std::string str = _srcCode.substr(index, _index - index);
            _token.type = INT_CONST;
            _token.val  = std::stoi(str);
            break;
        } else if ((nowChar >= 'a' && nowChar <= 'z') || 
                   (nowChar >= 'A' && nowChar <= 'Z')) {
            auto index = _index++;
            while (isIdentifierChar(_srcCode[_index])) 
                _index++;
            auto temp = _srcCode.substr(index, _index - index);
            if (isKeyword(temp)) {
                _token.type = KEYWORD;   
                _token.val  = temp;
            } else {
                _token.type = IDENTIFIER;
                _token.val  = temp;
            }
            break;
        } else if (nowChar == '"') {
            auto index1 = _index;
            auto index2 = _srcCode.find('"', index1 + 1);
            auto temp = _srcCode.substr(_index+1, index2 - index1 - 1);
            _token.type = STRING_CONST;
            _token.val = temp;
            _index = index2 + 1; 
            break;                
        }  else if (nowChar == '\n' || nowChar == '\t' || nowChar == '\r' || nowChar == ' ' || nowChar == '\0') {
            _index++;
            continue;
        } else if (nowChar == '/') {
            if (_srcCode[_index + 1] == '/') {
                auto index = _srcCode.find('\n',_index + 2);
                if (index == -1) {
                    std::cout << "// error!" << std::endl;
                    exit(0);
                }
                _index = index + 1;  
                continue;            
            } else if (_srcCode[_index + 1] == '*') {
                auto index = _srcCode.find("*/", _index + 2);     
                _index = index + 2;
                continue;          
            } else {
                _token.val = "/";
                _token.type = SYMBOL;
                _index++;
                break;
            }
        } else if (_symbols.find(nowChar) != -1) {
            _token.type = SYMBOL;
            _token.val  = std::string(1, nowChar);
            _index++;
            break;
        } else {
            std::cout << nowChar << std::endl;
            std::cout << "find token error!\n" << std::endl;
            exit(0);
        }
    }
}


Tokenizer::tokentype Tokenizer::tokenType() {
    return _token.type;
}

std::string Tokenizer::keyWord() {
    return std::get<std::string>(_token.val);
}

std::string Tokenizer::stringVal() {
    return std::get<std::string>(_token.val);
}

std::string Tokenizer::identifier() {
    return std::get<std::string>(_token.val);
}

std::size_t Tokenizer::intVal() {
    return std::get<int>(_token.val);
}

std::string Tokenizer::symbol() {
    return std::get<std::string>(_token.val);
}
