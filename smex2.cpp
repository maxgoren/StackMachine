#include <iostream>
#include <vector>
#include "stackmachine.hpp"
using namespace std;


int main() {
      vector<string> program = {
             "push 7",
             "show",
             "push 1",
             "sub",
             "jmpnz 1",
             "halt"
             };
      vector<string> program2 = {
            "push 7", 
            "push 3",
            "cmpeq",
            "jmpnz 7",
            "cmpl",
            "jmpnz 10",
            "halt",
            "push 13",
            "show",
            "retf",
            "push 21",
            "show",
            "retf"
      };
       StackMachine sm(true);
       sm.run(program2);
       return 0;
}