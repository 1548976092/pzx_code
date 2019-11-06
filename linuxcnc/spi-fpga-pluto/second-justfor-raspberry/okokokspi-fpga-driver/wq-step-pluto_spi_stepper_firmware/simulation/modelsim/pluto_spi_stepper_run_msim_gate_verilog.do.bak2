transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vlog -vlog01compat -work work +incdir+. {pluto_spi_stepper.vo}

vlog -vlog01compat -work work +incdir+D:/pzx_linuxcnc/spi-fpga-driver-master/pluto_spi_stepper_firmware {D:/pzx_linuxcnc/spi-fpga-driver-master/pluto_spi_stepper_firmware/stepgen_tb.v}

vsim -t 1ps +transport_int_delays +transport_path_delays -L altera_ver -L altera_lnsim_ver -L fiftyfivenm_ver -L gate_work -L work -voptargs="+acc"  stepgen_tb

add wave *
view structure
view signals
run -all
