#!/bin/sh

valgrind --leak-check=yes --log-file=valgrind.rpt build/out

