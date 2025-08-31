SRCS_S = \
	stm32f103c8_start.s

SRCS_C = \
	cold_boot.c

SUBSYSTEM = crt0

$(info ***Make for $(SUBSYSTEM)***)


include ../../Makefile.rule