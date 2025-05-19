# Pipex - 42 Network Project

## 📝 Description
Pipex is a project that aims to deepen understanding of UNIX mechanisms by recreating the pipe operator (`|`) functionality. The program will execute commands and handle pipes between them, similar to how the shell handles the pipe operator.

## 🔄 Understanding Pipes

### Visual Representation of Pipe Mechanism

```
Basic Pipe (cmd1 | cmd2):
                                                   
infile                 pipe                 outfile 
[Data] --> [Process1] ====> [Process2] --> [Result]
   |          |             |              |
   |          |             |              |
   v          v             v              v
< file      ls -l         wc -l           > file
   
Legend: --> = data flow
```
```
Pipe Flow:

[infile] ---> [Process1]     [Process2]     [outfile]
                      |        |      |      
                      |        |      |
                      v        v      v
                    fd[1]    fd[0] [outfile]
                      |        |      |
                      +------->+      |
                                      |
                                      |
                                      |
                                      v
                                [final output]
```

```
Data Flow Example (ls -l | wc -l):

1. First Command (ls -l)
   +----------------+     +---------------+
   |     ls -l     | --> | Pipe Write fd |
   +----------------+     +---------------+
          |                      |
          v                      v
   reads from infile      writes to pipe

2. Second Command (wc -l)
   +---------------+     +-----------------+
   | Pipe Read fd  | --> |     wc -l      |
   +---------------+     +-----------------+
          |                      |
          v                      v
   reads from pipe        writes to outfile
```
## 🔍 Deep Dive: How Pipes Work

### Understanding Process Isolation and Pipes

In shell operations like `cmd1 | cmd2`, each command runs in its own isolated process. Here's why and how it works:

### Process Creation with fork()
```
Parent Process
      |
      |---> Child Process 1 (cmd1)
      |
      |---> Child Process 2 (cmd2)
```

- Every command needs its own isolated process
- `fork()` creates exact copies of the parent process
- Each child process is completely isolated and can't directly affect the parent
- Child processes can't communicate with each other by default

### Default Command Behavior
- Commands naturally read from stdin (0)
- Commands naturally write to stdout (1)
- We need to redirect these to make our pipe work

### File Redirection with dup2()
```
First Command:
stdin (0) --[dup2]--> infile
stdout (1) --[dup2]--> pipe write end

Last Command:
stdin (0) --[dup2]--> pipe read end
stdout (1) --[dup2]--> outfile
```
### Pipe Communication
Since processes are isolated, we need pipes to allow them to communicate:

```
Before fork():
Create Pipe: int fd[2]
      
         fd[0] = read end
         fd[1] = write end

After fork():
cmd1 ----[writes to fd[1]]-----> pipe ----[reads from fd[0]]-----> cmd2
```

### Step-by-Step Pipe Operation:

1. **Create the Pipe**
```c
int fd[2];
pipe(fd);  // Creates two file descriptors
// fd[0] = read end
// fd[1] = write end
```

2. **First Command Process**
```c
// Redirect input to come from infile
dup2(infile, STDIN_FILENO);
// Redirect output to go to pipe
dup2(fd[1], STDOUT_FILENO);
```

3. **Second Command Process**
```c
// Redirect input to come from pipe
dup2(fd[0], STDIN_FILENO);
// Redirect output to go to outfile
dup2(outfile, STDOUT_FILENO);
```

### Important Points to Remember:
- Pipes are unidirectional (one-way flow)
- Pipes are created before forking
- Each process should close unused pipe ends
- File descriptors must be properly redirected using `dup2()`
- Always check for errors in pipe creation and process forking
- Memory and file descriptors must be properly managed to prevent leaks
- Use `exit()` to terminate processes and prevent zombie processes.

### Common Pitfalls

1. **File Descriptor Leaks**
- Always close unused pipe ends
- Close all file descriptors in parent and child processes

2. **Zombie Processes**
- Wait for all child processes to complete
- Handle process termination properly

3. **Buffer Management**
- Pipes have limited buffer size
- Large data transfers might block

4. **Error Handling**
- Check all system calls return values
- Handle pipe errors and broken pipes

## 🎯 Mandatory Part
The program should behave exactly the same as the shell command below:
```bash
$> < infile cmd1 | cmd2 > outfile
```

### Program Usage
```bash
./pipex infile "cmd1" "cmd2" outfile
```

### Implementation Requirements
- Program must handle the path and environment variables (`$PATH`, `env`)
- Program must implement error handling with appropriate error messages
- Program must handle pipe redirection between two commands
- Memory management must be clean with no leaks, and all opened file descriptors should be properly closed.
- Your code should handle failures gracefully by cleaning up all resources, including allocated memory and opened file descriptors.

### Example Usage
```bash
./pipex infile "ls -l" "wc -l" outfile
# Should behave like: < infile ls -l | wc -l > outfile

./pipex infile "grep a" "wc -w" outfile
# Should behave like: < infile grep a | wc -w > outfile
```

## 🌟 Bonus Part

### Multiple Pipes
Handle multiple pipes like the shell command:
```bash
$> < infile cmd1 | cmd2 | cmd3 | ... | cmdn > outfile
```

Usage:
```bash
./pipex infile "cmd1" "cmd2" "cmd3" ... "cmdn" outfile
```

### Here_doc
Handle heredoc (`<<`) functionality like the shell command:
```bash
$> cmd1 << LIMITER | cmd2 >> outfile
```

Usage:
```bash
./pipex here_doc LIMITER "cmd1" "cmd2" outfile
```

Example:
```bash
./pipex here_doc END "grep hello" "wc -w" outfile
> hello world    # User input
> hello again    # User input
> END           # LIMITER to stop input
```

## 🛠️ Technical Implementation Guidelines

### Key Functions to Use
- `pipe()`: Create a new pipe
- `fork()`: Create a new process
- `dup2()`: Duplicate file descriptors
- `execve()`: Execute a program
- `wait()`: Wait for process to change state
- `access()`: Check file permissions
- `unlink()`: Remove a file
- `open()`: Open and possibly create a file
- `close()`: Close a file descriptor

### Error Handling
- Check all function returns
- Handle command not found
- Handle permission denied
- Handle file access errors
- Handle cases where the environment variables are unset.
- Display appropriate error messages using `perror()`

## ⚙️ Compilation
```bash
make        # Compile mandatory part
make bonus  # Compile with bonus features
make clean  # Remove object files
make fclean # Remove object files and executable
make fclean_bonus # Remove bonus object files and executable
make re     # Recompile
make re_bonus #Recompile bonus
```
### Debugging and Testing

To ensure your code is robust and free from memory leaks, remember to:

- Regularly test with various scenarios.
- Use **Valgrind** for memory management checks:
  ```bash
  valgrind --leak-check=full --track-fds=yes ./your_program
  ```
OR Compile your code with :
 ```bash
  gcc -fsanitize=address
```
NOTES : why is the $HOME get expanded in <DELINITER>
