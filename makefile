ccflags = -fno-stack-protector -Wno-implicit-int -Wno-implicit-function-declaration -std=c99
ldflags = -nostdlib -nostartfiles

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
	cc $^ -o $@ $(ldflags)

clean:
	rm -r obj
