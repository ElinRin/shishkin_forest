#!/bin/bash
./scripts/buildAll.sh && cat $1 | ./parser
