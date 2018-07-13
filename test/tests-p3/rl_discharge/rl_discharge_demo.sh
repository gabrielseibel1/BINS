#!/usr/bin/env bash
./../../../cmake-build-debug/BINS rl_discharge.sp rl_discharge.dat
gnuplot -p rl_discharge.gnu