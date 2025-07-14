# GWBasic64 – Modern C++ Interpreter for GW-BASIC

Welcome to the GWBasic64 project – a 64-bit modern C++ implementation of a GW-BASIC interpreter.

This project includes:
- REPL and file mode execution
- Modular Lexer, Parser, Runtime, I/O system
- Unit tests using GoogleTest (no external installation needed)

---

## 🧰 Requirements

- [Visual Studio 2022 ]
- C++ Desktop Development workload
- Git
---

## 🧑‍💻 How to Set Up the Project on Windows (Using Visual Studio)

### 📥 1. Clone the Repository

Open **Developer Command Prompt for VS** :


git clone https://github.com/rmrc14/gw_basic_64.git

cd gw_basic_64

🧭 2. Open as CMake Project in Visual Studio
Launch Visual Studio

Go to File > Open > Folder...

Select the folder: gw_basic_64

Visual Studio will detect the CMakeLists.txt and configure automatically

🛠️ 3. Build the Project
From the top menu: Build > Build All

Or click the 🔨 icon

It will download GoogleTest automatically and compile everything

🧪 4. Run the Unit Tests
From Visual Studio:

Go to Test > Test Explorer

Click Run All Tests

Or from terminal:
cd build
ctest --output-on-failure -C Debug

🧩 Folder Structure

<pre> <code>```text gw_basic_64/ ├── main.cpp ← Entry point (REPL or File) ├── gw_basic/ ← Core interpreter │ ├── Lexer/ ← Tokenizer │ ├── Parser/ ← AST builder │ ├── Runtime/ ← Executor & Evaluator │ ├── ProgramInterface/ ← REPL, CLI handling │ ├── System/, IO/, Errors/ ← Support modules ├── tests/ ← Unit tests using GTest │ ├── test_dummy.cpp ├── CMakeLists.txt ← Build system ├── .github/workflows/ci.yml ← CI pipeline ```</code> </pre>
🧑‍💻 Team Workflow (Without PRs)
Since PR reviews are not enforced yet, follow this direct-push workflow:

1. Create Your Own Branch

git checkout -b feature/<your_module>
E.g., feature/parser, feature/runtime

2. Make Changes in Your Module Folder
Use the right folder: Lexer/, Parser/, Runtime/, etc.

3. Build and Run Tests
Ensure your code builds and passes tests before pushing.

4. Push Your Changes

git add .
git commit -m "Add parser expression support"
git push origin feature/parser

6. Merge to main (Optional)
If you're working in a small team and allowed to push:

git checkout main
git merge feature/parser
git push origin main

❗ Best Practices 
Keep your changes modular

Always run tests before pushing

