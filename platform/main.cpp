#include "httpServer.h"
#include  <exception>


int main(int argc, char *argv[])
{
    try{
        CLog::get_inst()->init();
        ConfigManager::get_inst()->read_config();
        CLog::get_inst()->SetLogLevel((CLog::LOG_LEVEL)ConfigManager::get_inst()->get_log_level());
        
        LOG_INFO("programm start.....!");

        httpServer server;
        server.start();
    }catch(std::exception & e){
        LOG_ERROR("exception:%s",e.what());
    }

    return 0;
}

