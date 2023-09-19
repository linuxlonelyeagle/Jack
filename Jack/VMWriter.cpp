#include "./include/VMWriter.h"

VMWriter::VMWriter(std::string filename):
_writer(filename) {

}

VMWriter::VMWriter() {

}


VMWriter::~VMWriter() {
    _writer.close();
}


void VMWriter::writePush(const std::string& segment, std::size_t index) {

    std::string tmp = "push "  + segment + " " + std::to_string(index) + "\n";
    _writer.write(tmp.c_str(), tmp.size());   
}


void VMWriter::writePop(const std::string& segment, std::size_t index) {
    std::string tmp = "pop " + segment + " " + std::to_string(index) + "\n";
    _writer.write(tmp.c_str(), tmp.size());
}

void VMWriter::writeArithmetic(const std::string& command) {
    std::string tmp= command + "\n";
    _writer.write(tmp.c_str(), tmp.size());
}

void VMWriter::writeLabel(const std::string& label) {
    std::string tmp = "label " + label + "\n";
    _writer.write(tmp.c_str(), tmp.size());   
}

void VMWriter::writeGoto(const std::string& label) {
    std::string tmp = "goto " + label + "\n";
    _writer.write(tmp.c_str(), tmp.size());
}

void VMWriter::writeIf(const std::string& label) {
    std::string tmp = "if-goto " + label +"\n";
    _writer.write(tmp.c_str(), tmp.size()); 
}

void VMWriter::writeFunction(const std::string& functionname, int nLocals) {   
    std::string tmp = "function " + functionname + " " + std::to_string(nLocals) + "\n";
    _writer.write(tmp.c_str(), tmp.size());
}

void VMWriter::writeCall(const std::string& label, int nArgs) {
    std::string tmp = "call " + label + " " + std::to_string(nArgs) + "\n";
    _writer.write(tmp.c_str(), tmp.size());
}

void VMWriter::writeReturn() {
    std::string tmp = "return\n";
    _writer.write(tmp.c_str(), tmp.size());
}
