# GWBasic64 – 64-bit C++ Interpreter for GW-BASIC

[![Build Status](https://github.com/rmrc14/gw_basic_64/actions/workflows/ci.yml/badge.svg)](https://github.com/rmrc14/gw_basic_64/actions)
![GitHub repo size](https://img.shields.io/github/repo-size/rmrc14/gw_basic_64)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-blue.svg)



Welcome to the **GWBasic64** project – a 64-bit modern C++ implementation of a GW-BASIC interpreter.

This project includes:
- REPL and file mode execution
- Modular Lexer, Parser, Runtime, I/O system
- Unit tests using GoogleTest (auto-downloaded via CMake)

---

## 🧰 Requirements

- [Visual Studio 2022](https://visualstudio.microsoft.com/)
- C++ Desktop Development workload
- Git

---



## 🧩 Folder Structure 

```bash
gw_basic_64/
├── main.cpp                  ← Entry point (REPL or File)
├── gw_basic/                 ← Core interpreter
│   ├── Lexer/                ← Tokenizer
│   ├── Parser/               ← AST builder
│   ├── Runtime/              ← Executor & Evaluator
│   ├── ProgramInterface/     ← REPL, CLI handling
│   ├── System/               ← System functions (memory, etc.)
│   ├── Errors/               ← Error handling
├── tests/                    ← Unit tests using GTest
│   └── test_dummy.cpp
├── CMakeLists.txt            ← Build system
├── .github/
│   └── workflows/ci.yml      ← GitHub Actions CI pipeline (not implemented)

