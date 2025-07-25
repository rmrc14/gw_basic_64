```bash
﻿gw_basic_64/
├── CMakeLists.txt
├── README.md
├── main.cpp                          ← Entry point (REPL or File mode)
│
├── gw_basic/                         ← Core interpreter modules
│   ├── GWBasic64.h                   ← Main interpreter class
│   ├── GWBasic64.cpp
│   │
│   ├── ProgramInterface/            ← REPL, screen rendering, input
│   │   ├── CommandLineEditor.h
│   │   ├── CommandLineEditor.cpp
│   │   └── SpecialKeyHandler.h      ← (optional advanced REPL keys)
│   │
│   ├── Storage/                     ← Stores program lines, line editing
│   │   ├── ProgramMemory.h
│   │   ├── ProgramMemory.cpp
│   │
│   ├── Lexer/                       ← Tokenizer
│   │   ├── Lexer.h
│   │   ├── Lexer.cpp
│   │   └── Token.h                  ← Token structure
│   ├── Parser/                      ← AST builder
│   │   ├── Parser.h
│   │   ├── Parser.cpp
│   │   └── ASTNode.h                ← AST tree structure
│   │  
│   ├── Runtime/                     ← Executor & evaluator
│   │   ├── StatementExecutor.h
│   │   ├── StatementExecutor.cpp
│   │   ├── ExpressionEvaluator.h    ← (optional, for parsing math)
│   │   ├── ExpressionEvaluator.cpp
│   │   ├── SymbolTable.h
│   │   ├── SymbolTable.cpp
│   │   └── TypeSystem.h             ← Type system (int, string, etc.)
│   │
│   │
│   ├── System/                      ← System functions & utilities
│   │   ├── SystemInterface.h
│   │   ├── SystemInterface_Windows.cpp
│   │   ├── SystemInterface_MiniOS.cpp  //custom OS to be implemented do we need asm?
│   │
│   ├── Errors/                      ← Error handling
│   │   ├── ErrorHandler.h
│   │   └── ErrorHandler.cpp
│   
│   
│                       
│       
│
├── tests/                           ← Unit tests
│   ├── CMakeLists.txt
│   ├── test_main.cpp
│   ├── test_lexer.cpp
│   ├── test_parser.cpp
│   └── test_executor.cpp
│
└── examples/                        ← Sample .BAS files to run
    ├── hello.bas
    ├── loop.bas
    └── calculator.bas
