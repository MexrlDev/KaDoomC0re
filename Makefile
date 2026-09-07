CC = x86_64-elf-gcc
LD = x86_64-elf-ld
OBJCOPY = x86_64-elf-objcopy
CFLAGS = -nostdlib -fno-stack-protector -fPIC -O2 -I. -Wall -Ithird_party/chocolate-doom/src -Ithird_party/chocolate-doom -I/usr/include/SDL2 -DHAVE_CONFIG_H
LDFLAGS = -T linker.ld

# Our source files (in src/)
SRC_FILES = src/hijack.c src/audio.c src/input.c src/ui.c src/ftp_server.c src/i_ps.c src/doom_engine.c src/doom_main.c src/stub_libc.c
OBJS = $(SRC_FILES:src/%.c=%.o)

# Chocolate Doom sources – keep only the core game logic
DOOM_SRC_DIR = third_party/chocolate-doom/src
DOOM_SRC_FILES = $(wildcard $(DOOM_SRC_DIR)/*.c)

# Exclude everything that uses stdio, file I/O, networking, config, DeHackEd, etc.
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/d_dedicated.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/d_loop.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/d_iwad.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/deh_%.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/m_config.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/m_controls.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/m_argv.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/m_misc.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/i_glob.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/memio.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/mus2mid.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/midi*.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/w_main.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/w_merge.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/w_checksum.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/z_zone.c, $(DOOM_SRC_FILES))

# Also exclude the standard SDL-based sound/video/input (we provide our own)
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/i_sound.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/i_video.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/i_input.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/i_system.c, $(DOOM_SRC_FILES))
DOOM_SRC_FILES := $(filter-out $(DOOM_SRC_DIR)/i_main.c, $(DOOM_SRC_FILES))

# Keep only essential engine files: p_*, r_*, g_*, hu_*, st_*, am_*, f_*, s_*, etc.
DOOM_SRC_FILES := $(filter $(DOOM_SRC_DIR)/p_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/r_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/g_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/hu_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/st_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/am_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/f_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/s_%.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/m_fixed.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/m_bbox.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/m_cheat.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/tables.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/sha1.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/z_native.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/w_wad.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/v_video.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/d_event.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/d_mode.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/aes_prng.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/d_main.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/g_game.c, $(DOOM_SRC_FILES)) \
                  $(filter $(DOOM_SRC_DIR)/p_setup.c, $(DOOM_SRC_FILES))

DOOM_OBJS = $(DOOM_SRC_FILES:$(DOOM_SRC_DIR)/%.c=doom_%.o)

all: doom.bin

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

doom_%.o: $(DOOM_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

doom.elf: $(OBJS) $(DOOM_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

doom.bin: doom.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o doom.elf doom.bin

.PHONY: all clean
