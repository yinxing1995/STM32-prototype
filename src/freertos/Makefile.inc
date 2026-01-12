SRCS_C = \
    croutine.c \
    event_groups.c \
    list.c \
    port.c \
    queue.c \
    stream_buffer.c \
    tasks.c \
    timers.c \
    heap_4.c \
    manual.c

SUBSYSTEM = RTOS

$(info ***Make for $(SUBSYSTEM)***)


include ../../Makefile.rule