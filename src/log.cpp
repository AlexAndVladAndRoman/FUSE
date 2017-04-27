#include "params.h"

#include <errno.h>
#include <fuse.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "log.h"

FILE *log_open() {
    FILE *logfile;
    logfile = fopen("bbfs.log", "w");
    if (logfile == NULL) {
        perror("logfile");
        exit(EXIT_FAILURE);
    }
    setvbuf(logfile, NULL, _IOLBF, 0);
    return logfile;
}

void log_msg(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(BB_DATA->logfile, format, ap);
}

int log_error(char const *func) {
    int ret = -errno;
    log_msg("    ERROR %s: %s\n", func, strerror(errno));
    return ret;
}

void log_fuse_context(struct fuse_context *context) {
    log_msg("    context:\n");
    log_struct(context, fuse, % 08x, );
    log_struct(context, uid, % d, );
    log_struct(context, gid, % d, );
    log_struct(context, pid, % d, );
    log_struct(context, private_data, % 08x, );
    log_struct(((struct bb_state *)context->private_data), logfile, % 08x, );
    log_struct(((struct bb_state *)context->private_data), rootdir, % s, );
    log_struct(context, umask, % 05o, );
}

void log_conn(struct fuse_conn_info *conn) {
    log_msg("    conn:\n");
    log_struct(conn, proto_major, % d, );
    log_struct(conn, proto_minor, % d, );
    log_struct(conn, async_read, % d, );
    log_struct(conn, max_write, % d, );
    log_struct(conn, max_readahead, % d, );
    log_struct(conn, capable, % 08x, );
    log_struct(conn, want, % 08x, );
    log_struct(conn, max_background, % d, );
    log_struct(conn, congestion_threshold, % d, );
}

void log_fi(struct fuse_file_info *fi) {
    log_msg("    fi:\n");
    log_struct(fi, flags, 0x % 08x, );
    log_struct(fi, fh_old, 0x % 08lx, );
    log_struct(fi, writepage, % d, );
    log_struct(fi, direct_io, % d, );
    log_struct(fi, keep_cache, % d, );
    log_struct(fi, fh, 0x % 016llx, );
    log_struct(fi, lock_owner, 0x % 016llx, );
}

void log_retstat(char const *func, int retstat) {
    int errsave = errno;
    log_msg("    %s returned %d\n", func, retstat);
    errno = errsave;
}

int log_syscall(char const *func, int retstat, int min_ret) {
    log_retstat(func, retstat);

    if (retstat < min_ret) {
        log_error(func);
        retstat = -errno;
    }

    return retstat;
}

void log_stat(struct stat *si) {
    log_msg("    si:\n");
    log_struct(si, st_dev, % lld, );
    log_struct(si, st_ino, % lld, );
    log_struct(si, st_mode, 0 % o, );
    log_struct(si, st_nlink, % d, );
    log_struct(si, st_uid, % d, );
    log_struct(si, st_gid, % d, );
    log_struct(si, st_rdev, % lld, );
    log_struct(si, st_size, % lld, );
    log_struct(si, st_blksize, % ld, );
    log_struct(si, st_blocks, % lld, );
    log_struct(si, st_atime, 0x % 08lx, );
    log_struct(si, st_mtime, 0x % 08lx, );
    log_struct(si, st_ctime, 0x % 08lx, );
}

void log_statvfs(struct statvfs *sv) {
    log_msg("    sv:\n");
    log_struct(sv, f_bsize, % ld, );
    log_struct(sv, f_frsize, % ld, );
    log_struct(sv, f_blocks, % lld, );
    log_struct(sv, f_bfree, % lld, );
    log_struct(sv, f_bavail, % lld, );
    log_struct(sv, f_files, % lld, );
    log_struct(sv, f_ffree, % lld, );
    log_struct(sv, f_favail, % lld, );
    log_struct(sv, f_fsid, % ld, );
    log_struct(sv, f_flag, 0x % 08lx, );
    log_struct(sv, f_namemax, % ld, );
}

void log_utime(struct utimbuf *buf) {
    log_msg("    buf:\n");
    log_struct(buf, actime, 0x % 08lx, );
    log_struct(buf, modtime, 0x % 08lx, );
}
