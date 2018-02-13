mkfifo fifo.cu8
rtl_sdr -g 30 -f 434248000 -s 1300000 fifo.cu8 &
../rtlomniv2 -i fifo.cu8 -l 43262 -t 560069

