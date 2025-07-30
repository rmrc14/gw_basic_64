#include "StatementExecutor.h"
#include <iostream>
#include <sstream> 

StatementExecutor::StatementExecutor(SymbolTable& table, ProgramMemory& mem)
    : table_(table), evaluator_(table), programMemory_(mem), flowControl_(table) {}

void StatementExecutor::setCurrentLine(int line) {
    currentLine_ = line;
    jumpToLine_ = -1;  // Reset previous jump

}




int StatementExecutor::getNextLine(int line) const {
    return (jumpToLine_ != -1) ? jumpToLine_ : programMemory_.getNextLineNumber(line);
}

void StatementExecutor::requestJump(int targetLine) {
    jumpToLine_ = targetLine;
}






void StatementExecutor::execute(ASTNode* node) {
    if (!node) return;

    switch (node->type()) {

    case ASTType::RemStmt:
        // Do nothing, REM is just a comment.
        return;

    case ASTType::IfElseStmt:
        executeIf(node);
        break;

    case ASTType::FieldStmt:
        executeField(static_cast<FieldNode*>(node));
        break;


    case ASTType::ForStmt:
        executeFor(static_cast<ForNode*>(node));  
        break;


    case ASTType::GosubStmt: {
        GosubNode* gosub = static_cast<GosubNode*>(node);
        int returnLine = programMemory_.getNextLineNumber(currentLine_);
        subroutineManager_.pushReturnAddress(returnLine);  // Push next line
        requestJump(gosub->line);  // Jump to subroutine
        break;
    }

    case ASTType::ReturnStmt: {
        if (!subroutineManager_.hasReturnAddress()) {
            throw std::runtime_error("RETURN without GOSUB");
        }
        int returnLine = subroutineManager_.popReturnAddress();
        requestJump(returnLine);
        break;
    }


    case ASTType::Program: 
    {
        ProgramNode* prog = static_cast<ProgramNode*>(node);
        for (auto stmt : prog->stmts) {
            execute(stmt);
        }
        break;
    }

    case ASTType::PrintStmt:
        executePrint(static_cast<PrintNode*>(node));
        break;

    case ASTType::StopStmt:
        std::exit(0); // Or throw an exception if you want custom STOP handling
        break;

    case ASTType::LetStmt:
        executeLet(static_cast<LetNode*>(node));
        break;

    case ASTType::DoLoopStmt:
        executeDoLoop(static_cast<DoLoopNode*>(node));
        break;


    case ASTType::InputStmt:
        executeInput(static_cast<InputNode*>(node));
        break;

    case ASTType::DataStmt: {
        auto* dataNode = static_cast<DataNode*>(node);
        std::vector<Value> parsedValues;

        for (const auto& valStr : dataNode->values) {
            std::string trimmed = valStr;
            trimmed.erase(0, trimmed.find_first_not_of(" \t"));
            trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

            // If it’s a string literal (starts and ends with quotes)
            if (trimmed.size() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
                parsedValues.emplace_back(trimmed.substr(1, trimmed.size() - 2));  // Remove quotes
            }
            else {
                Value val = evaluator_.evaluate(trimmed);  // Must be number or variable
                parsedValues.push_back(val);
            }
        }

        dataManager_.addData(parsedValues);
        break;
    }

    

    case ASTType::ReadStmt: {
        auto* read = static_cast<ReadNode*>(node);
        Value v = dataManager_.readNext();
        table_.setVariable(read->var, v);
        break;
    }
    

    case ASTType::GotoStmt: {
        GotoNode* gotoNode = static_cast<GotoNode*>(node);
        requestJump(gotoNode->line);
        break;
       /* class GotoNode :public ASTNode {
        public:
            int line;
            GotoNode(int n) {
                line = n;
            }
            ASTType type() const override {
                return ASTType::GotoStmt;
            }
        };*/
    }

    


    default:
        
        break;
    }
}

void StatementExecutor::executePrint(PrintNode* printNode) {
    Value result = evaluateExpr(printNode->expr);

    switch (result.getType()) {
    case ValueType::INT:
        std::cout << result.asInt() << std::endl;
        break;
    case ValueType::FLOAT:
        std::cout << result.asFloat() << std::endl;
        break;
    case ValueType::STRING:
        std::cout << result.asString() << std::endl;
        break;
    default:
        std::cout << "Unknown value type" << std::endl;
    }
}

void StatementExecutor::executeLet(LetNode* letNode) {
    Value result = evaluateExpr(letNode->expr);
    table_.setVariable(letNode->name, result);
}

