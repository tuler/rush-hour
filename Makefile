NAME=rush
COMP=xz
RIVEMU_RUN=rivemu
RIVEMU_EXEC=rivemu -no-window -sdk -workspace -exec

SRC = $(wildcard src/*.c)

ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_RUN=riv-run
	RIVEMU_EXEC=
endif
ifeq ($(CROSS),y)
	CFLAGS+=-Ilibriv -Llibriv -lriv
	CFLAGS+=-Wall -Wextra
	CFLAGS+=-march=rv64g -Og -g -rdynamic -fno-omit-frame-pointer -fno-strict-overflow -fno-strict-aliasing -std=c++17
	CFLAGS+=-Wl,--build-id=none,--sort-common
	CC=riscv64-buildroot-linux-musl-gcc
	STRIP=touch
else
	CC=$(RIVEMU_EXEC) gcc
#	CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed)
	CFLAGS=-O2
#	CFLAGS+=-DNDEBUG
#	CFLAGS+=-march=rv64g
#	CFLAGS+=-ffast-math
#	CFLAGS+=-fallow-store-data-races
#	CFLAGS+=-fgcse-after-reload
#	CFLAGS+=-fpredictive-commoning
#	CFLAGS+=-fsplit-paths
#	CFLAGS+=-ftree-partial-pre
#	CFLAGS+=-fno-strict-overflow
#	CFLAGS+=-fno-strict-aliasing
#	CFLAGS+=-fno-stack-protector
#	CFLAGS+=-Wl,--build-id=none
#	CFLAGS+=-Wl,--sort-common
#	CFLAGS+=-Wl,-O1
#	CFLAGS+=-Wl,--gc-sections
#	CFLAGS+=-Wl,--as-needed
#	CFLAGS+=-Wl,--hash-style=gnu
#	CFLAGS+=-Wl,-z,now
	CFLAGS+=-lriv

	STRIP=$(RIVEMU_EXEC) riv-strip
endif

build: $(NAME).sqfs

run: $(NAME).sqfs
	$(RIVEMU_RUN) $<

dev-run: $(NAME).elf
	$(RIVEMU_RUN) -no-loading -bench -workspace -exec $(NAME).elf

clean:
	rm -rf *.sqfs *.elf

distclean: clean
	rm -rf libriv

$(NAME).sqfs: $(NAME).elf *.png *.rivcard *.txt info.json
	$(RIVEMU_EXEC) riv-mksqfs $^ $@ -comp $(COMP)

$(NAME).elf: $(SRC) libriv
	$(CC) $(SRC) -o $@ $(CFLAGS)
	$(STRIP) $@

libriv:
	mkdir -p libriv
	$(RIVEMU_EXEC) cp /usr/include/riv.h libriv/
	$(RIVEMU_EXEC) cp /usr/lib/libriv.so libriv/
	$(RIVEMU_EXEC) cp /lib/libc.musl-riscv64.so.1 libriv/
