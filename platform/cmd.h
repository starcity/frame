#ifndef		_CODE_DEFINE_H_
#define     _CODE_DEFINE_H_


#define		NETPROBEXML				                    "httpServer.xml"
#define		MYHTTPD_VERSION			                    "http v 0.0.1"





#define     CODE_RETURN_SUCESSED			            0x00
#define     CODE_RETURN_ERROR_INITIALIZE		        -0x01
#define     CODE_RETURN_ERROR_CREATEPTHREAD		        -0x02
#define     CODE_RETURN_ERROR_OPERATOR_NULL		        -0x03
#define     CODE_RETURN_ERROR_NO_OPERATOR		        -0x04
#define     CODE_RETURN_ERROR_NO_COMMAND                -0X05
#define     CODE_RETURN_ERROR_MYSQL_INIT		        -0x10
#define     CODE_RETURN_ERROR_MYSQL_QUERY		        -0x11
#define     CODE_RETURN_ERROR_MYSQL_GETEXEC		        -0x12
#define     CODE_RETURN_ERROR_MYSQL_ROLLBACK	    	-0x13
#define     CODE_RETURN_ERROR_MYSQL_COMMIT		        -0x14
#define     CODE_RETURN_ERROR_GROUPEXIST		        -0x50
#define     CODE_RETURN_ERROR_USERNOEXIST		        -0x51
#define     CODE_RETURN_ERROR_MYSQL_DATAWRONG		    -0xA0
#define     CODE_RETURN_ERROR_HTTP_FIELD		        -0xA1
#define     CODE_RETURN_ERROR_QUERY_DOMAIN_TIME_OUT     -0xA2
#define     CODE_RETURN_ERROR_REDIS_FALED               -0xA3
#define     CODE_RETURN_ERROR_NODE_OFF_LINE             -0xA4
#define     CODE_RETURN_ERROR_DOWN_DOMAIN_FAILED        -0xA5
#define     CODE_RETURN_ERROR_GENERAL                   -0xA6
#define     CODE_RETURN_ERROR_DOWN_DOMAIN_CONFLICT      -0xA7


#define    CODE_HTTP_SUCESSED			            	0x00
#define    CODE_HTTP_BUSY				                0x01
#define    CODE_HTTP_FAILED				                0x02


//页面指令操作码
#define   HTTP_CMD_COMMAND                              "cmd"

//cmd 指令
#define   HTTP_CMD_TEST                                 "test"



#define   HTTP_URL_NAME				                    "name" 


//json响应字段
#define   HTTP_JSON_URL_CODE			                "code"
#define   HTTP_JSON_URL_MSG			                    "msg"



#define  HTTP_URL_CODE_SUCESSED					        u8"操作成功"
#define  HTTP_URL_CODE_FIELD_NULL				        u8"指定的字段名为空" 
#define  HTTP_URL_CODE_FIELD_NO_EXIST  			        u8"指定的字段名不存在"
#define  HTTP_URL_CODE_OPEN_FILE_FAILED  			    u8"打开文件失败"
#define  HTTP_URL_CODE_FILE_NOT_FOUND  			        u8"文件不存在"
#define  HTTP_URL_CODE_CMD_NOT_FOUND 			        u8"指令不存在"
#define  HTTP_URL_CODE_CMD_LABEL_JPEG 			        u8"获取图片失败"


#endif

