#include <iostream>
#include <vector>
#include "smCompiler.hpp"
using namespace std;

int mmkay() {
    vector<string> program = {
        "var x, y, z;",
        "procedure p;",
        "   var m;",
        "   begin",
        "       x := 10;",
        "       m := x;",
        "   end;",
        "       ",   
        "   begin",
        "      call p;",
        "      y := x;",
        "      z := y + 15;",
        "   end." };
        smCompiler compiler(program);
        compiler.compile(true);
    return 0;
}

int main() {
    mmkay();
    return 0;
}