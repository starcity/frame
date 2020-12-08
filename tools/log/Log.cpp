#include <ctime>
#include <string.h>
#include <cstdarg>
#include <stdio.h>
#include "Log.h"



using std::fstream;
using std::string;
using std::ios;
using std::streampos;

#ifdef		_WINDOWS
#define		LOGFILENAME			"D:\\virtualManage"
#else
#define		LOGFILENAME			"/var/log/httpServer"
#endif


std::shared_ptr<CLog> CLog::g_clog = nullptr;

// 构造函数  
CLog::CLog():m_maxLogFileSize(LOG_DEFAULT_FILESIZE)
{

}

std::shared_ptr<CLog> CLog::get_inst()
{
    if(g_clog == nullptr)
        g_clog = std::shared_ptr<CLog>(new CLog());

    return g_clog;
}

bool CLog::init(LOG_LEVEL level,std::string filePath)
{
    m_showLogFlag = true;
	if(filePath.empty()){
		time_t nowTime = time(NULL);
		struct tm *pLocalTime = localtime(&nowTime);
		char timeString[100] = {0};
		sprintf(timeString, "%s%04d-%02d-%02d.log",LOGFILENAME, pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, 
			pLocalTime->tm_mday);
		filePath = timeString;
	}
	m_fileOut.open(filePath, ios::out | ios::app);
	m_openSuccess = m_fileOut.is_open();
	m_logFilePath = filePath;
    m_LogLevel = level;

    return true;
}

bool CLog::OpenLogFile(std::string pFilePath, LOG_LEVEL level)
{
    if(pFilePath.empty()){
        return false;
    }

    if (m_openSuccess)  // 如果已经打开文件 关闭原文件
    {
        CloseLogFile();
    }
    m_fileOut.open(pFilePath, ios::out | ios::app);

	m_openSuccess = m_fileOut.is_open();
	m_LogLevel = level; 
	m_logFilePath = pFilePath;

    return true;
}


// <Summary>得到日志文件大小</Summary>
// <DateTime>2013/9/9</DateTime>
// <Returns>日志文件大小</Returns>
size_t CLog::GetLogFileSize()
{
	if (!m_openSuccess)
	{
		return 0;
	}
	streampos curPos = m_fileOut.tellg();  // 当前文件指针位置

	m_fileOut.seekg(0, ios::end);  // 将文件指针指向文件末尾 
	streampos pos = m_fileOut.tellg(); // 获取文件指针的位置 该值即为文件大小
	m_fileOut.seekg(curPos);   // 恢复文件指针位置
	return static_cast<size_t>(pos);
}

