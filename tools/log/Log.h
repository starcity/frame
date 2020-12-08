/************************************************************************/
// 类名称：  CLog
// 命名空间：无
// 文件名：  Log.h  Log.cpp
// 创建时间: 2013-9-9
// 作者：    guiyou
// 类说明：  日志记录类  该类没有使用winapi实现 可以在linux中进行使用       
// 遗留问题：1.多个fstream打开同一个文件，写数据时会出现问题
//           2.多线程对同一个fstream写数据，同步问题
// 解决方法：1.定义一个日志管理类（单例）通过这个日志管理类来得到日志对象，对于已经打开的文件，直接返回该日志对象
//              否则实例化一个新的日志对象
               
//            2.增加一个加锁写文件的方法，调用此方法时，对文件写数据进行同步
/************************************************************************/

#ifndef _LOG_INCLUDE_H_
#define _LOG_INCLUDE_H_

#define LOG_LINE_MAX 2048   //日志单行最大字符长度
#define LOG_DEFAULT_FILESIZE	(10 * 1024 * 1024)
#include <fstream>
#include <string>
#include <memory>
#include <mutex>


using namespace std;
using std::string;
using std::fstream;
using std::ios;





class CLog:public std::enable_shared_from_this<CLog>
{
public:
	enum LOG_LEVEL     // 日志级别
	{
        LL_DEBUG = 1,
        LL_INFO = 2,
        LL_WARN = 3,  
		LL_ERROR = 4     
	};

private:
     static std::shared_ptr<CLog> g_clog;
     CLog();

public:
    static std::shared_ptr<CLog> get_inst();

    CLog(CLog &) = delete;
    CLog & operator= (CLog &) = delete;

    virtual ~CLog(void)
    {
        if (m_openSuccess)
        {
			CloseLogFile();
        }
    }

    bool init(LOG_LEVEL level = LL_INFO,std::string filePath = "");

    // 获得打开文件是否成功的标识
    bool GetOpenStatus() const
    {
        return m_openSuccess;
    }

	// 打开日志文件
    bool OpenLogFile(std::string pFilePath, LOG_LEVEL level = LL_INFO);
	// 写日志操作
    void WriteLog(LOG_LEVEL level, const char *pLogText, ...);
    void WriteLog(string logText, LOG_LEVEL level = LL_INFO);
    void WriteLogEx(LOG_LEVEL level, const char *pLogText, ...);
	// 得到日志文件大小
	size_t GetLogFileSize();
	// 清空日志文件内容
	void ChangeLogFile();
	void CloseLogFile();

    void SetLogLevel(LOG_LEVEL level = LL_INFO)
    {
        m_LogLevel = level;
    }
    LOG_LEVEL GetLogLevel() const
    {
        return m_LogLevel;
    }

    void SetShowFlag(bool flag = true)
    {
        m_showLogFlag = flag;
    }
    bool GetShowFlag() const
    {
        return m_showLogFlag;
    }

    void SetMaxLogFileSize(size_t size)
    {
        m_maxLogFileSize = size;
    }
    size_t GetMaxLogFileSize() const
    {
        return m_maxLogFileSize;
    }

private:
    CLog(const CLog &clog){};

protected:
    string ConvertToRealLogText(const char *pLogText, LOG_LEVEL level = LL_INFO);
    const std::string &StringFormat(std::string &srcString, const char *pFormatString, ...);
protected:
    fstream m_fileOut;
    bool m_openSuccess;  // 日志文件打开状态 true表示已经成功打开 否则没有打开
	string m_logFilePath; // 日志文件完整路径 string类型

protected:
    LOG_LEVEL m_LogLevel;  // 日志记录级别 默认为LL_ERROR
	bool m_showLogFlag;    // 显示日志标志  默认为true
	size_t m_maxLogFileSize; // 日志文件最大大小 超过该大小将被删除 默认为10MB
    std::mutex  m_mutex;//lock
};



#endif

