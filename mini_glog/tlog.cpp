//
// Created by itachpei on 2019/8/1.
//

#include "tlog.h"

int TLogStreamBuf::flush_buffer()
{
    auto num = this->pcount();
    try
    {
        std::cout.write(this->pbase(), num).flush();
    }
    catch (std::ios_base::failure &fail)
    {
        return EOF;
    }
    if (m_saveLogFile)
    {
        try
        {
            m_ofs->write(this->pbase(), num).flush();
        }
        catch (std::ios_base::failure &fail)
        {
            return EOF;
        }
    }

    pbump(-num);
    return num;
}

const int TLogger::MAX_FILENAME_LEN = 20;
const std::array<std::string, 4> TLogger::LogSeverityNames{"INFO", "WARNING", "ERROR", "FATAL"};;
TLOG_SEVERITY TLogger::severity = TLOG_SEVERITY::INFO;
TLogStream *TLogger::logStream = nullptr;
NullStream *TLogger::nullLogStream = nullptr;
char *TLogger::buffer = nullptr;
std::ofstream *TLogger::ofs = nullptr;
bool TLogger::isInit = false;
size_t TLogger::defaultBufSize = 1024 * 10;


void TLogger::InitTLogger(char *buf, uint32_t bufSize, TLOG_SEVERITY sev, const char *logFile, bool saveLogFile)
{
    if (isInit)
    {
        TLOG(WARNING) << "TLogger can be initialized only once" << std::endl;
        return;
    }
    if (!buf)
    {
        TLogger::buffer = new char[bufSize];
        buf = TLogger::buffer;
    }
#ifdef TCNN_LOG_ENABLE
    if (saveLogFile)
    {
        if (!logFile)
        {
            struct tm tmTime{};
            time_t timestamp = time(nullptr);
            localtime_r(&timestamp, &tmTime);
            std::stringstream stime;
            stime << std::setw(4) << std::setfill('0') << tmTime.tm_year + 1900
                  << '-' << std::setw(2) << std::setfill('0') << 1 + tmTime.tm_mon
                  << '-' << std::setw(2) << std::setfill('0') << tmTime.tm_mday
                  << '_' << std::setw(2) << std::setfill('0') << tmTime.tm_hour
                  << '-' << std::setw(2) << std::setfill('0') << tmTime.tm_min
                  << '-' << std::setw(2) << std::setfill('0') << tmTime.tm_sec;
            std::string logFileStr = "tcnn_log_" + stime.str() + ".txt";
            fprintf(stdout, "Store log in default file: %s.\n", logFileStr.c_str());
            TLogger::ofs = new std::ofstream(logFileStr.c_str(), std::ios::trunc | std::ios::out);
        }
        else
        {
            fprintf(stdout, "Store log in file: %s.\n", logFile);
            TLogger::ofs = new std::ofstream(logFile, std::ios::trunc | std::ios::out);
        }
        if (!TLogger::ofs->is_open())
        {
            fprintf(stderr, "Open log file %s failed !\n", logFile);
        }
    }
#endif
    TLogger::logStream = new TLogStream(buf, bufSize, TLogger::ofs, TLogger::ofs != nullptr);
    TLogger::nullLogStream = new NullStream();
    TLogger::severity = sev;
    TLogger::isInit = true;
}

void TLogger::ShutdownTLogger()
{
    TLogger::logStream->flush();
    if (TLogger::ofs)
    {
        TLogger::ofs->close();
        delete TLogger::ofs;
    }
    delete[] TLogger::buffer;
}

void TLogger::genMsgHeader(std::ostream &stream, const char *filename, const char *funcname, int lineno, int sev)
{
    if ((int) sev < (int) TLogger::severity || sev < TCNN_LOG_SEVERITY)
    {
        validStream = false;
        return;
    }
    validStream = true;

    const int filenameLen = strlen(filename);
    std::string time_str;
    struct tm tm_time{};  // Time of creation of LogMessage
    time_t timestamp = time(nullptr);
    localtime_r(&timestamp, &tm_time);

    // log format
    stream << "[ " << TLogger::LogSeverityNames[sev]
           << ' ' << std::setw(4) << std::setfill('0') << tm_time.tm_year + 1900
           << '-' << std::setw(2) << std::setfill('0') << 1 + tm_time.tm_mon
           << '-' << std::setw(2) << std::setfill('0') << tm_time.tm_mday
           << ' ' << std::setw(2) << std::setfill('0') << tm_time.tm_hour
           << ':' << std::setw(2) << std::setfill('0') << tm_time.tm_min
           << ':' << std::setw(2) << std::setfill('0') << tm_time.tm_sec << " ";

    if (filenameLen > TLogger::MAX_FILENAME_LEN)
    {
        stream << "..." << filename + filenameLen - TLogger::MAX_FILENAME_LEN << " " << funcname << "."
               << lineno << " ] ";
    }
    else
    {
        stream << filename << " " << funcname << "." << lineno << " ] ";
    }
}

std::ostream &TLogger::stream()
{
    if (validStream)
    {
        return *TLogger::logStream;
    }
    else
    {
        return *TLogger::nullLogStream;
    }
}

//std::ostream &TLogger::nullStream()
//{
//    return *TLogger::nullLogStream;
//}