# 🐚 Minishell

`minishell` is a simplified UNIX shell, implemented in C, as part of the **42 Abu Dhabi Core Curriculum**.  
It replicates basic shell behavior including **built-in commands**, **redirections**, **pipes**, **environment variables**, and **signal handling**.

![Language](https://img.shields.io/badge/C-100%25-blue)
![Project](https://img.shields.io/badge/42%20Project-Minishell-green)
![Status](https://img.shields.io/badge/Status-Finished-success)

---

## 🧩 About

The goal of this project is to create a functioning shell with a subset of Bash’s capabilities.  
It handles command execution, redirections, pipes, and built-ins, while respecting POSIX standards and custom error/signal handling.

> ✅ No use of system()  
> ✅ No external parser (must write your own)  
> ✅ Must support proper memory and file descriptor management

---

## 🚀 Features

- ✅ Execution of binary files and built-in shell commands
- ✅ I/O Redirections: `>`, `>>`, `<`, `<<` (here_doc)
- ✅ Pipelines: `cmd1 | cmd2 | cmd3`
- ✅ Environment variables: `$PATH`, `$USER`, etc.
- ✅ `export`, `unset`, `env` command support
- ✅ Proper signal handling: `Ctrl+C`, `Ctrl+\`, etc.
- ✅ Exit codes for last executed command
- ✅ Quotes and escape sequences (`'`, `"`, `\`)

---

## 🛠 Installation

## Consider
>checking all edge cases open fds at the end of the child
>multi envir or multi $ in the terminal or in the heredoc "<"

```bash
git clone https://github.com/seeknoobwidom/minishell.git
cd minishell
make
