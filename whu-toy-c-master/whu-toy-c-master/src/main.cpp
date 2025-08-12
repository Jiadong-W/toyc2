// src/main.cpp
#include "parser.tab.h"
#include "ast.h"
#include <iostream>
#include "semantic.h"
#include "codegen.h"

extern FILE* yyin; // flex 提供的输入文件指针

int main(int argc, char** argv) {
    // Always read from stdin (interactive or redirected)
    yyin = stdin;

    CompUnit* root = nullptr;
    int ret = yyparse(&root);
    if (ret != 0 || !root) {
        std::cerr << "Syntax analysis failed\n";
        return 1;
    }
    // std::cout << "Syntax analysis succeeded\n";

    // semantic
    std::vector<FuncInfo> funcs;
    if (!semantic_analyze(root, funcs)) {
        std::cerr << "Semantic analysis failed\n";
        delete root;
        return 1;
    }
    // std::cout << "Semantic analysis succeeded\n";

    // generate assembly to stdout
    if (!generate_riscv(root, funcs, "-")) {
        std::cerr << "Code generation failed\n";
        delete root;
        return 1;
    }
    delete root;
    return 0;
}
