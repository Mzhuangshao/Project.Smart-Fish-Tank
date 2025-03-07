#include "sys.h"
u8 TEMIP_MAX;
u8 TEMIP_MIN;
u8 mqtt_buff[400];
u8 Connect_flag = 0;
u8 ConnectPack_flag = 0;
u8 SubscribePack_flag = 0;
u8 Ping_flag = 0;
int sensorMax = 0;
uint16_t ReadData = 0;

float tempx;
unsigned char  mqtt_TxBuf[7][400];
unsigned char *mqtt_TxInPtr;
unsigned char *mqtt_TxOutPtr;
unsigned char *mqtt_TxEndPtr;
char str[20] = "";
unsigned char  mqtt_RxBuf[7][400];
unsigned char *mqtt_RxInPtr;
unsigned char *mqtt_RxOutPtr;
unsigned char *mqtt_RxEndPtr;
char dest[5] = {0};
unsigned char  mqtt_CMDBuf[7][400];
unsigned char *mqtt_CMDInPtr;
unsigned char *mqtt_CMDOutPtr;
unsigned char *mqtt_CMDEndPtr;

char clientid[128];
int  clientid_size;
char lex[256];
char username[128];
int  username_size;

char passward[128];
int  passward_size;

char ServerIP[128];
int  ServerPort;

void Mqtt_Subscribe(char *topic_name, int QoS);
void mqtt_DealTxData(unsigned char *data, int size);