// 将logText写入日志文件
// level: 该条日志信息级别  logText：日志内容
void CLog::WriteLog(string logText, LOG_LEVEL level)
{
    if (m_openSuccess && level >= m_LogLevel)
    {
        string flag;
        switch(level)
        {
            case LL_DEBUG:
                flag = "[DEBUG] ";
            break;
            case LL_INFO:
                flag = "[INFO] ";
            break;
            case LL_WARN:
                flag = "[WARN] ";
            break;
            case LL_ERROR:
                flag = "[ERROR] ";
            break;
        }

        if (m_maxLogFileSize > 0 && GetLogFileSize() > m_maxLogFileSize)  // 日志信息超过最大日志大小 则先清空日志
		{
			ChangeLogFile();
		}
		// 写日志信息
		if (m_showLogFlag)
		{
			m_fileOut.write(flag.c_str(), flag.size());
		}
		// 在日志后添加时间信息
		time_t nowTime = time(NULL);
        struct tm *pLocalTime = localtime(&nowTime);
		char timeString[100] = {0};
		sprintf(timeString, " [%04d-%02d-%02d %02d:%02d:%02d]	", pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, 
			pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
		m_fileOut.write(timeString, strnlen(timeString, sizeof(timeString)));
		logText +="\n";
		m_fileOut.write(logText.c_str(), logText.size()); 
        m_fileOut.flush();
    }
    return;
}

void CLog::WriteLog(LOG_LEVEL level, const char *pLogText, ...)
{
    va_list args, args_tmp;

    // 初始化args
    va_start(args, pLogText);

    // args1 是 args 的一个拷贝
    va_copy(args_tmp, args);

    // 使用nullptr和0作为前两个参数来获取格式化这个变长参数列表所需要的字符串长度
    // 使用 string(size_t n, char c) 构造函数，构造一个长度为n的字符串，内容为n个c的拷贝
    string res(1 + vsnprintf(nullptr, 0, pLogText, args_tmp), 0);
    // args1 任务完成，将其关闭，清理。
    va_end(args_tmp);

    // 使用args来格式化要返回的字符串res， 指定长度size
    vsnprintf(&res[0], res.size(), pLogText, args);
    m_mutex.lock();
    WriteLog(res, level);
    m_mutex.unlock();
    // args 任务完成，关闭，清理
    va_end(args);
}

// <Summary>格式化字符串</Summary>
// <DateTime>2013/10/31</DateTime>
// <Parameter name="srcString" type="IN">需要进行格式化的字符串</Parameter>
// <Parameter name="pFormatString" type="IN">格式字符串</Parameter>
// <Returns>格式化后的字符串</Returns>
const string& CLog::StringFormat(std::string &srcString, const char *pFormatString, ...)
{
    va_list args;
    char tempString[2048] = {0};  // 格式化字符串最大支持2048个字节
    va_start(args, pFormatString);
    vsnprintf(tempString, sizeof(tempString), pFormatString, args);
    srcString = tempString;
    return srcString;
}

// <Summary>将日志字符串转化为真正的日志信息</Summary>
// <DateTime>2013/10/31</DateTime>
// <Parameter name="pLogText" type="IN">日志字符串</Parameter>
// <Parameter name="level" type="IN">日志级别</Parameter>
// <Returns>真正的日志信息</Returns>
string CLog::ConvertToRealLogText(const char *pLogText, LOG_LEVEL level)
{
    if (NULL == pLogText)
    {
        return string("");
    }

    // 得到当前系统时间
    time_t nowTime = time(NULL);
    struct tm *pLocalTime = localtime(&nowTime);

    string strLogText(pLogText);
    string::size_type pos = strLogText.find("$(");
    while (pos != string::npos)
    {
        string::size_type tempPos = strLogText.find(")", pos);
        if (tempPos != string::npos)
        {
            string strSubString = strLogText.substr(pos, tempPos - pos + 1);
            if (strSubString == string("$(DATE)"))
            {
                StringFormat(strSubString, "%04d-%02d-%02d", pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, pLocalTime->tm_mday);
            }
            else if (strSubString == string("$(TIME)"))
            {
                StringFormat(strSubString, "%02d:%02d:%02d", pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
            }
            else if (strSubString == string("$(DATETIME)"))
            {
                StringFormat(strSubString, "%04d-%02d-%02d %02d:%02d:%02d", pLocalTime->tm_year + 1900, 
                    pLocalTime->tm_mon + 1, pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, 
                    pLocalTime->tm_sec);
            }
            else if (strSubString == string("$(LEVELFLAG)"))
            {
                switch(level)
                {
                    case LL_DEBUG:
                        strSubString = "[DEBUG] ";
                    break;
                    case LL_INFO:
                        strSubString = "[INFO] ";
                    break;
                    case LL_WARN:
                        strSubString = "[WARN] ";
                    break;
                    case LL_ERROR:
                        strSubString = "[ERROR] ";
                    break;
                }
            }
            strLogText.replace(pos, tempPos - pos + 1, strSubString);
            pos = strLogText.find("$(", tempPos + 1);
        }
        else 
        {
            break;
        }
    }

    return strLogText;
}

// <Summary>
// 新增函数 用于将日志按要求的格式输出
// 以logText的实际内容和格式记录日志，为了便于日志记录，设置一些预定义常量，用于代表某些特殊的信息
// 常量的引用格式，类似makefile文件中的变量，即$(常量名称),常量名称为大写，目标已经可用的常量有：
//   1.$(DATE):表示当前日期 格式为YYYY-MM-DD
//   2.$(TIME):表示当前时间 格式为HH:MM:SS
//   3.$(LEVELFLAG):表示该日志的级别标志
//   4.$(DATETIME):表示当前日期和时间 格式为YYYY-MM-DD HH:mm:SS
// 例子：WriteLogEx(LL_ERROR, "[$(DATE) $(TIME)] 线程退出 线程id=%d [$(LEVELFLAG)]", 10);
// 效果：[2013-10-31 09:55:30] 线程退出 线程id=10 [ERROR]
// </Summary>
// <DateTime>2013/10/31</DateTime>
// <Parameter name="logText" type="IN">日志内容</Parameter>
// <Parameter name="level" type="IN">日志级别</Parameter>
void CLog::WriteLogEx(LOG_LEVEL level, const char *pLogText, ...)
{
    if (m_openSuccess && level <= m_LogLevel)
    {
        va_list args;
        char szLogText[LOG_LINE_MAX] = {0};
        va_start(args, pLogText);
        vsnprintf(szLogText, LOG_LINE_MAX - 1, pLogText, args);
        string strRealLogText = ConvertToRealLogText(szLogText, level);
        if (m_maxLogFileSize > 0 && GetLogFileSize() > m_maxLogFileSize)  // 日志信息超过最大日志大小 则先清空日志
        {
            ChangeLogFile();
        }
        m_fileOut.write(strRealLogText.c_str(), strRealLogText.length());
    }
}

void CLog::CloseLogFile()
{
    m_fileOut.clear();  // 清理流状态
    m_fileOut.close();  // 关闭流
    m_openSuccess = false;
}

// 删除日志文件内容
void CLog::ChangeLogFile()
{
	time_t nowTime = time(NULL);
	struct tm *pLocalTime = localtime(&nowTime);
	char timeString[100] = {0};
        sprintf(timeString, "%s%04d-%02d-%02d.log",m_logFilePath.c_str(), pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1,
		pLocalTime->tm_mday);
	string filePath = timeString;

	OpenLogFile(filePath.c_str());
}
