#ifndef __INCLUDE__VMWRITER__H
#define __INCLUDE__VMWRITER__H

#include <fstream>

class VMWriter{
    private:
        std::ofstream _writer;
    public:
        VMWriter(std::string filename);
        VMWriter();
        ~VMWriter();
        void writePush(const std::string& segment, std::size_t index);
        void writePop(const std::string& segment, std::size_t index);
        void writeArithmetic(const std::string& command);
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIf(const std::string& label);
        void writeCall(const std::string& label, int nArgs);
        void writeFunction(const std::string& name, int nLocals);
        void writeReturn();
     
};

#endif
