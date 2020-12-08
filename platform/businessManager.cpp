#include "businessManager.h"
#include <algorithm>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <evhttp.h>
#include "json/json.h"
#include "cmd.h"
#include "cencode.h"


typedef int32_t (Business_manager::*downLinkFunction)(struct evkeyvalq &params,string &reInfo);
typedef std::map<string,downLinkFunction> downMapFunction;


//下行指令映射消息列表
static downMapFunction gdownMapFunction = {
	{ HTTP_CMD_TEST		,&Business_manager::cmdTest},
};


 std::shared_ptr<Business_manager> Business_manager::g_business_mgr = nullptr;

Business_manager::Business_manager()
{
}


Business_manager::~Business_manager()
{
}

std::shared_ptr<Business_manager> Business_manager::get_inst()
{
	if(g_business_mgr == nullptr){
		g_business_mgr = std::shared_ptr<Business_manager>(new Business_manager());
		g_business_mgr->init();
	}

	return g_business_mgr;
}


int32_t Business_manager::init()
{
	LOG_INFO("Business_manager init ......");

	return CODE_RETURN_SUCESSED;
}

//接口   指令分发
int Business_manager::doPlatform(evkeyvalq &params, string &reInfo)
{
	Json::Value		root;

	const char *tmp = evhttp_find_header(&params,HTTP_CMD_COMMAND);
	if (tmp == NULL)
	{
		LOG_ERROR("%s is null",HTTP_CMD_COMMAND);
		root[HTTP_JSON_URL_CODE] = CODE_HTTP_FAILED;
		root[HTTP_JSON_URL_MSG] = HTTP_URL_CODE_FIELD_NULL;
		reInfo = root.toStyledString();
		return CODE_RETURN_ERROR_NO_COMMAND;
	}
	std::string cmd = tmp;
	//命令分发
	downMapFunction::iterator iter = gdownMapFunction.find(cmd);
	if (iter == gdownMapFunction.end())
	{
		LOG_ERROR("cmd:%s",cmd.c_str());
		root[HTTP_JSON_URL_CODE] = CODE_HTTP_FAILED;
		root[HTTP_JSON_URL_MSG] = HTTP_URL_CODE_CMD_NOT_FOUND;
		reInfo = root.toStyledString();
		return CODE_RETURN_ERROR_NO_COMMAND;
	}

	(this->*gdownMapFunction[cmd])(params,reInfo);

	return CODE_RETURN_SUCESSED;
}


int32_t Business_manager::cmdTest(evkeyvalq &params, string &reInfo)
{
	Json::Value  root;

	const char *path = evhttp_find_header(&params, HTTP_URL_NAME);
	if(NULL == path)
	{
		root[HTTP_JSON_URL_CODE] = CODE_HTTP_FAILED;
		root[HTTP_JSON_URL_MSG] = HTTP_URL_CODE_FIELD_NULL;
		reInfo = root.toStyledString();
		LOG_ERROR("path is NULL");
		return CODE_RETURN_ERROR_HTTP_FIELD;
	}

	root[HTTP_JSON_URL_CODE] = CODE_HTTP_SUCESSED;
	root[HTTP_JSON_URL_MSG]  = HTTP_URL_CODE_SUCESSED;

	reInfo = root.toStyledString();

	return CODE_RETURN_SUCESSED;
}
