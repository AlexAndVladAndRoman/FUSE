#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>

#define log_struct(st, field, format, typecast) \
log_msg("    " #field " = " #format "\n", typecast st->field)

FILE *log_open(void);
void log_msg(const char *format, ...);
void log_conn(struct fuse_conn_info *conn);
int log_error(char *func);
void log_fi(struct fuse_file_info *fi);
void log_fuse_context(struct fuse_context *context);
void log_retstat(char *func, int retstat);
void log_stat(struct stat *si);
void log_statvfs(struct statvfs *sv);
int  log_syscall(char *func, int retstat, int min_ret);
void log_utime(struct utimbuf *buf);

#endif
