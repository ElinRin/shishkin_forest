#!/bin/bash
./buildAll.sh && cat $1 | ./parser
