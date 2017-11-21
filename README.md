# shishkin_forest 
**shishkin_forest** - a compiler for MiniJava language

### Requirements:
- gcc
- flex++
- bison
- GraphViz(dot)

### Usage:
```
./scripts/parseFile.sh <File> #Generate a syntax tree and save as svg-image
./scripts/listClasses.sh <File> #Display all classes, methods and their variables
./scripts/checkTypes.sh <File> #Run checks for syntax, declaration and type errors
```
---
Developed by [ElinRin](https://github.com/ElinRin) and [IKholopov](https://github.com/IKholopov) as part of ABBYY course in MIPT on compilers development, 2017-2018.
