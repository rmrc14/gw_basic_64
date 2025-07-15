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

<pre> git checkout -b feature/<your_module_or_task>  </pre>
Examples:

<pre>
git checkout -b feature/lexer
git checkout -b feature/parser-fix
git checkout -b feature/add-input-support  </pre>
This keeps everyone's changes organized.
