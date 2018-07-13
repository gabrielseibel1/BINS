#!/usr/bin/env bash
./../../../cmake-build-debug/BINS rc_discharge.sp rc_discharge.dat
gnuplot -p rc_discharge.gnu