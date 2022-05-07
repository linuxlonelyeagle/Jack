#include "./include/CompilationEngine.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <string.h>



template<typename T>
std::vector<std::string> get_File(std::string Path, T&& f)
{
    struct stat st;
    if (stat(Path.c_str(), &st)) {
        std::cout << "get file stat faild!" << std::endl;
        return{};
    }
    std::vector<std::string > files;
    if (S_ISDIR(st.st_mode)) {
        DIR* dir = opendir(Path.c_str());
            while (dirent *info =  readdir(dir)) {
               if(strlen(info->d_name) > 5 && f(info->d_name)) {
                   files.push_back(Path + "/" + info->d_name);
               } 
            }   
    }
    else
        if (strlen(Path.c_str()) >  5 && f(Path))
            files.push_back(Path);
    return files;
}




int main(int argc, char* argv[])
{
    std::vector<std::string> files = get_File(argv[1],
    [](std::string file){
        if (file.find(".jack") != -1) 
            return true;
            return false;
    });
    
    for (auto file : files) {
        auto index1 = file.find_last_of('/');
        if (index1 == -1)
            index1 = 0;
        auto index2 = file.find(".jack", index1);
        auto temp = file.substr(0, index2);
        temp += ".vm";
        CompilationEngine c(file, temp);  
        c.CompileClass();
    }
    return 0;
}
