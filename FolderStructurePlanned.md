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
│   │   ├── ScreenRender.h
│   │   └── SpecialKeyHandler.h      ← (advanced REPL keys)
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
│   │   ├── ExpressionEvaluator.h    ← (for parsing math)
│   │   ├── ExpressionEvaluator.cpp
│   │   ├── DataManager.cpp
│   │   ├── DataManager.h
│   │   ├── FlowControl.cpp
│   │   ├── FlowControl.h
│   │   ├── IOHandler.cpp
│   │   ├── IOHandler.h
│   │   ├── SubroutineManager.cpp    -->assigned to magesh
│   │   ├── SubroutineManager.h
│   │   ├── FunctionLibraries.cpp    -->assigned to vijaylaxmi
│   │   ├── FunctionLibraries.h
│   │   ├── SymbolTable.h
│   │   ├── SymbolTable.cpp
│   │   ├── TypeSystem.cpp
│   │   └── TypeSystem.h             ← Type system (int, string, etc.)
│   │
│   │
│   ├── System/                      ← System functions & utilities
│   │   ├── SystemInterface.h
│   │   ├── SystemInterface_Windows.cpp
│   │   ├── SystemInterface_MiniOS.cpp  //custom OS to be implemented do we need asm? requires x86_64-elf g++ cross compiler
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
