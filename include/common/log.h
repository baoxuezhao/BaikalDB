// Copyright (c) 2018 Baidu, Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef BAIDU_INTERNAL
#include <com_log.h>
#else
#include <stdarg.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#endif

namespace baikaldb {
#ifdef BAIDU_INTERNAL
#ifndef NDEBUG
#define DB_DEBUG(_fmt_, args...) \
    do {\
        com_writelog("DEBUG", "[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);
#else
#define DB_DEBUG(_fmt_, args...) 
#endif

#define DB_TRACE(_fmt_, args...) \
    do {\
        com_writelog("TRACE", "[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define DB_NOTICE(_fmt_, args...) \
    do {\
        com_writelog("NOTICE", "[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define DB_WARNING(_fmt_, args...) \
    do {\
        com_writelog("WARNING", "[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define DB_FATAL(_fmt_, args...) \
    do {\
        com_writelog("FATAL", "[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define SELF_TRACE(_fmt_, args...) \
    do {\
        com_writelog("MY_TRACE", "[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);
#else
const int MAX_LOG_LEN = 2048;
inline void glog_info_writelog(const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    LOG(INFO) << buf;
}
inline void glog_warning_writelog(const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    LOG(WARNING) << buf;
}
inline void glog_error_writelog(const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    LOG(ERROR) << buf;
}
#ifndef NDEBUG
#define DB_DEBUG(_fmt_, args...) \
    do {\
        ::baikaldb::glog_info_writelog("[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0); 
#else
#define DB_DEBUG(_fmt_, args...) 
#endif

#define DB_TRACE(_fmt_, args...) \
    do {\
        ::baikaldb::glog_info_writelog("[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define DB_NOTICE(_fmt_, args...) \
    do {\
        ::baikaldb::glog_info_writelog("[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define DB_WARNING(_fmt_, args...) \
    do {\
        ::baikaldb::glog_warning_writelog("[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define DB_FATAL(_fmt_, args...) \
    do {\
        ::baikaldb::glog_error_writelog("[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);

#define SELF_TRACE(_fmt_, args...) \
    do {\
        ::baikaldb::glog_info_writelog("[%s:%d][%s][%llu]" _fmt_, \
                strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__, bthread_self(), ##args);\
    } while (0);
#endif

#define DB_DEBUG_CLIENT(sock, _fmt_, args...) \
    do {\
        DB_DEBUG("user=%s fd=%d ip=%s port=%d errno=%d:" _fmt_, \
            sock->username.c_str(), sock->fd, sock->ip.c_str(), sock->port, \
            sock->query_ctx->stat_info.error_code, ##args);\
    } while (0);

#define DB_TRACE_CLIENT(sock, _fmt_, args...) \
    do {\
        DB_TRACE("user=%s fd=%d ip=%s port=%d errno=%d:" _fmt_, \
            sock->username.c_str(), sock->fd, sock->ip.c_str(), sock->port, \
            sock->query_ctx->stat_info.error_code, ##args);\
    } while (0);

#define DB_NOTICE_CLIENT(sock, _fmt_, args...) \
    do {\
        DB_NOTICE("user=%s fd=%d ip=%s port=%d errno=%d:" _fmt_, \
            sock->username.c_str(), sock->fd, sock->ip.c_str(), sock->port, \
            sock->query_ctx->stat_info.error_code, ##args);\
    } while (0);

#define DB_WARNING_CLIENT(sock, _fmt_, args...) \
    do {\
        DB_WARNING("user=%s fd=%d ip=%s port=%d errno=%d:" _fmt_, \
            sock->username.c_str(), sock->fd, sock->ip.c_str(), sock->port, \
            sock->query_ctx->stat_info.error_code, ##args);\
    } while (0);

#define DB_FATAL_CLIENT(sock, _fmt_, args...) \
    do {\
        DB_FATAL("user=%s fd=%d ip=%s port=%d errno=%d:" _fmt_, \
            sock->username.c_str(), sock->fd, sock->ip.c_str(), sock->port, \
            sock->query_ctx->stat_info.error_code, ##args);\
    } while (0);

inline int init_log(const char* bin_name) {
#ifdef BAIDU_INTERNAL
    const char* CONFIG_LOG_PATH = "./conf";
    const char* CONFIG_LOG_NAME = "comlog.conf";
    return com_loadlog(CONFIG_LOG_PATH, CONFIG_LOG_NAME);
#else
    ::google::InitGoogleLogging(bin_name);
    FLAGS_max_log_size = MAX_LOG_LEN;
    FLAGS_stop_logging_if_full_disk = true;
    FLAGS_logbufsecs = 0;
    FLAGS_logtostderr = false;
    FLAGS_alsologtostderr = false;
    FLAGS_log_dir = "";
    ::google::SetLogDestination(google::GLOG_INFO, "log/task_info_log.");
    ::google::SetLogDestination(google::GLOG_WARNING, "log/task_warning_log.");
    return 0;
#endif
}

} //namespace baikaldb

/* vim: set ts=4 sw=4 sts=4 tw=100 */
