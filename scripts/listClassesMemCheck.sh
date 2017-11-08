#!/bin/bash
make all && cat $1 |   valgrind --leak-check=full -v ./testTableListClasses 
