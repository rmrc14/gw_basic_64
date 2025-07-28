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
#include "FlowControl.h"

class StatementExecutor {
public:
    StatementExecutor(SymbolTable& table, ProgramMemory& mem);

    void execute(ASTNode* node);
    void setCurrentLine(int line);        // Needed for GOTO/FOR
    int getNextLine(int line) const;      // Chooses next line based on jump
    void requestJump(int targetLine);     // Called from GOTO/IF THEN GOTO

private:
    SymbolTable& table_;
    ExpressionEvaluator evaluator_;
    ProgramMemory& programMemory_;
    FlowControl flowControl_;

    int currentLine_ = -1;
    int jumpToLine_ = -1;

    void executePrint(PrintNode* printNode);
    void executeLet(LetNode* letNode);
    void executeIf(ASTNode* node);
    Value evaluateExpr(ASTNode* exprNode);
};
