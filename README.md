# autocompile & maketree

A lightweight, zero-dependency C++ build toolset for automatic `Makefile` generation and parallel multi-directory project orchestration.

---

## 🌟 Overview

`autocompile` eliminates the burden of manually writing and updating Makefiles for C/C++ projects. It automatically parses `#include` header dependencies using the compiler (`g++ -MM`), generates optimal Makefiles (including precompiled header support), and orchestrates multi-directory builds in parallel using directed acyclic graph (DAG) dependency resolution.

* **Zero External Dependencies**: Built 100% with standard C++ (C++17).
* **Automatic Dependency Tracking**: Scans `.cpp`, `.c`, `.hpp`, and `.h` files and calculates header/PCH dependencies dynamically.
* **Parallel DAG Traversal**: `maketree` builds independent subdirectories concurrently using worker threads (`std::thread`).
* **Cycle Detection**: Automatically detects cyclic folder dependencies.

---

## 🛠️ Tools Included

### 1. `autocompile`
Directory-level Makefile generator. Scans source files in the current folder, computes compiler/header dependencies, and outputs a complete `Makefile` to `stdout`.

```bash
# Generate a Makefile in the current directory
autocompile > Makefile
```

### 2. `maketree`
Multi-directory parallel build orchestrator. Recursively navigates project subdirectories in dependency order (defined by `dirs` files) and executes `make` or custom target commands.

```bash
# 1. Bootstrap Makefiles in a fresh project tree
maketree -f /path/to/autocompile/bootstrap

# 2. Regenerate all Makefiles across subdirectories
maketree autocompile

# 3. Build the entire project tree recursively
maketree

# 4. Clean all build outputs recursively
maketree clean
```

#### `maketree` Command Line Options:
* `-1`: Single-threaded execution mode.
* `-v`: Dry-run verbose mode (prints commands to execute without running them).
* `<target>`: Target command passed to `make` in each subdirectory (e.g., `clean`, `autocompile`).

---

## ⚙️ Configuration Files

### `.autocompile` Configuration
`autocompile` searches for configuration directives in `$HOME/.autocompile`, parent directory `.autocompile` files, and local `.autocompile`.

| Directive | Alias | Description | Default |
| :--- | :--- | :--- | :--- |
| `out` | `output` | Output binary filename | `main` |
| `main` | - | C++ source file containing `main()` | `main.cpp` |
| `cc-flag` | `cc_flag` | Primary compiler flags | `-std=c++17 -Wall -O3` |
| `cc-flag2` | `cc_flag2` | Additional linker/post flags | *(empty)* |
| `dlibs` | `dynamic-libs` | Dynamic libraries to link (`-llibname`) | *(empty)* |
| `objs` | `libs`, `static-libs` | Static libraries or `.o` object files to link | *(empty)* |
| `cc` | `compile-link` | Compiler executable format string | `g++ %1% -o %2%` |
| `cc-m` | `cc_m` | Dependency generation command string | `g++ -MM %1%` |
| `cc-h` | `cc_h` | Precompiled header compilation format string | `g++ -x c++-header %1% -o %2%` |

---

### `dirs` File (Directory Dependency Graph)
Place a `dirs` file in a directory to configure folder exclusion and inter-directory build dependencies:

```text
# Ignore build or experimental subdirectories
-experimental
-temp_build

# Declare build dependencies (folder1 depends on folder2 and folder3)
folder1: folder2 folder3
```

---

## 🚀 Quick Start Guide

### 1. Installation & Build
```bash
git clone https://github.com/zijiehe/autocompile.git
cd autocompile
./compile.O3.sh

# Add autocompile to your PATH
export PATH=$(pwd):$PATH
```

### 2. Bootstrapping & Building a Project
```bash
cd /path/to/your/project

# 1. Initialize Makefiles across all subdirectories
maketree -f /path/to/autocompile/bootstrap

# 2. Build the entire project tree
maketree

# 3. Clean build outputs
maketree clean
```

---

## 📄 License
Released under the MIT License.
