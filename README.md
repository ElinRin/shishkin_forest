# shishkin_forest 
**shishkin_forest** - a compiler for MiniJava language  
#### AST
<img src="./tree.svg"/>  

#### IR  
<img src="./IR.svg"/>  

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
./scripts/buildIR.sh <File> #Run checks, creates IR-tree and save as svg-image
```
---
Developed by [ElinRin](https://github.com/ElinRin) and [IKholopov](https://github.com/IKholopov) as part of ABBYY course in MIPT on compiler development, 2017-2018.
