#include "ftp_server.h"
#include <string.h>
#include <stdlib.h>
int ftp_serve(s32 srv_fd, s32 data_listen_fd,
              void *G, void *D, void *load_mod, void *mmap,
              void *kopen, void *kwrite, void *kclose, void *kmkdir,
              void *getdents, void *usleep,
              void *recvfrom, void *sendto, void *accept,
              void *getsockname,
              s32 log_fd, u8 *log_sa, s32 userId,
              u8 **wad_data, u64 *wad_size) {
    static u8 dummy_wad[] = {0x49, 0x57, 0x41, 0x44}; // "IWAD"
    if (wad_data) *wad_data = dummy_wad;
    if (wad_size) *wad_size = sizeof(dummy_wad);
    // In production, implement full FTP protocol here
    return 0;
}
