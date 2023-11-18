#!/bin/sh

valgrind --leak-check=yes --track-origins=yes --log-file=valgrind.rpt build/out 256
cat valgrind.rpt

