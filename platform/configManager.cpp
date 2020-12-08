#include "configManager.h"
#include "common.h"
#include <yaml.h>


#define  CONFIG_FILE	"config.yaml"


std::shared_ptr<ConfigManager> ConfigManager::g_configManager = nullptr;

ConfigManager::ConfigManager()
{

}

std::shared_ptr<ConfigManager> ConfigManager::get_inst()
{
    if(g_configManager == nullptr)
        g_configManager = std::shared_ptr<ConfigManager>(new ConfigManager());

    return g_configManager;
}

ConfigManager::~ConfigManager()
{

}

bool ConfigManager::read_config()
{
    FILE *fp = fopen(CONFIG_FILE,"r");
    yaml_parser_t parser;
    yaml_token_t  token;
    char name[50] = {0};

    if(!yaml_parser_initialize(&parser)){
    	LOG_INFO("Failed to initzlize parser");
        return false;
    }
    if(NULL == fp){
    	LOG_INFO("Failed to open config.yaml");
        return false;
    }
    
    yaml_parser_set_input_file(&parser,fp);

    do {
        yaml_parser_scan(&parser,&token);
        
        if(token.type == YAML_SCALAR_TOKEN){
            if(strcmp(name,"ip") == 0){
                listen_ip = (char *)token.data.scalar.value;
            }
            if(strcmp(name,"port") == 0){
                char *p = (char *)token.data.scalar.value;
                listen_port = atoi(p);
            }
            if(strcmp(name,"timeout") == 0){
                char *p = (char *)token.data.scalar.value;
                httpd_timeout = atoi(p);
            }
            if(strcmp(name,"thread") == 0){
                char *p = (char *)token.data.scalar.value;
                httpd_thread = atoi(p);
            }
            if(strcmp(name,"logLevel") == 0){
                char *p = (char *)token.data.scalar.value;
                log_level = atoi(p);;
            }
            else {
                    char *p = (char *)token.data.scalar.value;
                strncpy(name,p,sizeof(name) -1);
            }
        }
        if(token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);
    
    yaml_token_delete(&token);
    yaml_parser_delete(&parser);
    fclose(fp);

    return true;
}

std::string ConfigManager::get_listen_ip()
{
    return listen_ip;
}

int32_t ConfigManager::get_listen_port()
{
    return listen_port;
}

int32_t ConfigManager::get_httpd_timeout()
{
    return httpd_timeout;
}

int32_t ConfigManager::get_httpd_thread()
{
    return httpd_thread;
}

int32_t ConfigManager::get_log_level()
{
    return log_level;
}

