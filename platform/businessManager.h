#ifndef BUSINESSMANAGEMENT
#define BUSINESSMANAGEMENT

#include <stdint.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "common.h"

using namespace std;


//文件ID 多线程安全
class onlyID
{
	public:
		onlyID():m_id(0x10000000){}
		uint64_t getOnlyNumber()
		{
			uint32_t number;
			m_lock.lock();
			number = ++m_id;
			m_lock.unlock();
			return number;
		}

	private:
		std::mutex  m_lock;
		uint32_t    m_id;
};


//typedef map<uint64_t,MDS_Handle> Map_file;

class Business_manager :public std::enable_shared_from_this<Business_manager>
{
	private:
		static std::shared_ptr<Business_manager> g_business_mgr;
	public:
		static std::shared_ptr<Business_manager> get_inst();

	public:
		~Business_manager();
		Business_manager(const Business_manager &) = delete;
		Business_manager & operator=(const Business_manager &) = delete;

	private:
		Business_manager();

		int32_t  init();

	public:
		int32_t doPlatform(struct evkeyvalq &params,string &reInfo);
		int32_t cmdTest(struct evkeyvalq &params,string &reInfo);
};

#endif // BUSINESSMANAGEMENT

