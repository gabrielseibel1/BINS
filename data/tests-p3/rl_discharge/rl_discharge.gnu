set term wxt title 'RL Discharge'
set ylabel "Voltage (V)"
set xlabel "Time (s)"
plot 'rl_discharge.dat' using 1:2 title 'V(Ind)' lc rgb 'red'