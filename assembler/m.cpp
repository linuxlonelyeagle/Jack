#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <variant>
#include <initializer_list>
#include <string.h>
using namespace std;

template<unsigned M, unsigned N>
int compare(const char (&p1)[M], const char (&p2)[N])
{
    cout << M << " " << N << endl;
    return strcmp(p1, p2);   
}

int main(int argc, char* argv[]) 
{   char p[] = "heloo";
    printf("%s",p);
    return 0;
}