void MQTT_Buff_Init(void)
{
    mqtt_TxInPtr = mqtt_TxBuf[0];
    mqtt_TxOutPtr = mqtt_TxInPtr;
    mqtt_TxEndPtr = mqtt_TxBuf[6];
    mqtt_RxInPtr = mqtt_RxBuf[0];
    mqtt_RxOutPtr = mqtt_RxInPtr;
    mqtt_RxEndPtr = mqtt_RxBuf[6];
    mqtt_CMDInPtr = mqtt_CMDBuf[0];
    mqtt_CMDOutPtr = mqtt_CMDInPtr;
    mqtt_CMDEndPtr = mqtt_CMDBuf[6];
    Mqtt_ConnectMessege();
    Mqtt_Subscribe(SUBSCRIBE_TOPIC, 1);
}
void Mqtt_Subscribe(char *topic_name, int QoS)
{
    int payload_size;
    int remaining_size;
    int len;
    int all_size = 1;
    int variable_size = 2;
    payload_size = 2 + strlen(topic_name) + 1;
    remaining_size = variable_size + payload_size;
    mqtt_buff[0] = 0x82;

    while (remaining_size > 0) {
        len = remaining_size % 128;
        remaining_size = remaining_size / 128;

        if (remaining_size > 0) {
            len = len | 0x80;
        }

        mqtt_buff[all_size++] = len;
    }

    mqtt_buff[all_size + 0] = 0x00;
    mqtt_buff[all_size + 1] = 0x0A;
    mqtt_buff[all_size + 2] = strlen(topic_name) / 256;
    mqtt_buff[all_size + 3] = strlen(topic_name) % 256;
    memcpy(&mqtt_buff[all_size + 4], topic_name, strlen(topic_name));
    mqtt_buff[all_size + 4 + strlen(topic_name)] = QoS;
    mqtt_DealTxData(mqtt_buff, all_size + variable_size + payload_size);
}
void Mqtt_ConnectMessege()
{
    int remaining_size;
    int len;
    int variable_size;
    int payload_size;
    int all_size = 1;
    variable_size = 10;
    payload_size = 2 + clientid_size  + 2 + username_size  + 2 + passward_size;
    remaining_size = variable_size + payload_size;
    mqtt_buff[0] = 0x10;

    while (remaining_size > 0) {
        len = remaining_size % 128;
        remaining_size = remaining_size / 128;

        if (remaining_size > 0) {
            len = len | 0x80;
        }

        mqtt_buff[all_size++] = len;
    }

    mqtt_buff[all_size + 0] = 0x00;
    mqtt_buff[all_size + 1] = 0x04;
    mqtt_buff[all_size + 2] = 0x4D;
    mqtt_buff[all_size + 3] = 0x51;
    mqtt_buff[all_size + 4] = 0x54;
    mqtt_buff[all_size + 5] = 0x54;
    mqtt_buff[all_size + 6] = 0x04;
    mqtt_buff[all_size + 7] = 0xC2;
    mqtt_buff[all_size + 8] = 0x00;
    mqtt_buff[all_size + 9] = 0x64;
    mqtt_buff[all_size + 10] = clientid_size / 256;
    mqtt_buff[all_size + 11] = clientid_size % 256;
    memcpy(&mqtt_buff[all_size + 12], clientid, clientid_size);
    mqtt_buff[all_size + 12 + clientid_size] = username_size / 256;
    mqtt_buff[all_size + 13 + clientid_size] = username_size % 256;
    memcpy(&mqtt_buff[all_size + 14 + clientid_size], username, username_size);
    mqtt_buff[all_size + 14 + clientid_size + username_size] = passward_size / 256;
    mqtt_buff[all_size + 15 + clientid_size + username_size] = passward_size % 256;
    memcpy(&mqtt_buff[all_size + 16 + clientid_size + username_size], passward, passward_size);
    mqtt_DealTxData(mqtt_buff, all_size + variable_size + payload_size);
    //	for(j=0;j<sizeof(mqtt_buff);j++)
    //	{
    //		u1_printf("%x/",mqtt_buff[j]);
    //	}
    //    u1_printf("\r\n");
}
void mqtt_PublishQs0(char *topic, char *data, int data_len)
{
    int all_size = 1;
    int variable_size, payload_size, remaining_size, len;
    variable_size = 2 + strlen(topic);
    payload_size = data_len;
    remaining_size = variable_size + payload_size;
    mqtt_buff[0] = 0x30;

    while (remaining_size > 0) {
        len = remaining_size % 128;
        remaining_size = remaining_size / 128;

        if (remaining_size > 0) {
            len = len | 0x80;
        }

        mqtt_buff[all_size++] = len;
    }

    mqtt_buff[all_size + 0] = strlen(topic) / 256;
    mqtt_buff[all_size + 1] = strlen(topic) % 256;
    memcpy(&mqtt_buff[all_size + 2], topic, strlen(topic));
    memcpy(&mqtt_buff[all_size + 2 + strlen(topic)], data, data_len);
    //	for(j=0;j<sizeof(mqtt_buff);j++)
    //	{
    //		u1_printf("%x ",mqtt_buff[j]);
    //	}
    //    u1_printf("\r\n");
    mqtt_DealTxData(mqtt_buff, all_size + variable_size + payload_size);
}
void mqtt_DealCmdSet(unsigned char *data, int size)
{
    memcpy(&mqtt_CMDInPtr[2], data, size);
    mqtt_CMDInPtr[0] = size / 256;
    mqtt_CMDInPtr[1] = size % 256;
    mqtt_CMDInPtr[size + 2] = '\0';
    mqtt_CMDInPtr += 400;

    if (mqtt_CMDInPtr == mqtt_CMDEndPtr) {
        mqtt_CMDInPtr = mqtt_CMDBuf[0];
    }
}
void mqtt_Dealsetdata_Qs0(unsigned char *redata)
{
    int i, topic_len, cmd_len, cmd_local, temp, temp_len, multiplier;
    unsigned char tempbuff[400];
    unsigned char *data;
    temp_len = 0;
    i = multiplier = 1;
    data = &redata[2];

    do {
        temp = data[i];
        temp_len += (temp & 127) * multiplier;
        multiplier *= 128;
        i++;
    } while ((temp & 128) != 0);

    topic_len = data[i] * 256 + data[i + 1] + 2;
    cmd_len = temp_len - topic_len;
    cmd_local = topic_len + i;
    memcpy(tempbuff, &data[cmd_local], cmd_len);
    mqtt_DealCmdSet(tempbuff, cmd_len);
}

void mqtt_DealTxData(unsigned char *data, int size)
{
    mqtt_TxInPtr[0] = size / 256;
    mqtt_TxInPtr[1] = size % 256;
    memcpy(&mqtt_TxInPtr[2], data, size);
    mqtt_TxInPtr += 400;

    if (mqtt_TxInPtr == mqtt_TxEndPtr) {
        mqtt_TxInPtr = mqtt_TxBuf[0];
    }
}

