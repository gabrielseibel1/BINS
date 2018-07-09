#!/usr/bin/env bash
#././../../bin/BINS-p3 rc_charge.sp rc_charge.dat
#././../../bin/BINS-p3 rc_discharge.sp rc_discharge.dat
gnuplot -p rc_charge.gnu
gnuplot -p rc_discharge.gnu