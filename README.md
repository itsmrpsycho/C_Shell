# C shell

## Description
A custom shell that supports user and system commands, piping and redirecting commands, process management, and networking. It simulates a Linux Shell.

## Shell Specifications

### User Interface
- Display prompt with current working directory and username.
- Parse user input for commands and arguments.
- Handle `;` and `&` separated command lists.
- Support for random spaces and tabs in input.

### Built-in Commands
- **warp**: Change directory (supports absolute, relative paths, home, and multiple arguments).
- **peek**: List files and directories (supports `-a`, `-l`, `.`, `..`, `~`, `-`).
  - Files in white, directories in blue, executables in green.
- **pastevents**: Manage command history (stores last 15 commands, purges history, executes commands).
- **proclore**: Display process information (ID, status, group, virtual memory, executable path).
- **seek**: Search files and directories (supports `-d`, `-f`, `-e`).

### External Commands
- Execute system commands (e.g., `emacs`, `sleep`, `echo`, `gedit`) in foreground and background.
- Print execution time for foreground processes > 2 seconds.
- Display PIDs and termination messages for background processes.

### I/O Redirection
- Support for `>`, `>>`, `<` operators.
- Handle non-existent input files, create output files with appropriate permissions.

### Pipes
- Connect multiple commands using pipes (`|`).
- Support for combining I/O redirection with pipes.

### Process Management
- List running foreground and background processes.
- Send signals to processes (`ping <pid> <signal_number>`).
- Handle keyboard interrupts (Ctrl-C), logout (Ctrl-D), backgrounding (Ctrl-Z).
- Manage processes (`fg <pid>`, `bg <pid>`).

### Other Features
- **neonate**: Print PID of the most recently created process every n seconds until `x` is pressed.
- **iMan**: Fetch and display man pages from a website.

## Steps to Run the Shell
1. Clone the repository.
2. Run `make` in the terminal.
3. Execute the shell with `./a.out`.

## Assumptions
1. Input may be delimited by `;` and `&`.
2. Output of background processes is printed after execution and redirected to an external file.
3. `exit` command exits the shell; additional arguments are ignored.
4. Shell remains in the same directory if an incorrect path is provided.
5. `pastevents execute` outputs only for valid indexes.
6. `proclore` returns an error for incorrect PID.
7. `seek` returns "no match found" for incorrect arguments.
8. `peek` returns an error for an incorrect directory.
9. `pastevents` adds incorrect and background events to history.
10. `peek` returns nothing if empty.

### Reference and problem statement:
[C Shell project](https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1).
