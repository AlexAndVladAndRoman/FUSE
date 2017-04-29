#ifndef _BBFS_H_
#define _BBFS_H_

#include "params.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"
#include "state.h"

void bb_fullpath(char fpath[PATH_MAX], const char *path);

int bb_getattr(const char *path, struct stat *statbuf);
int bb_readlink(const char *path, char *link, size_t size);
int bb_mknod(const char *path, mode_t mode, dev_t dev);
int bb_mkdir(const char *path, mode_t mode);
int bb_unlink(const char *path);
int bb_rmdir(const char *path);
int bb_symlink(const char *path, const char *link);
int bb_rename(const char *path, const char *newpath);
int bb_link(const char *path, const char *newpath);
int bb_chmod(const char *path, mode_t mode);
int bb_chown(const char *path, uid_t uid, gid_t gid);
int bb_truncate(const char *path, off_t newsize);
int bb_utime(const char *path, struct utimbuf *ubuf);
int bb_open(const char *path, struct fuse_file_info *fi);
int bb_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int bb_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int bb_statfs(const char *path, struct statvfs *statv);
int bb_flush(const char *path, struct fuse_file_info *fi);
int bb_release(const char *path, struct fuse_file_info *fi);
int bb_fsync(const char *path, int datasync, struct fuse_file_info *fi);
int bb_opendir(const char *path, struct fuse_file_info *fi);
int bb_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
int bb_releasedir(const char *path, struct fuse_file_info *fi);
int bb_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi);
void *bb_init(struct fuse_conn_info *conn);
void bb_destroy(void *userdata);
int bb_access(const char *path, int mask);
int bb_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi);
int bb_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi);
void bb_usage();

#endif
