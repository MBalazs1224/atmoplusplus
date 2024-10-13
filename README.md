
# Atmosphere Compiler

**Atmosphere** is a programming language designed as part of my final thesis. It combines the simplicity of Python with the readability of SQL. The main part of **Atmosphere** is to let the developers write code that reads like natural English, making the programming process more intuitive. The project will translate into *64 bit x86 assembly*.

## Features

- **English-like Syntax:** Clear and easy to read syntax.
- **Object-Oriented:** Supports object-oriented programming principles.

## Project Status
This project is under active development, currently the work is focused on the *semantic analyzer*.
### Completed Features:
- Variable, function and class definition
- Function calls (correct parameters, return type)
- Identifier management
- Basic control structures (if statements, loops etc.)
- Types
- Access control
- Constructor definition and calls
### OOP Support
- Inheritance and encapsulation are implemented but need more testing
- Polymorphism is currently work in progress (Parent and child class relations are more or less supported)

### Known bugs
- Underlines the wrong part of the source code, if the code is nested in multiple blocks
- Currently does not check for constructor chaining
- There are some shitf/reduce conflicts in the parser, that will be resolved later

## Getting Started

### Requirements

- A compatible C++ compiler
- Flex for lexical analysis
- Bison for syntactic analysis
- Make for building the project
- Google Test for testing

### Installation

1. Clone the repository:
   `git clone https://github.com/MBalazs1224/atmoplusplus.git`
   
2. Navigate to the project directory:
 `cd atmoplusplus`
 3. Build the project:
 `make`
 4. Run the compiler (the input file defaults to test.txt for now, which contains example code):
 `./main`

## Testing
Tests are currently focused on the lexer using the **Google Test** framework.
#### Building the tests:
1. Install **Google Test** framework
2. Navigate to the project directory:
 `cd atmoplusplus`
3. Build the tests:
`make tests`
4. Run the tests:
`./runTests`