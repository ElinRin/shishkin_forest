#!/bin/bash
for f in $2/*
do
    echo "Processing $f ..."
    ./$1 $f
done
