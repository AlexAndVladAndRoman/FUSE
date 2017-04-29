#include "bbfs.h"

struct fuse_operations bb_oper;
void init_operations() {
    bb_oper.getattr = bb_getattr;
    bb_oper.readlink = bb_readlink;
    bb_oper.getdir = nullptr; // ???
    bb_oper.mknod = bb_mknod;
    bb_oper.mkdir = bb_mkdir;
    bb_oper.unlink = bb_unlink;
    bb_oper.rmdir = bb_rmdir;
    bb_oper.symlink = bb_symlink;
    bb_oper.rename = bb_rename;
    bb_oper.link = bb_link;
    bb_oper.chmod = bb_chmod;
    bb_oper.chown = bb_chown;
    bb_oper.truncate = bb_truncate;
    bb_oper.utime = bb_utime;
    bb_oper.open = bb_open;
    bb_oper.read = bb_read;
    bb_oper.write = bb_write;
    bb_oper.statfs = bb_statfs;
    bb_oper.flush = bb_flush;
    bb_oper.release = bb_release;
    bb_oper.fsync = bb_fsync;
    bb_oper.opendir = bb_opendir;
    bb_oper.readdir = bb_readdir;
    bb_oper.releasedir = bb_releasedir;
    bb_oper.fsyncdir = bb_fsyncdir;
    bb_oper.init = bb_init;
    bb_oper.destroy = bb_destroy;
    bb_oper.access = bb_access;
    bb_oper.ftruncate = bb_ftruncate;
    bb_oper.fgetattr = bb_fgetattr;
}

int main(int argc, char *argv[]) {
    int fuse_stat;

    if ((getuid() == 0) || (geteuid() == 0)) {
        fprintf(stderr, "Running BBFS as root opens unnacceptable security holes\n");
        return 1;
    }
    if ((argc < 3) || (argv[argc - 2][0] == '-') || (argv[argc - 1][0] == '-')) bb_usage();

    state* data = new state(realpath(argv[argc - 2], nullptr), log_open());

    argv[argc - 2] = argv[argc - 1];
    argv[argc - 1] = nullptr;
    argc--;

    fprintf(stderr, "about to call fuse_main\n");
    init_operations();
    fuse_stat = fuse_main(argc, argv, &bb_oper, data);
    fprintf(stderr, "fuse_main returned %d\n", fuse_stat);

    return fuse_stat;
}