Value StatementExecutor::evaluateExpr(ASTNode* exprNode) {
    switch (exprNode->type()) {
    case ASTType::NumberExpr:
        return Value(std::stoi(static_cast<NumberNode*>(exprNode)->value));

    case ASTType::IdentExpr:
        return table_.getVariable(static_cast<IdentNode*>(exprNode)->name);

    case ASTType::StringExpr:
        return Value(static_cast<StringNode*>(exprNode)->value);

    case ASTType::BinOpExpr: {
        BinOpNode* bin = static_cast<BinOpNode*>(exprNode);
        Value leftVal = evaluateExpr(bin->left);
        Value rightVal = evaluateExpr(bin->right);

        if (bin->op == "+") {
            if (leftVal.getType() == ValueType::STRING || rightVal.getType() == ValueType::STRING)
                return Value(leftVal.asString() + rightVal.asString());
            return Value(leftVal.asInt() + rightVal.asInt());
        }
        else if (bin->op == "-") {
            return Value(leftVal.asInt() - rightVal.asInt());
        }
        else if (bin->op == "*") {
            return Value(leftVal.asInt() * rightVal.asInt());
        }
        else if (bin->op == "/") {
            if (rightVal.asInt() == 0) throw std::runtime_error("Division by zero");
            return Value(leftVal.asInt() / rightVal.asInt());
        }
        else {
            throw std::runtime_error("Unknown operator: " + bin->op);
        }
    }

    default:
        throw std::runtime_error("Unsupported expression type");
    }
}

void StatementExecutor::executeIf(ASTNode* node) {
    auto* ifElse = static_cast<IfElseNode*>(node);
    ASTNode* lhs = ifElse->left;
    std::string op = ifElse->op;
    ASTNode* rhs = ifElse->right;
    ASTNode* thenStmt = ifElse->thenStmt;
    ASTNode* elseStmt = ifElse->elseStmt;

    Value leftVal = evaluateExpr(lhs);
    Value rightVal = evaluateExpr(rhs);

    if (flowControl_.evaluateCondition(op, leftVal, rightVal)) {
        execute(thenStmt);
    }
    else if (elseStmt) {
        execute(elseStmt);
    }
}



void StatementExecutor::executeFor(ForNode* forNode) {
    std::string varName = forNode->var;
    Value startVal = evaluateExpr(forNode->start);
    Value endVal = evaluateExpr(forNode->end);
    Value stepVal = forNode->step ? evaluateExpr(forNode->step) : Value(1);

    table_.setVariable(varName, startVal);

    while (true) {
        int current = table_.getVariable(varName).asInt();
        int end = endVal.asInt();
        int step = stepVal.asInt();

        if ((step > 0 && current > end) || (step < 0 && current < end))
            break;


        execute(forNode->body);  


        table_.setVariable(varName, Value(current + step));
    }
}


void StatementExecutor::executeInput(InputNode* node) {
    if (!node) {
        std::cerr << "[System Error] : INPUT requires a variable name\n";
        return;
    }

    std::string input;
    std::cout << "? ";
    std::getline(std::cin, input);

    Value val;
    try {
        if (input.find('.') != std::string::npos) {
            val = Value(std::stof(input));
        }
        else {
            val = Value(std::stoi(input));
        }
    }
    catch (...) {
        val = Value(input);  // Fallback as string
    }

    table_.setVariable(node->name, val);
}
void StatementExecutor::executeDoLoop(DoLoopNode* node) {
    if (!node) return;

    if (!node->body) {
        std::cout << "[System Error] : DO missing LOOP" << std::endl;
        return;
    }

    while (true) {
        execute(node->body);  // run the loop body

        // No condition = infinite loop (break manually if desired)
        if (!node->cond) {
            break;  // Or remove this to allow infinite loops
        }

        Value condVal = evaluateExpr(node->cond);
        bool result = condVal.asInt();

        if (node->untilStyle) {
            if (result) break; // UNTIL cond true → exit
        }
        else {
            if (!result) break; // WHILE cond false → exit
        }
    }
}
void StatementExecutor::executeField(FieldNode* node) {
    std::cout << "DEBUG: fileNum = " << node->fileNum << "\n";
    if (node->fileNum <= 0) {
        std::cerr << "[System Error] : FIELD needs file number\n";
        return;
    }

    std::cout << "[FIELD #" << node->fileNum << "] ";
    for (auto& f : node->fields)
        std::cout << f.second << "(" << f.first << ") ";
    std::cout << "\n";
}




