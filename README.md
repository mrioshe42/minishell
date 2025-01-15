# minishell

A custom shell implementation inspired by bash, developed as part of the 42 school curriculum. This project recreates basic shell functionality, allowing users to interact with their operating system through a command-line interface.

## 🚀 Features

- Command line parsing and execution
- Built-in commands implementation:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Environment variable management
- Signal handling (ctrl-C, ctrl-D, ctrl-\\)
- Pipes and redirections
- Quote handling (single and double)
- Error handling and status codes

## 🛠️ Technical Requirements

- Linux/Unix-based system
- GCC compiler
- GNU Make
- Readline library

## 📥 Installation

1. Clone the repository:

git clone https://github.com/mrioshe42/minishell.git
cd minishell

Compile the project:

make

## 💻 Usage

Run the shell:

./minishell

## 🔍 Implementation Details

- Written in C (99.3% of codebase)
- Uses readline library for input handling
- Implements process creation and management
- Handles command history
- Memory management and leak prevention

## 📝 License
This project is part of the 42 school curriculum.
