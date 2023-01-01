# Mortgage Calculator
[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)
Calculates and displays an amortization table for a home loan. The '-c' at the end of mortgage-c is to quickly identify this program as written in the C language.

## Table of Contents
- [Install](#install)
- [Usage](#usage)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Install
The mortgage calculator is built with GNU Make. It is compiled and installed in the same directory as the repository, so *install* is a bit of an exaggeration. To make the project, clone the repository and navigate to the project's root directory. There you can run the command `make all`.

To see the specific compile steps, view the Makefile using a command like `cat Makefile`. The most basic compiling command would something like this:
```
gcc -Wall -Wextra -pedantic -std=c99 console_util.c mortgage.c -o mortgage.exe
```

The first few options tell the GCC compiler to be as strict as possible with the warnings and to enforce the C99 version of the C language. These lines aren't necessarily required, but they help verify that the code is written reasonble well.

## Usage
To run the program, you must specify the mortgage amount, the interest rate (as an APR), and the loan term in months. If you omit one of these terms, the program will display an error message. For example:
```
./mortgage.exe 200000 4.25 360
```
Which would generate an amortization table for a $200k loan at 4.25% APR over 30 years.

## Maintainers

[@JoshuaTallman](https://github.com/prof-tallman).

## Contributing
This program is written as an example for students.

## License
[MIT](LICENSE) © [@JoshuaTallman](https://github.com/prof-tallman)
