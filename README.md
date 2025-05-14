
# Pipex

## Overview

**Pipex** is a C project that replicates the Unix shell's piping and redirection behavior.
It allows you to execute a sequence of commands, connecting their input and output streams via pipes,
similar to how you would use `|` in a shell. The project is divided into a mandatory part (2 commands)
and a bonus part (multiple commands and here_doc support).

## Features

### Mandatory

- Execute two commands with a pipe:

```bash
./pipex infile "cmd1" "cmd2" outfile
```

Equivalent to:

```bash
< infile cmd1 | cmd2 > outfile
```

- Handles:
  - Input/output file errors
  - Command not found
  - Permission denied

### Bonus

- Supports an arbitrary number of commands:

```bash
./pipex_bonus infile "cmd1" "cmd2" ... "cmdN" outfile
```

Equivalent to:

```bash
< infile cmd1 | cmd2 | ... | cmdN > outfile
```

- `here_doc` mode:

```bash
./pipex_bonus here_doc LIMITER "cmd1" ... "cmdN" outfile
```

Equivalent to:

```bash
<< LIMITER cmd1 | ... | cmdN >> outfile
```

- Appends to the output file in here_doc mode
- Robust error handling for all edge cases

## Build

To build the project, run:

```bash
make        # Builds the mandatory pipex program
make bonus  # Builds the bonus pipex_bonus program
```

This will generate the following executables:

- `pipex` for mandatory part
- `pipex_bonus` for the bonus part with multiple command and here_doc support

Clean up object files and executables with:

```bash
make clean      # Removes object files
make fclean     # Removes object files and executables
make re         # Cleans and rebuilds everything
```

## Usage

### Mandatory

```bash
./pipex infile "grep foo" "wc -l" outfile
# Equivalent to: < infile grep foo | wc -l > outfile
```

### Bonus

```bash
./pipex_bonus infile "sort" "grep a" "wc -l" outfile
# Equivalent to: < infile sort | grep a | wc -l > outfile

./pipex_bonus here_doc END "tr a-z A-Z" "cat" result.txt
# Equivalent to: << END tr a-z A-Z | cat >> result.txt
```

## Examples

```bash
# 2 commands (mandatory)
./pipex infile.txt "cat" "wc -l" outfile.txt

# Multiple commands (bonus)
./pipex_bonus infile.txt "grep foo" "sort" "uniq -c" outfile.txt

# here_doc (bonus)
./pipex_bonus here_doc LIMITER "cat" "wc -l" outfile.txt
```

## Testing

- The project includes extensive manual and automated test scripts.
- See the comments in `pipex.c` and `pipex_bonus.c` for ready-to-use bash test scripts.

## Project Structure

```
mandatory/
  inc/
    pipex.h
  srcs/
    ...

bonus/
  inc_bonus/
    pipex_bonus.h
  srcs_bonus/
    ...

libft/
Makefile
LICENSE
```

## License

This project is licensed under the MIT License. See `LICENSE` for details.

## Author

**Levent Akdogan**

---

**Note:**  
This project is part of the 42 school curriculum and is intended for educational purposes, focusing on process management, pipes, and file descriptor manipulation in C.
