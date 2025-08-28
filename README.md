# hy345sh

A custom shell (terminal) implemented in C for the HY345 Operating Systems course.  
The shell supports process creation, pipelines, environment variables, and input/output redirection.

---

## ✨ Features

- **Basic command execution**  
  Runs standard UNIX commands (`ls`, `cat`, `date`, etc.) by creating child processes with `fork()` and `exec()`.

- **Multiple commands**  
  Supports multiple commands separated by `;`.  
  Example:  
  ```bash
  ls; date; mkdir test

- **Arguments** 

Handles commands with arguments (e.g., `ls -al`).

- **Environment variables** 

Allows definition and usage of shell variables

```bash
x=world
echo hello $x   # prints "hello world"
```

- **Pipes** 

Redirects the output of one command as the input to another.  
Example:

```bash
ls -al | grep ".c"
```

- **I/O Redirection**
    - Input redirection (`<`)
    ```bash
    cat < input.txt
    ```
    
    - Output redirection (`>`) (overwrite)
    ```bash
    ls -al > output.txt
    ```
    - Append redirection (`>>`)
    ```bash
    ls >> files.txt
    ```

- **Custom prompt**

The shell has the form:

```bash
<AM>-hy345sh@<User>:<Directory>
```