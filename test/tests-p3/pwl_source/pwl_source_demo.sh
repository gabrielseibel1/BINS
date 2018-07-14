#!/usr/bin/env bash
./../../../cmake-build-debug/BINS pwl_source.sp pwl_source.dat
gnuplot -p pwl_source.gnu