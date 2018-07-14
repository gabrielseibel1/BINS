set term wxt title 'PWL Source and Resistor'
set multiplot                       # multiplot mode (prompt changes to 'multiplot')
set size 1, 0.5

set origin 0.0,0.5
set ylabel "Voltage (V)"
set xlabel "Time (s)"
plot 'pwl_source.dat' using 1:2 title 'V(Source)' lc rgb 'green'

set origin 0.0,0.0
set ylabel "Current (A)"
set xlabel "Time (s)"
plot 'pwl_source.dat' using 1:3 title 'I(Res)' lc rgb 'blue'

unset multiplot                     # exit multiplot mode (prompt changes back to 'gnuplot')