#!/bin/bash

if (( $# == 1 ))
then
    make clean && make
fi

bin/main
