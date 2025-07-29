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
#include "DataManager.h"


class StatementExecutor {
public:
    StatementExecutor(SymbolTable& table, ProgramMemory& mem);
    void executeInput(InputNode* node);

    void execute(ASTNode* node);
    void setCurrentLine(int line);        // Needed for GOTO/FOR
    int getNextLine(int line) const;      // Chooses next line based on jump
    void requestJump(int targetLine);     // Called from GOTO/IF THEN GOTO

private:
    SymbolTable& table_;
    ExpressionEvaluator evaluator_;

    ProgramMemory& programMemory_;
    FlowControl flowControl_;
    DataManager dataManager_;

    int currentLine_ = -1;
    int jumpToLine_ = -1;


    void executePrint(PrintNode* printNode);
    void executeLet(LetNode* letNode);
    void executeIf(ASTNode* node);
    void executeFor(ForNode* forNode);
    Value evaluateExpr(ASTNode* exprNode);
    
};
