#include "bbfs.h"

#include <iostream>
#include <string>

void bb_fullpath(char fpath[PATH_MAX], const char *path) {
    strcpy(fpath, state::get()->rootdir);
    strncat(fpath, path, PATH_MAX);
    log_msg("    bb_fullpath:  rootdir = \"%s\", path = \"%s\", fpath = \"%s\"\n", state::get()->rootdir, path, fpath);
}

int bb_getattr(const char *path, struct stat *statbuf) {
    int retstat;
    char fpath[PATH_MAX];

    log_msg("\nbb_getattr(path=\"%s\", statbuf=0x%08x)\n", path, statbuf);
    bb_fullpath(fpath, path);
    retstat = log_syscall("lstat", lstat(fpath, statbuf), 0);
    log_stat(statbuf);

    //SOME SHIT
    if (std::string(path) == "\\") {
        statbuf->st_nlink++;
    }

    return retstat;
}

int bb_readlink(const char *path, char *link, size_t size) {
    int retstat;
    char fpath[PATH_MAX];

    log_msg("bb_readlink(path=\"%s\", link=\"%s\", size=%d)\n", path, link, size);
    bb_fullpath(fpath, path);

    retstat = log_syscall("fpath", readlink(fpath, link, size - 1), 0);
    if (retstat >= 0) {
        link[retstat] = '\0';
        retstat = 0;
    }

    return retstat;
}

int bb_mknod(const char *path, mode_t mode, dev_t dev) {
    int retstat;
    char fpath[PATH_MAX];

    log_msg("\nbb_mknod(path=\"%s\", mode=0%3o, dev=%lld)\n", path, mode, dev);
    bb_fullpath(fpath, path);

    if (S_ISREG(mode)) {
        retstat = log_syscall("open", open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode), 0);
        if (retstat >= 0) retstat = log_syscall("close", close(retstat), 0);
    } else if (S_ISFIFO(mode))
        retstat = log_syscall("mkfifo", mkfifo(fpath, mode), 0);
    else
        retstat = log_syscall("mknod", mknod(fpath, mode, dev), 0);

    return retstat;
}

int bb_mkdir(const char *path, mode_t mode) {
    char fpath[PATH_MAX];

    log_msg("\nbb_mkdir(path=\"%s\", mode=0%3o)\n", path, mode);
    bb_fullpath(fpath, path);

    return log_syscall("mkdir", mkdir(fpath, mode), 0);
}

int bb_unlink(const char *path) {
    char fpath[PATH_MAX];

    log_msg("bb_unlink(path=\"%s\")\n", path);
    bb_fullpath(fpath, path);

    return log_syscall("unlink", unlink(fpath), 0);
}

int bb_rmdir(const char *path) {
    char fpath[PATH_MAX];

    log_msg("bb_rmdir(path=\"%s\")\n", path);
    bb_fullpath(fpath, path);

    return log_syscall("rmdir", rmdir(fpath), 0);
}

int bb_symlink(const char *path, const char *link) {
    char flink[PATH_MAX];

    log_msg("\nbb_symlink(path=\"%s\", link=\"%s\")\n", path, link);
    bb_fullpath(flink, link);

    return log_syscall("symlink", symlink(path, flink), 0);
}

int bb_rename(const char *path, const char *newpath) {
    char fpath[PATH_MAX];
    char fnewpath[PATH_MAX];

    log_msg("\nbb_rename(fpath=\"%s\", newpath=\"%s\")\n", path, newpath);
    bb_fullpath(fpath, path);
    bb_fullpath(fnewpath, newpath);

    return log_syscall("rename", rename(fpath, fnewpath), 0);
}

int bb_link(const char *path, const char *newpath) {
    char fpath[PATH_MAX], fnewpath[PATH_MAX];

    log_msg("\nbb_link(path=\"%s\", newpath=\"%s\")\n", path, newpath);
    bb_fullpath(fpath, path);
    bb_fullpath(fnewpath, newpath);

    return log_syscall("link", link(fpath, fnewpath), 0);
}

int bb_chmod(const char *path, mode_t mode) {
    char fpath[PATH_MAX];

    log_msg("\nbb_chmod(fpath=\"%s\", mode=0%03o)\n", path, mode);
    bb_fullpath(fpath, path);

    return log_syscall("chmod", chmod(fpath, mode), 0);
}

int bb_chown(const char *path, uid_t uid, gid_t gid) {
    char fpath[PATH_MAX];

    log_msg("\nbb_chown(path=\"%s\", uid=%d, gid=%d)\n", path, uid, gid);
    bb_fullpath(fpath, path);

    return log_syscall("chown", chown(fpath, uid, gid), 0);
}

int bb_truncate(const char *path, off_t newsize) {
    char fpath[PATH_MAX];

    log_msg("\nbb_truncate(path=\"%s\", newsize=%lld)\n", path, newsize);
    bb_fullpath(fpath, path);

    return log_syscall("truncate", truncate(fpath, newsize), 0);
}

int bb_utime(const char *path, struct utimbuf *ubuf) {
    char fpath[PATH_MAX];

    log_msg("\nbb_utime(path=\"%s\", ubuf=0x%08x)\n", path, ubuf);
    bb_fullpath(fpath, path);

    return log_syscall("utime", utime(fpath, ubuf), 0);
}

int bb_open(const char *path, struct fuse_file_info *fi) {
    int retstat = 0;
    int fd;
    char fpath[PATH_MAX];

    log_msg("\nbb_open(path\"%s\", fi=0x%08x)\n", path, fi);
    bb_fullpath(fpath, path);

    fd = log_syscall("open", open(fpath, fi->flags), 0);
    if (fd < 0) retstat = log_error("open");

    fi->fh = fd;
    log_fi(fi);

    return retstat;
}

