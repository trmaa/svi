ccflags = -Iinclude -fno-stack-protector -Wno-implicit-int -Wno-implicit-function-declaration -std=c23

srcc = $(wildcard *.c)
srcs = $(wildcard *.s)

src = $(srcc) $(srcs)

obj = $(src:.c=.o)
obj := $(obj:.s=.o)

all: obj svi

obj:
	mkdir -p obj

obj/%.o: %.c
	cc -c $< -o $@ $(ccflags)

obj/%.o: %.s
	as $< -o $@

svi: $(addprefix obj/,$(obj)) | obj
	ld $^ -o $@

clean:
	rm -r obj
