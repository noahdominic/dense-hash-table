#!/bin/sh

valgrind --leak-check=yes --track-origins=yes --log-file=valgrind.rpt build/out "$@"
cat valgrind.rpt

