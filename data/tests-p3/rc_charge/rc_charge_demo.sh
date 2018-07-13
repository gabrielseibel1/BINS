#!/usr/bin/env bash
./../../../cmake-build-debug/BINS rc_charge.sp rc_charge.dat
gnuplot -p rc_charge.gnu