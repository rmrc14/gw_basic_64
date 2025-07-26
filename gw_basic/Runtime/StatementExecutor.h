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

class StatementExecutor {
public:
    StatementExecutor(SymbolTable& table);

    void execute(ASTNode* node);

private:
    SymbolTable& table_;
    ExpressionEvaluator evaluator_;

    void executePrint(PrintNode* printNode);
    void executeLet(LetNode* letNode);
    Value evaluateExpr(ASTNode* exprNode);
};