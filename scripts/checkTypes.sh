#!/bin/bash
make all && cat $1 | ./checkTypes
