#ifndef FTP_SERVER_H
#define FTP_SERVER_H
#include "core.h"
int ftp_serve(s32 srv_fd, s32 data_listen_fd,
              void *G, void *D, void *load_mod, void *mmap,
              void *kopen, void *kwrite, void *kclose, void *kmkdir,
              void *getdents, void *usleep,
              void *recvfrom, void *sendto, void *accept,
              void *getsockname,
              s32 log_fd, u8 *log_sa, s32 userId,
              u8 **wad_data, u64 *wad_size);
#endif
