# GWBasic64 – Modern C++ Interpreter for GW-BASIC

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

## 🧑‍💻 How to Set Up the Project on Windows (Using Visual Studio)

### 📥 1. Clone the Repository

Open **Developer Command Prompt for VS** or Git Bash:

```bash
git clone https://github.com/rmrc14/gw_basic_64.git
cd gw_basic_64 ```/bash
🧭 2. Open as a CMake Project in Visual Studio
Launch Visual Studio

Go to File > Open > Folder...

Select the root folder: gw_basic_64

Visual Studio will detect the CMakeLists.txt and configure automatically

🛠️ 3. Build the Project
From the top menu: Build > Build All

Or click the 🔨 Build icon

📦 GoogleTest will be downloaded and compiled automatically

🧪 4. Run the Unit Tests
Option A – Visual Studio GUI:
Go to Test > Test Explorer

Click Run All Tests

Option B – Command Line:
bash
Copy
Edit
cd build
ctest --output-on-failure -C Debug
🧩 Folder Structure
text
Copy
Edit
gw_basic_64/
├── main.cpp                  ← Entry point (REPL or File)
├── gw_basic/                 ← Core interpreter
│   ├── Lexer/                ← Tokenizer
│   ├── Parser/               ← AST builder
│   ├── Runtime/              ← Executor & Evaluator
│   ├── ProgramInterface/     ← REPL, CLI handling
│   ├── System/               ← System functions (memory, etc.)
│   ├── IO/                   ← Console I/O, FILES
│   ├── Errors/               ← Error handling
├── tests/                    ← Unit tests using GTest
│   └── test_dummy.cpp
├── CMakeLists.txt            ← Build system
├── .github/
│   └── workflows/ci.yml      ← GitHub Actions CI pipeline
👨‍💻 Team Workflow (No Pull Requests Required)
Since PRs are not enforced yet, contributors can follow this direct push workflow:

🔀 1. Create Your Feature Branch
bash
Copy
Edit
git checkout -b feature/<your_module>
Example: feature/parser, feature/runtime

💻 2. Make Your Changes
Work inside your assigned module folder:

Lexer/, Parser/, Runtime/, etc.

🔍 3. Build and Test
Ensure your changes compile cleanly and tests pass:

bash
Copy
Edit
ctest --output-on-failure -C Debug
☁️ 4. Push Changes
bash
Copy
Edit
git add .
git commit -m "Add <your_feature_description>"
git push origin feature/<your_module>
🔀 5. Merge to main (Optional, if allowed)
bash
Copy
Edit
git checkout main
git merge feature/<your_module>
git push origin main
✅ Best Practices
✅ Keep commits modular and clear

✅ Run tests before pushing
