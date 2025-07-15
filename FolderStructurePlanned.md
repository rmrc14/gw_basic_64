```bash
﻿gw_basic_64/
├── CMakeLists.txt
├── README.md
├── main.cpp                          ← Entry point (REPL or File mode)
│
├── gw_basic/                         ← Core interpreter modules
│   ├── GWBasic64.h                   ← Main interpreter class
│   ├── GWBasic64.cpp
│
│   ├── ProgramInterface/            ← REPL, screen rendering, input
│   │   ├── CommandLineEditor.h
│   │   ├── CommandLineEditor.cpp
│   │   └── SpecialKeyHandler.h      ← (optional advanced REPL keys)
│
│   ├── Storage/                     ← Stores program lines, line editing
│   │   ├── ProgramMemory.h
│   │   ├── ProgramMemory.cpp
│
│   ├── Lexer/                       ← Tokenizer
│   │   ├── Lexer.h
│   │   ├── Lexer.cpp
│
│   ├── Parser/                      ← AST builder
│   │   ├── Parser.h
│   │   ├── Parser.cpp
│
│   ├── Runtime/                     ← Executor & evaluator
│   │   ├── StatementExecutor.h
│   │   ├── StatementExecutor.cpp
│   │   └── ExpressionEvaluator.h   ← (optional, for parsing math)
│
│   ├── IO/                          ← I/O (optional: file IO, console)
│   │   ├── ConsoleIO.h              ← Abstracts `PRINT`, `INPUT`
│   │   ├── ConsoleIO.cpp
│
│   ├── System/                      ← System functions & utilities
│   │   ├── SystemInterface.h
│   │   ├── SystemInterface.cpp
│   │   ├── MemoryInterface.h        ← Simulate memory (optional)
│   │   └── MemoryInterface.cpp
│
│   ├── Errors/                      ← Error handling
│   │   ├── ErrorHandler.h
│   │   └── ErrorHandler.cpp
│
│   └── Utils/                       ← Common structs & types
│       ├── Token.h                  ← Token structure
│       ├── ASTNode.h                ← AST tree structure
│       └── Types.h                  ← Type system (int, string, etc.)
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
