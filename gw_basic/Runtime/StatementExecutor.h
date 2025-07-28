/*#pragma once

class StatementExecuter {
public:
    StatementExecuter();
    //void StatementExecuter::execute(const ASTNode&);
    // todo by nivya
};
*/


#pragma once
#include "ASTNode.h"
#include "SymbolTable.h"
#include "ExpressionEvaluator.h"
#include "ProgramMemory.h"

class StatementExecutor {
public:
    StatementExecutor(SymbolTable& table, ProgramMemory& mem);

    void execute(ASTNode* node);
    void setCurrentLine(int line);         // sets current executing line number
    int getNextLine(int line) const;       // decides the next line to jump to
    void requestJump(int targetLine);      // sets a GOTO or IF jump

private:
    SymbolTable& table_;
    ExpressionEvaluator evaluator_;

    void executePrint(PrintNode* printNode);
    void executeLet(LetNode* letNode);
    Value evaluateExpr(ASTNode* exprNode);

    ProgramMemory& programMemory_;
    int currentLine_ = -1;
    int jumpToLine_ = -1;


};