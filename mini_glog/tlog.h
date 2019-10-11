//
// Created by itachpei on 2019/8/1.
//

#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <iostream>
#include <chrono>
#include "utils/tlog_config.h"


// Recording start
#define TIMER_START(_X) auto _X##_start = std::chrono::steady_clock::now(), _X##_stop = _X##_start

// Recording end
#define TIMER_STOP(_X) _X##_stop = std::chrono::steady_clock::now()

// return duration time with nanosecond
#define TIMER_NSEC(_X)                                                                             \
    std::chrono::duration_cast<std::chrono::nanoseconds>(_X##_stop - _X##_start).count()

// return duration time with microsecond
#define TIMER_USEC(_X)                                                                             \
    std::chrono::duration_cast<std::chrono::microseconds>(_X##_stop - _X##_start).count()

// return duration time with millisecond
#define TIMER_MSEC(_X)                                                                             \
    (0.000001 *                                                                                    \
     std::chrono::duration_cast<std::chrono::nanoseconds>(_X##_stop - _X##_start).count())

// return duration time with seconds
#define TIMER_SEC(_X)                                                                              \
    (0.000001 *                                                                                    \
     std::chrono::duration_cast<std::chrono::microseconds>(_X##_stop - _X##_start).count())

// return duration time with minutes
#define TIMER_MIN(_X)                                                                              \
    std::chrono::duration_cast<std::chrono::minutes>(_X##_stop - _X##_start).count()


// default severity is error
#ifndef TCNN_LOG_SEVERITY
#define TCNN_LOG_SEVERITY 2
#endif  // TCNN_LOG_SEVERITY

// Definition of LOG() usage.
#define TLOG(severity) COMPACT_TLOG_##severity

#define TLOG_FUNC_CALL TLogger
#define TLOG_FUNC_CALL_FATAL TLoggerFatal


#ifdef TCNN_LOG_ENABLE

#if TCNN_LOG_SEVERITY == 0
#define COMPACT_TLOG_INFO TLOG_FUNC_CALL(__FILE__, __FUNCTION__, __LINE__, 0).stream()
#else
#define COMPACT_TLOG_INFO NullStream()
#endif  // TCNN_LOG_SEVERITY

#if TCNN_LOG_SEVERITY <= 1
#define COMPACT_TLOG_WARNING TLOG_FUNC_CALL(__FILE__, __FUNCTION__, __LINE__, 1).stream()
#else
#define COMPACT_TLOG_WARNING NullStream()
#endif  // TCNN_LOG_SEVERITY

#if TCNN_LOG_SEVERITY <= 2
#define COMPACT_TLOG_ERROR TLOG_FUNC_CALL(__FILE__, __FUNCTION__, __LINE__, 2).stream()
#else
#define COMPACT_TLOG_ERROR NullStream()
#endif  // TCNN_LOG_SEVERITY

#else   // TCNN_LOG_ENABLE

#define COMPACT_TLOG_INFO NullStream()
#define COMPACT_TLOG_WARNING NullStream()
#define COMPACT_TLOG_ERROR NullStream()

#endif  // TCNN_LOG_ENABLE

// fatal message always print.
#define COMPACT_TLOG_FATAL TLOG_FUNC_CALL_FATAL(__FILE__, __FUNCTION__, __LINE__, 3).stream()


class LogVoidify
{
public:
    LogVoidify() = default;

    void operator&(std::ostream &) {}
};


#define TLOG_IF(severity, condition) \
    static_cast<void>(0),             \
    !(condition) ? (void) 0 : LogVoidify() & TLOG(severity)

// Check assert, if the condition are not met then abort.
#define CHECK_ASSERT(condition)  TLOG_IF(FATAL, (!(condition))) << "Check failed: " #condition << std::endl

#define _CHECK_BINARY(x, cmp, y) CHECK_ASSERT(x cmp y)

#define CHECK_EQ(x, y) _CHECK_BINARY(x, ==, y)
#define CHECK_NE(x, y) _CHECK_BINARY(x, !=, y)
#define CHECK_LT(x, y) _CHECK_BINARY(x, <, y)
#define CHECK_LE(x, y) _CHECK_BINARY(x, <=, y)
#define CHECK_GT(x, y) _CHECK_BINARY(x, >, y)
#define CHECK_GE(x, y) _CHECK_BINARY(x, >=, y)


enum class TLOG_SEVERITY
{
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
    FATAL = 3,
    NUM_SEVERITY = 4
};


class TLogStreamBuf : public std::streambuf
{
public:
    TLogStreamBuf(char *buf, int len, std::ofstream *ofs, bool saveLogFile) : m_ofs(ofs), m_saveLogFile(saveLogFile)
    {
        // set front and tail pointer
        setp(buf, buf + len - 2);   // keep a char space for overflow().
    }

    size_t pcount() const
    {
        return pptr() - pbase();
    }

    char *pbase() const
    {
        return std::streambuf::pbase();
    }

    void clean()
    {
        this->pbump(-this->pcount());
    }

    int overflow(int_type c) override
    {
        if (c != EOF)
        {
            *pptr() = c;
            this->pbump(1);
        }

        if (flush_buffer() == EOF)
        {
            return EOF;
        }
        return c;
    }

    int sync() override
    {
        if (flush_buffer() == EOF)
        {
            return -1;
        }
        return 0;
    }

protected:

    // flush buffer content
    int flush_buffer();

private:
    std::ofstream *m_ofs;
    bool m_saveLogFile;
};


class TLogStream : public std::ostream
{
public:
    TLogStream(char *buf, int len, std::ofstream *ofs, bool saveLogFile) : m_streambuf(buf, len, ofs, saveLogFile)
    {
        // set stream buffer
        this->rdbuf(&m_streambuf);
    }

    TLogStream(const TLogStream &) = delete;

    TLogStream &operator=(const TLogStream &) = delete;

    // return the length of buffer content
    size_t pcount() const
    {
        return m_streambuf.pcount();
    }

    // return the buffer pointer
    char *pbase() const
    {
        return m_streambuf.pbase();
    }

    // return the buffer content
    char *str() const
    {
        return pbase();
    }

    // reset the rbase pointer
    void clean()
    {
        m_streambuf.clean();
    }

private:
    TLogStreamBuf m_streambuf;
};


class NullStream : public std::ostream
{
    class NullStreamBuffer : public std::streambuf
    {
    public:
        int overflow(int c) override
        {
            return c;
        }
    } m_buffer;

public:
    NullStream() : std::ostream(&m_buffer) {}
};


class TLogger
{
public:
    static TLogStream *logStream;

    // if saveLogFile and logFile is null, then generate log file by default.
    // if buffer is null, then allocate and manage memory in TLogger(buffer).
    static void InitTLogger(char *buf, uint32_t bufSize, TLOG_SEVERITY sev, const char *logFile = nullptr,
                            bool saveLogFile = false);

    static void ShutdownTLogger();

    TLogger(const char *file, const char *func, int lineno, int sev)
    {
        if (!TLogger::isInit)
        {
            TLogger::InitTLogger(nullptr, TLogger::defaultBufSize, TLOG_SEVERITY::INFO, nullptr, false);
        }
        TLogger::genMsgHeader(*logStream, file, func, lineno, sev);
    }

    TLogger(const TLogger &) = delete;

    void operator=(const TLogger &) = delete;

    ~TLogger()
    {
        TLogger::logStream->flush();
    }

    std::ostream &stream();

//    std::ostream &nullStream();

private:
    static TLOG_SEVERITY severity;
    static NullStream *nullLogStream;
    static const std::array<std::string, 4> LogSeverityNames;
    static const int MAX_FILENAME_LEN;
    static std::ofstream *ofs;
    static char *buffer;
    static bool isInit;
    static size_t defaultBufSize;

    bool validStream = false;

    void genMsgHeader(std::ostream &stream, const char *filename, const char *funcname, int lineno, int sev);
};


// TLogger for fatal information
class TLoggerFatal : public TLogger
{
public:
    TLoggerFatal(const char *file, const char *func, int lineno, int severity) : TLogger(file, func, lineno,
                                                                                         severity) {}

    ~TLoggerFatal()
    {
        TLogger::logStream->flush();
        abort();
    }
};