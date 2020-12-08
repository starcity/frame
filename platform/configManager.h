#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include <memory>
#include <string>



class ConfigManager:public std::enable_shared_from_this<ConfigManager>
{
    private:
        static std::shared_ptr<ConfigManager> g_configManager;
        std::string          listen_ip;
        int32_t		         listen_port ;
        int32_t		         httpd_timeout ; //in seconds
        int32_t              httpd_thread;
        int32_t              log_level;

    private:
        ConfigManager();

    public:
        static std::shared_ptr<ConfigManager> get_inst();

        ~ConfigManager();
        ConfigManager(ConfigManager &) = delete;
        ConfigManager & operator=(ConfigManager &) = delete;

        bool read_config();
        std::string get_listen_ip();
        int32_t     get_listen_port();
        int32_t     get_httpd_timeout();
        int32_t     get_httpd_thread();
        int32_t     get_log_level();
};

 
#endif // _CONFIG_MANAGER_H