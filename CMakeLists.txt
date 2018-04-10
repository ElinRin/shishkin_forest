cmake_minimum_required(VERSION 3.8)
project(shishkin_forest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g")

include_directories(
        .
        parser_src/bison
        SymbolTable
        SyntaxTree
        TypeChecker
        ActivationRecords
        TestActivationRecords
        IR IR/Expressions IR/Statements IR/Translate
        X86
        CodeGeneration
)

# Build library bison_spec

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_src)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_src/bison)

set(BisonFile ${CMAKE_SOURCE_DIR}/spec.yy)
set(BisonOutput ${CMAKE_SOURCE_DIR}/parser_src/bison/parser.cpp)
set(BisonTokens ${CMAKE_SOURCE_DIR}/parser_src/bison/tokens.h)

find_package(BISON 3.)
if (BISON_PATH)
    message("Use custom bison.")
    set(BISON_EXECUTABLE ${BISON_PATH}/bin/bison)
    set(BISON_FOUND TRUE)
endif ()

if (NOT BISON_FOUND)
    message(FATAL_ERROR "Bison not found.")
endif ()

if (BISON_FOUND)
    add_custom_command(
            OUTPUT ${BisonOutput} ${BisonTokens}
            COMMAND ${BISON_EXECUTABLE}
            --defines=${BisonTokens}
            --output=${BisonOutput}
            ${BisonFile}
            MAIN_DEPENDENCY ${BisonFile}
            COMMENT "Generating parser.cpp and tokens.h from ${BisonFile}"

    )
endif ()

# Build library lexer

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_src/flex)

set(FlexFile ${CMAKE_SOURCE_DIR}/lexer.lex)
set(FlexOutput ${CMAKE_SOURCE_DIR}/parser_src/flex/lex.yy.cc)

find_package(FLEX 2.6)
if (FLEX_PATH)
    message("Use custom flex.")
    set(FLEX_EXECUTABLE ${FLEX_PATH}/bin/flex)
    set(FLEX_LIBRARIES ${FLEX_PATH}/lib/libfl.a)
    set(FLEX_FOUND TRUE)
endif ()

if (NOT FLEX_FOUND)
    message(FATAL_ERROR "Flex not found.")
endif ()

if (FLEX_FOUND)
    add_custom_command(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            --c++
            --outfile=${FlexOutput}
            ${FlexFile}
            MAIN_DEPENDENCY ${FlexFile} ${BisonTokens}
            COMMENT "Generating scanner.cpp from ${FlexFile}"
    )
endif ()

add_library(bison_spec STATIC ${BisonOutput} ${FlexOutput})

if (FLEX_PATH)
    target_include_directories(bison_spec PRIVATE ${FLEX_PATH}/include)
endif ()

# build library SymbolTable
file(GLOB SymbolTable_SRC SymbolTable/*.cpp)
add_library(symbol_table STATIC ${SymbolTable_SRC})

# build library SyntaxTree
file(GLOB SyntaxTree_SRC SyntaxTree/*.cpp)
add_library(syntax_tree STATIC ${SyntaxTree_SRC})

# build library ActivationRecords
file(GLOB ActivationRecords_SRC ActivationRecords/*.cpp)
add_library(activation_records STATIC ${ActivationRecords_SRC})

# build library IR
file(GLOB IR_SRC IR/*.cpp IR/Translate/*.cpp IR/Statements/*.cpp IR/Expressions/*.cpp)
add_library(ir STATIC ${IR_SRC})

# build library PrettyPrint
file(GLOB PrettyPrint_SRC PrettyPrint/*.cpp)
add_library(pretty_print STATIC ${PrettyPrint_SRC})

# build library TestSymbolTable
file(GLOB TestSymbolTable_SRC TestSymbolTable/*.cpp)
add_library(test_symbol_table STATIC ${TestSymbolTable_SRC})

# build library TypeChecker
file(GLOB TypeChecker_SRC TypeChecker/*.cpp)
add_library(type_checker STATIC ${TypeChecker_SRC})

# build library X86
file(GLOB X86_SRC X86/*.cpp)
add_library(x86 STATIC ${X86_SRC})
target_link_libraries(x86 ir activation_records)

# build graph
add_executable(graph graph.cpp)
target_link_libraries(
        graph
        bison_spec
        syntax_tree pretty_print
        x86
        ${FLEX_LIBRARIES}
)

# build checkTypes
add_executable(checkTypes CheckTypes.cpp)
target_link_libraries(
        checkTypes
        bison_spec
        syntax_tree
        type_checker
        symbol_table
        x86
        ${FLEX_LIBRARIES}
)

#build testTableListClasses
add_executable(testTableListClasses tableTest.cpp)
target_link_libraries(
        testTableListClasses
        bison_spec
        syntax_tree
        test_symbol_table symbol_table
        x86
        ${FLEX_LIBRARIES}
)

#build listActivations
add_executable(listActivations ListActivations.cpp)
target_link_libraries(
        listActivations
        bison_spec
        syntax_tree
        symbol_table
        type_checker
        x86
        ${FLEX_LIBRARIES}
)

#build buildIR
add_executable(buildIR BuildIR.cpp)
target_link_libraries(
        buildIR
        bison_spec
        syntax_tree
        test_symbol_table symbol_table
        type_checker
        x86
        ${FLEX_LIBRARIES}
)

set(scripts_path "./scripts/")

file(GLOB Scripts
    "${scripts_path}*.sh"
)

foreach(script ${Scripts})
    get_filename_component(script_name ${script} NAME)
    configure_file(${script} ${CMAKE_CURRENT_BINARY_DIR}/${scripts_path}${script_name})
endforeach(script)