int bb_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    /* int retstat = 0; */

    log_msg("\nbb_read(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n", path, buf, size, offset, fi);
    log_fi(fi);

    return log_syscall("pread", pread(fi->fh, buf, size, offset), 0);
}

int bb_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    /* int retstat = 0; */

    log_msg("\nbb_write(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n", path, buf, size, offset, fi);
    log_fi(fi);

    return log_syscall("pwrite", pwrite(fi->fh, buf, size, offset), 0);
}

int bb_statfs(const char *path, struct statvfs *statv) {
    int retstat = 0;
    char fpath[PATH_MAX];

    log_msg("\nbb_statfs(path=\"%s\", statv=0x%08x)\n", path, statv);
    bb_fullpath(fpath, path);

    retstat = log_syscall("statvfs", statvfs(fpath, statv), 0);

    log_statvfs(statv);

    return retstat;
}

int bb_flush(const char *path, struct fuse_file_info *fi) {
    log_msg("\nbb_flush(path=\"%s\", fi=0x%08x)\n", path, fi);
    log_fi(fi);
    return 0;
}

int bb_release(const char *path, struct fuse_file_info *fi) {
    log_msg("\nbb_release(path=\"%s\", fi=0x%08x)\n", path, fi);
    log_fi(fi);
    return log_syscall("close", close(fi->fh), 0);
}

int bb_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    log_msg("\nbb_fsync(path=\"%s\", datasync=%d, fi=0x%08x)\n", path, datasync, fi);
    log_fi(fi);

#ifdef HAVE_FDATASYNC
    if (datasync)
        return log_syscall("fdatasync", fdatasync(fi->fh), 0);
    else
#endif
        return log_syscall("fsync", fsync(fi->fh), 0);
}

int bb_opendir(const char *path, struct fuse_file_info *fi) {
    DIR *dp;
    int retstat = 0;
    char fpath[PATH_MAX];

    log_msg("\nbb_opendir(path=\"%s\", fi=0x%08x)\n", path, fi);
    bb_fullpath(fpath, path);

    dp = opendir(fpath);
    log_msg("    opendir returned 0x%p\n", dp);
    if (dp == NULL) retstat = log_error("bb_opendir opendir");

    fi->fh = (intptr_t)dp;

    log_fi(fi);

    return retstat;
}

int bb_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    int retstat = 0;
    DIR *dp;
    struct dirent *de;

    log_msg("\nbb_readdir(path=\"%s\", buf=0x%08x, filler=0x%08x, offset=%lld, fi=0x%08x)\n", path, buf, filler, offset, fi);
    dp = (DIR *)(uintptr_t)fi->fh;

    de = readdir(dp);
    log_msg("    readdir returned 0x%p\n", de);
    if (de == 0) {
        retstat = log_error("bb_readdir readdir");
        return retstat;
    }
    do {
        log_msg("calling filler with name %s\n", de->d_name);
        if (filler(buf, de->d_name, NULL, 0) != 0) {
            log_msg("    ERROR bb_readdir filler:  buffer full");
            return -ENOMEM;
        }
    } while ((de = readdir(dp)) != NULL);

    //SOME SHIT
    filler(buf, "shirvik_pidr", nullptr, 0);

    log_fi(fi);

    return retstat;
}

int bb_releasedir(const char *path, struct fuse_file_info *fi) {
    int retstat = 0;

    log_msg("\nbb_releasedir(path=\"%s\", fi=0x%08x)\n", path, fi);
    log_fi(fi);

    closedir((DIR *)(uintptr_t)fi->fh);

    return retstat;
}

int bb_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi) {
    int retstat = 0;

    log_msg("\nbb_fsyncdir(path=\"%s\", datasync=%d, fi=0x%08x)\n", path, datasync, fi);
    log_fi(fi);

    return retstat;
}

void *bb_init(struct fuse_conn_info *conn) {
    log_msg("\nbb_init()\n");

    log_conn(conn);
    log_fuse_context(fuse_get_context());

    return state::get();
}

void bb_destroy(void *userdata) {
    log_msg("\nbb_destroy(userdata=0x%08x)\n", userdata);
}

int bb_access(const char *path, int mask) {
    int retstat = 0;
    char fpath[PATH_MAX];

    log_msg("\nbb_access(path=\"%s\", mask=0%o)\n", path, mask);
    bb_fullpath(fpath, path);

    retstat = access(fpath, mask);

    if (retstat < 0) retstat = log_error("bb_access access");

    return retstat;
}

int bb_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi) {
    int retstat = 0;

    log_msg("\nbb_ftruncate(path=\"%s\", offset=%lld, fi=0x%08x)\n", path, offset, fi);
    log_fi(fi);

    retstat = ftruncate(fi->fh, offset);
    if (retstat < 0) retstat = log_error("bb_ftruncate ftruncate");

    return retstat;
}

int bb_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi) {
    int retstat = 0;

    log_msg("\nbb_fgetattr(path=\"%s\", statbuf=0x%08x, fi=0x%08x)\n", path, statbuf, fi);
    log_fi(fi);

    retstat = fstat(fi->fh, statbuf);
    if (retstat < 0) retstat = log_error("bb_fgetattr fstat");

    log_stat(statbuf);

    return retstat;
}

void bb_usage() {
    fprintf(stderr, "usage:  bbfs [FUSE and mount options] rootDir mountPoint\n");
    abort();
}