void mqtt_Ping(void)
{
    mqtt_buff[0] = 0xC0;
    mqtt_buff[1] = 0x00;
    mqtt_DealTxData(mqtt_buff, 2);
}

void Ali_MsessageInit()
{
    memset(clientid, 128, 0);
    sprintf(clientid, "%s", DEVICENAME);
    clientid_size = strlen(clientid);
    memset(username, 128, 0);
    sprintf(username, "%s", PRODUCTKEY);
    username_size = strlen(username);
    memset(passward, 128, 0);
    sprintf(passward, "%s", DEVICESECRE);
    passward_size = strlen(passward);
    memset(ServerIP, 128, 0);
    sprintf(ServerIP, "%s", BAD);                //构建服务器域名
    ServerPort = 1883;
}

char *mid(char *dst, char *src, int n, int m) /*n为长度，m为位置*/
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);

    if (n > len) {
        n = len - m;    /*从第m个到最后*/
    }

    if (m < 0) {
        m = 0;    /*从第一个开始*/
    }

    if (m > len) {
        return NULL;
    }

    p += m;

    while (n--) {
        *(q++) = *(p++);
    }

    *(q++) = '\0'; /*有必要吗？很有必要*/
    return dst;
}





void mqtt_Content(void)
{
    if (Connect_flag == 0) {
        memset(WiFi_RX_BUF, 0, 1024);
        WiFi_RxCounter = 0;
        TIM_Cmd(TIM4, DISABLE);
        TIM_Cmd(TIM3, DISABLE);
        u1_printf("正在连接MQTT服务器\r\n");

        if (WIFI_Connect() == 0) {
            Connect_flag = 1;
            u1_printf("已连接MQTT服务器\r\n\r\n");
            memset(WiFi_RX_BUF, 0, 1024);
            WiFi_RxCounter = 0;
            MQTT_Buff_Init();
            TIM_Cmd(TIM4, ENABLE);
            TIM_Cmd(TIM3, ENABLE);
        }
    } else {
        if (mqtt_TxOutPtr != mqtt_TxInPtr) {
            if ((mqtt_TxOutPtr[2] == 0x10) || ((mqtt_TxOutPtr[2] == 0x82) && ConnectPack_flag == 1) || SubscribePack_flag == 1) {
                u1_printf("发送数据:0x%x\r\n", mqtt_TxOutPtr[2]);
                mqtt_TxData(mqtt_TxOutPtr);
                mqtt_TxOutPtr += 400;

                if (mqtt_TxOutPtr == mqtt_TxEndPtr) {
                    mqtt_TxOutPtr = mqtt_TxBuf[0];
                }
            }
        }

        if (mqtt_RxOutPtr != mqtt_RxInPtr) {
            if (mqtt_RxOutPtr[2] == 0x20) {
                switch (mqtt_RxOutPtr[5]) {
                case 0x00 :
                    u1_printf("CONNECT报文连接成功\r\n\r\n");
                    ConnectPack_flag = 1;
                    break;

                case 0x01 :
                    u1_printf("0x01连接已拒绝，不支持的协议版本，准备重启\r\n");
                    Connect_flag = 0;
                    break;

                case 0x02 :
                    u1_printf("0x02连接已拒绝，不合格的客户 端标识符，准备重启\r\n");
                    Connect_flag = 0;
                    break;

                case 0x03 :
                    u1_printf("0x03连接已拒绝，服务端不可用，准备重启\r\n");
                    Connect_flag = 0;
                    break;

                case 0x04 :
                    u1_printf("0x04连接已拒绝，无效的用户名 或密码，准备重启\r\n");
                    Connect_flag = 0;
                    break;

                case 0x05 :
                    u1_printf("0x05连接已拒绝，未授权，准备重启\r\n");
                    Connect_flag = 0;
                    break;

                default   :
                    u1_printf("连接已拒绝，准备重启\r\n");
                    Connect_flag = 0;
                    break;
                }
            } else if (mqtt_RxOutPtr[2] == 0x90) {
                switch (mqtt_RxOutPtr[6]) {
                case 0x01 :
                    u1_printf("订阅成功\r\n\r\n");
                    SubscribePack_flag = 1;
                    TIM3_Init(60000 - 1, 7200 - 1);
                    Ping_flag = 0;
                    mqtt_PublishQs0(P_TOPIC_NAME, lex, strlen(lex));
                    break;

                default   :
                    u1_printf("订阅失败，准备重启\r\n");
                    Connect_flag = 0;
                    break;
                }
            } else if (mqtt_RxOutPtr[2] == 0xD0) {
                u1_printf("Ping回复正常！\r\n");
                Ping_flag = 1;
            } else if ((mqtt_RxOutPtr[2] == 0x30)) {
                u1_printf("MQTT服务器推送的内容是\r\n");
                mqtt_Dealsetdata_Qs0(mqtt_RxOutPtr);
            }

            mqtt_RxOutPtr += 400;

            if (mqtt_RxOutPtr == mqtt_RxEndPtr) {
                mqtt_RxOutPtr = mqtt_RxBuf[0];
            }
        }

        if (mqtt_CMDOutPtr != mqtt_CMDInPtr) {
					
					
					
					 
						
               if (strstr((char *)mqtt_CMDOutPtr + 2, "zidong")) {   OperateMode=0;   oled_Clear(); }
					     if (strstr((char *)mqtt_CMDOutPtr + 2, "dingshi")) {  OperateMode=1;    oled_Clear(); }
               if (strstr((char *)mqtt_CMDOutPtr + 2, "yuzhi"))     {OperateMode=2;   oled_Clear(); }
               if (strstr((char *)mqtt_CMDOutPtr + 2, "shoudong")) { OperateMode=3;    oled_Clear();}
					
						   if (strstr((char *)mqtt_CMDOutPtr + 2, "weishi"))    {GoTimeFlag=0;   TimeFlag=0;}
               if (strstr((char *)mqtt_CMDOutPtr + 2, "zengyang"))  {GoTimeFlag=1;   TimeFlag=0;}
					//
					       if (strstr((char *)mqtt_CMDOutPtr + 2, "TimeOn")) {
									  TimeFlag=4;						 
								   if(GoTimeFlag==0) TimeFlag1=1;   	else  STimeFlag1=1;
								 }	
								 
									if (strstr((char *)mqtt_CMDOutPtr + 2, "TimeOff")) {
									  TimeFlag=4;									 
								   if(GoTimeFlag==0) TimeFlag1=0;   	else  STimeFlag1=0;
								 } 
				     //
					      if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor5down")) {
									  TimeFlag=1;						 
								   if(GoTimeFlag==0)  SensorData.Sensor5--;   	else  SensorData.Sensor8--;
								 }	
								 
									if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor5add")) {
									  TimeFlag=1;									 
								   if(GoTimeFlag==0)  SensorData.Sensor5++;   	else  SensorData.Sensor8++;  
								 } 
					//
								 	if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor6down")) {
									  TimeFlag=2;						 
								   if(GoTimeFlag==0)  SensorData.Sensor6--;   	else  SensorData.Sensor9--;
								 }	
								 
									if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor6add")) {
									  TimeFlag=2;									 
								   if(GoTimeFlag==0)  SensorData.Sensor6++;   	else  SensorData.Sensor9++;  
								 } 
									//
					 								 	if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor7down")) {
									  TimeFlag=3;						 
								   if(GoTimeFlag==0) GoFoodTime--;   	else   GoOxTime--;
								 }	
								 
									if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor7add")) {
									  TimeFlag=3;									 
								   if(GoTimeFlag==0)  GoFoodTime++;   	else   GoOxTime++;  
								 } 
					     //
								  if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor1down")) {
									     	if(IndexFlag1==4){oled_Clear();}
												
												   IndexFlag1=1;
										       CursorFlag1=1;
								          Threshold.Sensor1MAX--;   
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor1add")) {
										   	if(IndexFlag1==4){oled_Clear();}
												
												   IndexFlag1=1;
										       CursorFlag1=1;
								           Threshold.Sensor1MAX++;   
								 }	
								//
								  if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor2down")) {
											 if(IndexFlag1==4){oled_Clear();}
										
												   IndexFlag1=2;
										       CursorFlag1=2;
								           Threshold.Sensor2MAX--;   
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor2add")) {
										 	 if(IndexFlag1==4){oled_Clear();}
										 
												   IndexFlag1=2;
										       CursorFlag1=2;
								           Threshold.Sensor2MAX++;   
								 }								 

								 
								if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor3down")) {
										 if(IndexFlag1==4){oled_Clear();}
									
												   IndexFlag1=3;
									         CursorFlag1=3;
								           Threshold.Sensor3MAX--;   
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor3add")) {
										 if(IndexFlag1==4){oled_Clear();}
										 
												   IndexFlag1=3;
										       CursorFlag1=3;
								           Threshold.Sensor3MAX++;   
								 }
								 
								 if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor4down")) {
									         if(IndexFlag1<4){oled_Clear();}
												   IndexFlag1=4;
									         CursorFlag1=1;
									         
								           Threshold.Sensor4MAX--;   
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Sensor4add")) {
										  if(IndexFlag1<4){oled_Clear();}
												   IndexFlag1=4;
										       CursorFlag1=1;
								           Threshold.Sensor4MAX++;   
								 }
					      //
								if (strstr((char *)mqtt_CMDOutPtr + 2, "ExecuteOn")) {
									     if(IndexFlag>3){  oled_Clear();} 
									    
								    	 CursorFlag=1;
										   IndexFlag=1;		  
								       System.Execute = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "ExecuteOff")) {
										  if(IndexFlag>3){  oled_Clear();} 

										   CursorFlag=1;
											 IndexFlag=1;			
											 System.Execute = 0;  										 
								 }
								 
								if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute2On")) {
									 if(IndexFlag>3){  oled_Clear();} 
									
									     CursorFlag=2;
										   IndexFlag=2;		  
								       System.Execute2 = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute2Off")) {
										  if(IndexFlag>3){  oled_Clear();} 
										 
										  CursorFlag=2;
											 IndexFlag=2;			
											 System.Execute2 = 0;  										 
								 }
						     
								 
								 if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute3On")) {
									  if(IndexFlag>3){  oled_Clear();} 
							
									     CursorFlag=3;
										   IndexFlag=3;		  
								       System.Execute3 = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute3Off")) {
										  if(IndexFlag>3){  oled_Clear();} 
										  
										   CursorFlag=3;
											 IndexFlag=3;			
											 System.Execute3 = 0;  										 
								 }
								 
								if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute4On")) {
									 if(IndexFlag<=3){  oled_Clear();} 
									
									    
										   IndexFlag=4;	
												CursorFlag=0;									
								       System.Execute4 = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute4Off")) {
										  if(IndexFlag<=3){  oled_Clear();} 
										 
										 CursorFlag=0;		
										
											 IndexFlag=4;			
											 System.Execute4 = 0;  										 
								 }
								 
								  	if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute5On")) {
											 if(IndexFlag<=3){  oled_Clear();} 
											
											CursorFlag=1;		
										   IndexFlag=5;		  
								       System.Execute5 = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute5Off")) {
										  if(IndexFlag<=3){  oled_Clear();} 
										 
										   CursorFlag=1;		
											 IndexFlag=5;			
											 System.Execute5 = 0;  										 
								 }
								 
								 if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute6On")) {
									  if(IndexFlag<=3){  oled_Clear();} 
									 
									      CursorFlag=2;		
										   IndexFlag=6;		  
								       System.Execute6 = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute6Off")) {
										  if(IndexFlag<=3){  oled_Clear();} 
										 
										     CursorFlag=2;		
											 IndexFlag=6;			
											 System.Execute6 = 0;  										 
								 }
									 
								 	if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute7On")) {
										 if(IndexFlag<=3){  oled_Clear();} 
										
										     CursorFlag=3;		
										   IndexFlag=7;		  
								       System.Execute7 = 1;  
								 }	
									
								   if (strstr((char *)mqtt_CMDOutPtr + 2, "Execute7Off")) {
										  if(IndexFlag<=3){  oled_Clear();} 
										 
										   CursorFlag=3;		
											 IndexFlag=7;			
											 System.Execute7 = 0;  										 
								 }
								 
            memset(mqtt_CMDOutPtr + 2, 0x00, sizeof(mqtt_CMDOutPtr + 2));
            mqtt_CMDOutPtr += 400;

            if (mqtt_CMDOutPtr == mqtt_CMDEndPtr) {
                mqtt_CMDOutPtr = mqtt_CMDBuf[0];
            }
        }
    };
}


