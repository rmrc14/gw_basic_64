# 👨‍💻 Team Member Workflow Guide – GWBasic64

This guide explains how to set up the repo, create your own feature branch, make changes, and push your work — all without needing pull requests (PRs), since direct pushes to `main` are currently allowed.

---

## 🔧 Prerequisites

Make sure you have:
- [Visual Studio 2022](https://visualstudio.microsoft.com/)
- Git for Windows: https://git-scm.com/
- C++ Desktop Development workload installed

---

## 🧭 Step-by-Step Setup & Contribution Guide

### 📥 1. Clone the Repository

Open Developer Command Prompt or Git Bash:

<pre>  git clone https://github.com/rmrc14/gw_basic_64.git 
cd gw_basic_64  </pre>

### 2. Create a New Branch for Your Work

<prev> git checkout -b feature/<your_module_or_task>  </prev>
Examples:

<prev>
git checkout -b feature/lexer
git checkout -b feature/parser-fix
git checkout -b feature/add-input-support  </prev>
This keeps everyone's changes organized.

### 🛠️ 3. Open the Project in Visual Studio
Launch Visual Studio 2022

Go to File > Open > Folder...

Select the gw_basic_64 folder you cloned

Visual Studio will auto-detect CMakeLists.txt and configure the project

### 🧪 4. Build and Run Tests
In Visual Studio:
<prev>
Click Build > Build All

Run all tests: Test > Test Explorer > Run All Tests </prev>

Or from terminal:

<prev>
ctest --output-on-failure -C Debug</prev>

### 💻 5. Make Your Changes
Edit files inside your assigned folder/module:

Lexer/, Parser/, Runtime/, etc.

Keep your work modular and test after every major change.

### 💾 6. Commit and Push Your Branch
<prev>
git add .
git commit -m "Add feature: parser enhancement"
git push origin feature/<your_module_or_task> </prev>
  
This pushes your branch to GitHub under your name.
  
### 🔄 7. Sync with main Without Losing Your Work
If main has been updated (e.g., someone pushed new changes to your module):

## ✅ Option 1: Safe Method – Merge main into your branch
<prev>
git checkout feature/<your_module_or_task>
git fetch origin
git merge origin/main </prev>
  
This brings the latest changes from main into your branch while preserving your work. You may be prompted to resolve conflicts if any.

### 🚀 8. Merge to main (If Allowed)
Only after your code is tested and working:
<prev>
git checkout main
git pull origin main
git merge feature/<your_module_or_task>
git push origin main </prev>

## 🧼 Best Practices
✅ Create a new branch for every task
✅ Build and test before every commit
✅ Keep commits focused – 1 feature = 1 commit (ideally)
✅ Use clear, descriptive commit messages
✅ Pull main regularly to avoid conflicts
✅ Don't edit others' modules without syncing




