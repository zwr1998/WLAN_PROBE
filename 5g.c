#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
void dispatch_ServerMsg(char* recvbufFromSer);
int  dispatch_2g_command();
int  dispatch_server_command();
void ReadStrUnit(char* str, char* temp_str, int idx, int len);
int GetSubStrPos(char* str1, char* str2);
void getMac(char* str, int i);
void* packetcapture(void);
void printmessage(void);
void  MacAddressReporter(int size);
int ready_socket();
int ready_socket_2g();
int determine_the_frame_type(char *s);
void determine_add_macaddress(char *s,int type);
void printf_current_mac_info(int num);
int get_frame_type(char *s);
int checkout_ssid(char *data, int ch);
void ready_to_send();
void handle_with_control(char *data, int frame_type);
void write_file(int type,int channel);
void print_ssid_info();
void handle_with_probe_request(char *data);
void attack_wifi2g();
int get_channel(char *data);
void send_begin_cap_to_windows();


//*****************************************************************

#define MAXMACNUMBER  100000

#define MACADDRESSLEN  20
#define RECVFROMBUF MAXMACNUMBER * sizeof(mac_data_static) + 100
#define SSIDLEN  100


#define ERROR         -1
#define FRAME_MANAGER 0
#define FRAME_CONTROL 1
#define FRAME_DATA    2
#define FRAME_UNKNOW  3
#define FRAME_PROBE   4


#define FRAME_BEACON 5
#define FRAME_PROBERESPONSE 6

//*****************************************************************
typedef struct
{
 char ssid[SSIDLEN];
 int channel;
 int command;//0:干扰，具体SSID见ssid[]，1：停止
}command;


typedef struct
{
 char staMacAddress[MACADDRESSLEN];
 int NumOfProbReq;//number of Probe Request frame
 int NumOftotal;//number of total manager and control frame
 int band;//2:  2G,5:    5G
 int channel;//channel
 int NumOfData;//number of data frame
 int option;//0: normal,1: exceed the max mac number(MAXMACNUMBER)
 char ssid[SSIDLEN];
 char apMacAddress[MACADDRESSLEN];
}mac_data_static;


typedef struct
{
	char ssid[SSIDLEN];
	char ap_mac[MACADDRESSLEN];
	int channel;
}wl_net;


mac_data_static maccal[MAXMACNUMBER ];//用来存放获取得到的MAC地址
mac_data_static maccal2[MAXMACNUMBER];//用来存放获取得到的2gMAC地址
mac_data_static maccal3[1000];//用来存放获取得到的AP          MAC地址

wl_net networkcal[1000];


char sendbuf[MAXMACNUMBER * sizeof(mac_data_static) + 100];//发送buffer
char recvbufFromSer[MAXMACNUMBER * sizeof(mac_data_static) + 100];//接收buffer
char recvbufFromSer_Windows[MAXMACNUMBER ];


//int sockServer = -1;//通信本地套接字
struct sockaddr_in addrServer;//保存Windows的ip和端口
struct sockaddr_in temp_addr;
struct sockaddr_in loc_addr; //和windows 通信的socket
struct sockaddr_in loc_addr2;// 和2g抓包通信的socket
struct sockaddr_in loc_addr3; //和windows 通信的udp  socket

int loc_addr_len;
int loc_addr_len2;
int loc_addr_len3;

socklen_t addrServerlen = sizeof(addrServer);//通信对象大小
socklen_t temp_addr_len = sizeof(temp_addr);
char ServerIP[20] = "192.168.0.101";
int  port = 5051;



int fram_type = -1;
int current_mac_num = 0;
int current_mac_num2 = 0;
int current_network_num = 0;



FILE* fp = NULL;//指向管道的文件指针
char data[400] = { '0' };/* temp print */
char mac1[20]= { '0' };
char mac2[20]= { '0' };

char temp_str[30]= { '0' };    // 临时子串
int sk;
int sk2;
int sk3_udp;

int main()
{	
	int res = 0;
	ready_socket(); /* ready udp sk3 receive command and  tcp sk to send mac info */
	ready_socket_2g(); /* ready tcp sk2 to receive 2g info */
    packetcapture();
	send_begin_cap_to_windows();
	pthread_t id1, id2,id3;
	res = pthread_create(&id1, NULL, (void*)printmessage, NULL);
    if (res) {
        printf("create pthread printmessage error\n");
    }
	 res = pthread_create(&id2, NULL, (void*)dispatch_server_command, NULL);
    if (res) {
        printf("create pthread COMMU1 error\n");
    } 
	res = pthread_create(&id3, NULL, (void*)dispatch_2g_command, NULL);
    if (res) {
        printf("create pthread COMMU2g error\n");
    }
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
   	pthread_join(id3, NULL);
    pclose(fp);
    return 0;
}

void send_begin_cap_to_windows()
{
	char bufn[1000];
	command sign;
	sign.command = 2;
	memcpy(bufn,(char *)&sign,sizeof(command));
	if(sendto(sk3_udp,bufn,sizeof(bufn),0,(struct sockaddr*)&addrServer,addrServerlen) != -1){
		///printf("Send signal begin capture success\n");
	}
}

void printmessage(void)/* 打印抓到的信息 */
{
    int size = 0;

	int get_type_flag = 0;
	int get_type_next = 0;

	int checkflag = 0;
	char *temp_str = NULL;
	char* str2 = "A:";
	int index = 0;
	int len = 0;
	int len2 =0;
	int ch = 0;
	int ret = 0;
    while (fgets(data, sizeof(data), fp) != NULL){

		ch =  get_channel(data);		   //Current 5G 信道    Done
		checkflag = checkout_ssid(data,ch);/* get ssid from beacon and probe response data first num include this info  Done*/
		if(checkflag)   				
			continue;   /* beacon and probe response  */

		
		temp_str = data;  /* CTS/RTS  BA/BAR  ACK/PS POLL  QOS Data*/
        index = 0;
		len = strlen(data);
        index = GetSubStrPos(temp_str, str2);// str2 = "A:"  find the position of A:;


        if (index < 0 && get_type_flag == 1) {
			if(get_type_next == 1){
				
				get_type_flag = 0;
				len2 = strlen(data);
				///printf("get_frame 0x0010  :len : %d, %s\n",len2,data);

				fram_type = get_frame_type(data);  //collect  frame type
				
				if(fram_type == FRAME_CONTROL || fram_type == FRAME_DATA){
					fgets(data, sizeof(data), fp); //include mac address string
					 handle_with_control(data,fram_type);
				}
				else if(fram_type == FRAME_PROBE){
					 fgets(data, sizeof(data), fp);
					 handle_with_probe_request(data);

				}
			
			}			
			get_type_next++;
        }
		if (size == 40 && get_type_flag == 0) {
			ready_to_send();
			
            MacAddressReporter(current_mac_num);
		
			printf_current_mac_info(current_mac_num);
			
			print_ssid_info();
			
			sleep(1);
			write_file(-1,0); // write 			
            size = 0;
        }
		
     	if(index >= 0){                      //find  mac address position                  
			get_type_flag = 1;
			get_type_next = 0;
            ///printf("i = %d   ", index);
			///printf("len/5 ---- >%d   ", len);
        	///printf("get ---- >%s\n\n", data);
			//getMac(temp_str, index);
        	size++;
			printf("sned size : %d\n",size);
			}
        } 
    }
	

void* packetcapture(void)/* 开始监听网卡 */
{
    fp = popen("/data/tcpdump -i radiotap0 -e -xxvv", "r");
    if (fp == NULL){
        printf("popen error!\n");
        return NULL;
    }
}


int ready_socket_2g()
{
	int ret = 0;
	//配置本地信息2
	bzero(&loc_addr2, sizeof(loc_addr2));
    loc_addr2.sin_family = AF_INET;                                //设置为IPV4通信
    loc_addr2.sin_addr.s_addr = htonl(INADDR_ANY);                 //设置目的ip
    loc_addr2.sin_port = htons(5055);                              //设置本地端口去链接服务器
    loc_addr_len2 = sizeof(loc_addr2);
	sk2 = socket(AF_INET, SOCK_STREAM, 0); 
	if(sk2<0){
		printf("socket2 create failure\n");
		return -1;
			}
	ret = bind(sk2,(struct sockaddr*)&loc_addr2,loc_addr_len2);
    if(ret < 0){
        printf("socket bind failure %d\n",__LINE__);
		sleep(5);
        return -1;
    }

	listen(sk2, 5);
}


int  dispatch_2g_command()//socket with 2gclient
{
	int sockConn = 0;
    while (1) {                    //接收来自2g客户端的数据
    	sockConn = accept(sk2, NULL, NULL);
        if (recv(sockConn, recvbufFromSer, sizeof(recvbufFromSer),MSG_WAITALL) < 0){
            printf("receive error from 2g\n");
			sleep(5);
        	}
        else{
			printf("!!!Received from 2gServer!!!\n");
			sleep(3);
            dispatch_ServerMsg(recvbufFromSer);}
            
    }
    return 0;
}






int ready_socket()
{
	int ret = 0;
	int flag = 1;
	/********************************/
	/*  tcp server and tcp client  */
	addrServer.sin_family = AF_INET; 	//使用AF_INET协议族 
	addrServer.sin_port = htons(port);  //设置地址结构体中的端口号
	addrServer.sin_addr.s_addr = inet_addr(ServerIP);//htonl(INADDR_ANY);   //inet_addr(ServerIP);   //设置通信ip
	   
	//配置本地信息
    bzero(&loc_addr, sizeof(loc_addr));
	loc_addr.sin_family = AF_INET;                                //设置为IPV4通信
    loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);                 //设置目的ip
    loc_addr.sin_port = htons(5050);                              //设置本地端口去链接服务器
    loc_addr_len = sizeof(loc_addr);
	   
  	sk = socket(AF_INET, SOCK_STREAM, 0);                         
   	 if(sk<0){
      	printf("socket create failure,%d\n",__LINE__);
        return -1;
     }
    setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(sk,(struct sockaddr*)&loc_addr,loc_addr_len);
    if(ret < 0){
        printf("socket bind failure5\n");
        return -1;
    } 

	/*           udp server      */
	bzero(&loc_addr3, sizeof(loc_addr3));
    loc_addr3.sin_family = AF_INET;                                //设置为IPV4通信
    loc_addr3.sin_addr.s_addr = htonl(INADDR_ANY);                 //设置目的ip
    loc_addr3.sin_port = htons(5051);                              //设置本地端口去链接服务器
    loc_addr_len3 = sizeof(loc_addr3);
	   
  	sk3_udp = socket(AF_INET, SOCK_DGRAM, 0);                         
   	if( sk3_udp < 0 ){
      	printf("socket create failure,%d\n",__LINE__);
        return -1;
     }
    ret = bind(sk3_udp,(struct sockaddr*)&loc_addr3,loc_addr_len3);
    if(ret < 0){
        printf("socket bind failure ,%d\n",__LINE__);
        return -1;
    } 
}


int  dispatch_server_command()//socket with server
{
    while (1) {
        if (recvfrom(sk3_udp,recvbufFromSer_Windows, sizeof(recvbufFromSer_Windows), 0,NULL,NULL) < 0){
            printf("receive error from Windows Server\n");
			sleep(5);
        	}
        else{
			printf("!!!Received from Window Server  Reboot!!!\n");
			sleep(5);
            dispatch_ServerMsg(recvbufFromSer_Windows);
		}
            
    }
    return 0;
}

void  MacAddressReporter(int size) //add 2g
{	
    int i;
	int flag =1;
    char* p = sendbuf;
	char *pp = sendbuf;
	int sub = 0;
	int sub2 = 0;
	int ret;
	
	close(sk);
	sk = socket(AF_INET, SOCK_STREAM, 0);                         
   	if(sk<0){
      printf("socket create failure %d\n",__LINE__);
      return;
     }
    setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(sk,(struct sockaddr*)&loc_addr,loc_addr_len);
    if(ret < 0){
        printf("socket bind failure  %d\n",__LINE__);
        sleep(5);
    }
	//memcpy(p,(char *)&current_mac_num,sizeof(current_mac_num));
    p = p +sizeof(current_mac_num);
	
    for (i = 0; i < current_mac_num; i++) {
		if(strcmp(maccal[i].apMacAddress,"") != 0 && strcmp(maccal[i].staMacAddress,"") == 0){sub++;continue;}
		
        memcpy(p, (char*)&maccal[i], sizeof(mac_data_static));
        p += sizeof(mac_data_static);
    }
	
	 for (i = 0; i < current_mac_num2; i++) {
	 ///if(strcmp(maccal2[i].apMacAddress,"") != 0 && strcmp(maccal2[i].staMacAddress,"") == 0){sub2++;continue;}
        memcpy(p, (char*)&maccal2[i], sizeof(mac_data_static));
        p += sizeof(mac_data_static);
    }

	 for(i =0;i<current_network_num;i++){
		strcpy(maccal3[i].apMacAddress,networkcal[i].ap_mac);
		strcpy(maccal3[i].ssid,networkcal[i].ssid);
	 	maccal3[i].channel = networkcal[i].channel;
		if(1<=maccal3[i].channel && maccal3[i].channel<=14){
			maccal3[i].band = 2;
		}else if(36<= maccal3[i].channel && maccal3[i].channel <=165){
			maccal3[i].band = 5;
		}else{
			maccal3[i].band = 0;
		}
	 }

	 for( i = 0;i<current_network_num;i++){
		memcpy(p,(char *)&maccal3[i],sizeof(mac_data_static));
		p += sizeof(mac_data_static);
	 }
	
	i = current_mac_num + current_mac_num2 + current_network_num - sub; //- sub2;
	memcpy(pp,(char *)&i,sizeof(i));
	if(connect(sk, (const struct sockaddr*)&addrServer, sizeof(struct sockaddr_in)) != -1){
		 if(send(sk, &sendbuf, (i + 5 + 1000) * sizeof(mac_data_static), MSG_NOSIGNAL) != -1){
			printf("********7g**************7g**************7g**************7g******\n");
			printf("mac_num : %d i %d\n", current_mac_num, i);
			printf("send 777g success\n");
			printf("**************\n");
			close(sk);
			sleep(15);
		 }else{
			printf("SEND 8000 to Windows FAIL,%d\n",__LINE__);
			close(sk);
			sleep(15);
		 }
	}
}


void  dispatch_ServerMsg(char* recvbufFromSer) 
{
	/*  1:benin attack
		2:5:
		9: recv 2g info


		*/
	int i = 0;
	int channel = 0;
    char *p = recvbufFromSer;
	char *p2 = recvbufFromSer;
	char *p3 = recvbufFromSer;
	mac_data_static * ppp = NULL;
	int *num_2g = NULL;
	command *pp = (command *)p;
	if(pp->command == 1){
		printf(" 重启以干扰信号\n");
		printf(" 需要干扰的SSID: %s\n", pp->ssid);
		sleep(5);
		for( i = 0; i< current_network_num; i++){
			if(strcmp(pp->ssid,networkcal[i].ssid) == 0){
				channel = networkcal[i].channel;
				break;
				}
			}
		channel = pp->channel;
		write_file(0,channel);
		system("rm /etc/rc3.d/S90cap.sh");
		system("cp /DHD/S91attack.sh  /etc/rc3.d/S91attack.sh");
		system("chmod 777 /etc/rc3.d/S91attack.sh");
		system("reboot");
	}
	else if(pp->command == 2){
	}
	else if(pp->command == 5){
	}
	else if(pp->command == 9){
		// deal with 2g message , store then send to baolaoshi
		num_2g = (int *)(p2+ sizeof(command));
		current_mac_num2 = *num_2g;
		ppp = (mac_data_static *)(p3 + sizeof(command) + sizeof(int));
		for( i = 0; i<current_mac_num2; i++){
			memcpy(&maccal2[i],ppp+i,sizeof(mac_data_static));
			//p3++;
		}
	}
}

void ReadStrUnit(char* str, char* temp_str, int idx, int len)  // 从母串中获取与子串长度相等的临时子串
{
    int index = 0;
    for (index; index < len; index++)
    {
        temp_str[index] = str[idx + index];
    }
    temp_str[index] = '\0';
}

int GetSubStrPos(char* str1, char* str2)
{
    int idx = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 < len2)
    {
       /// printf("error 1 \n"); // 子串比母串长
        return -1;
    }

    while (1)
    {
        ReadStrUnit(str1, temp_str, idx, len2);    // 不断获取的从 母串的 idx 位置处更新临时子串
        if (strcmp(str2, temp_str) == 0)break;      // 若临时子串和子串一致，结束循环
        idx++;                                  // 改变从母串中取临时子串的位置
        if (idx >= len1)return -1;                 // 若 idx 已经超出母串长度，说明母串不包含该子串
    }

    return idx;    // 返回子串第一个字符在母串中的位置
}

void printf_current_mac_info(int num)
{
	int i = 0;
	for(i = 0; i < num; i++){
		if(strcmp(maccal[i].apMacAddress,"") != 0 && strcmp(maccal[i].staMacAddress,"") == 0){continue;}
		printf("#################5G#####################\n");
		printf("mac_sta_address : %s\n",maccal[i].staMacAddress);
		printf("mac_ap_address : %s\n",maccal[i].apMacAddress);
		printf("mac_ap_ssid : %s\n",maccal[i].ssid);
		printf("mac_num_total : %d\n",maccal[i].NumOftotal);
		printf("mac_band : %d\n",maccal[i].band);
		printf("mac_channel : %d\n",maccal[i].channel);
		printf("mac_data_num : %d\n",maccal[i].NumOfData);
		printf("mac_probe_num : %d\n",maccal[i].NumOfProbReq);
	}
		for(i = 0; i < current_mac_num2; i++){
		if(strcmp(maccal2[i].apMacAddress,"") != 0 && strcmp(maccal2[i].staMacAddress,"") == 0){continue;}
		printf("*************2G*****************\n");
		printf("*************2G*****************\n");
		printf("mac_sta_address : %s\n",maccal2[i].staMacAddress);
		printf("mac_ap_address : %s\n",maccal2[i].apMacAddress);
		printf("mac_ap_ssid : %s\n",maccal2[i].ssid);
		printf("mac_num_total : %d\n",maccal2[i].NumOftotal);
		printf("mac_band : %d\n",maccal2[i].band);
		printf("mac_channel : %d\n",maccal2[i].channel);
		printf("mac_data_num : %d\n",maccal2[i].NumOfData);
		printf("mac_probe_num : %d\n",maccal2[i].NumOfProbReq);
	}
		
}

int get_frame_type(char *s)
{
	int type_index = 39;
	int type = -1;
	int subtype = 100;
	int len = strlen(s);
	char tem1[2] = "\0";
	char tem2[2] = "\0";
	char *target = "0x0010";
	int target_index = GetSubStrPos(s,target);
	if(len > 40 && target_index >= 0){
		///printf("1\n");
		tem1[0] = s[40];
		tem2[0] = s[41];
 		
		subtype = atoi(tem1);
		type = atoi(tem2);
		///printf("typde %d, sub %d\n",type,subtype);
		if(type == 0 && subtype == 4)
			return FRAME_PROBE;
		else if(type == 0 && subtype == 8)
			return FRAME_BEACON;
		else if(type == 0 && subtype == 5)
			return FRAME_PROBERESPONSE;
		switch (type){
			case 0:
			case 1:
			case 2:
			case 3:
				return FRAME_MANAGER;
				break;
			case 4:
			case 5:
			case 6:
			case 7:
				return FRAME_CONTROL;
				break;
			case 8:
			case 9:
				return FRAME_DATA;
				break;
			default :
				return FRAME_UNKNOW;
				break;
		}
	}
	return FRAME_UNKNOW;
}


int get_channel(char *data)
{
	int index_start = -1;
	int index_stop = -1;
	int i = 0;
	int count = 0;
	char *data2 = data;
	char num[5] = "";
	index_start = GetSubStrPos(data, "tsft");
	index_start = index_start + 5;

	index_stop = GetSubStrPos(data2, "MHz");
	index_stop = index_stop - 1;
	///printf("Channel start : %d, stop : %d\n",index_start,index_stop);
	if(index_start < index_stop && (index_stop - index_start)<5){
		for(i = index_start; i<index_stop; i++){
			num[count] = data[i];
			count++;

		}
		i = atoi(num);

		
		if(2000<i && i<2500){
		i = ((i-2412)/5) + 1;
		return i;
		}

		
		switch (i)
		{
		case 5180:
			return 36;
			break;
		case 5190:
			return 38;
			break;
		case 5200:
			return 40;
			break;
		case 5220:
			return 44;
			break;
		case 5230:
			return 46;
			break;
		case 5240:
			return 48;
			break;
		case 5260:
			return 52;
			break;
		case 5270:
			return 54;
			break;
		case 5280:
			return 56;
			break;
		case 5300:
			return 60;
			break;
		case 5310:
			return 62;
			break;
		case 5320:
			return 64;
		case 5745:
			return 149;
			break;
		case 5755:
			return 151;
			break;
		case 5765:
			return 153;
			break;
		case 5785:
			return 157;
			break;
		case 5795:
			return 159;
			break;
		case 5805:
			return 161;
			break;
		case 5825:
			return 165;
			break;	
		}
		//i = ((i-5170) / 10 )* 2 +34;
	}
	return 6;
}



int checkout_ssid(char *data, int ch)
{
	
	char *ret1 = NULL;
	char *ret2 = NULL;
	char ssid[SSIDLEN] = "";
	char mac[MACADDRESSLEN] = "";
	int i = 0;
	int count = 0;
	int flag = 0;
	
	int index1 = 0;
	int index2 = 0;
	char tag1[20] = "Probe Response";  // BSSID:5a:a0:23:b7:d0:c7 (oui Unknown) DA:00:90:4c:1b:00:01 (oui Unknown) SA:5a:a0:23:b7:d0:c7 (oui Unknown) Probe Response (
	char tag2[10] = "Beacon";          // BSSID:9c:a6:15:e9:b9:c1 (oui Unknown) DA:Broadcast SA:9c:a6:15:e9:b9:c1 (oui Unknown) Beacon (GD) [6.0* 9.0 1
	ret1 = strstr(data,tag1);
	ret2 = strstr(data,tag2);
	
	
	if(ret1){
		flag = 1;
		index1 = GetSubStrPos(ret1,"(");
		index2 = GetSubStrPos(ret1,")");
		for(i = index1 +1 ; i< index2; i++){
			ssid[count] = ret1[i];
			count++;
		}
		///printf("Probe Response : ssid ->  %s",ssid);
	}
	
	if(ret2){
		flag = 1;
		index1 = GetSubStrPos(ret2,"(");
		index2 = GetSubStrPos(ret2,")");
		for(i = index1 +1 ; i< index2; i++){
				ssid[count] = ret2[i];
				count++;
		}
		///printf("Beacon : mac ->  %s",ssid);
	}

	if(flag){
		count = 0;
		index1 =  GetSubStrPos(data,"D:");
		index2 = index1 + 18;
		index1 = index1 + 2;
		for(index1 ;index1<=index2;index1++){
			mac[count] = data[index1];
			count++;
		}
		
		///printf("======mac ->  %s\n",mac);

		for(i = 0; i < current_network_num; i++){
			if(strcmp(ssid,networkcal[i].ssid) == 0){
				networkcal[i].channel = ch;
				break;
			}

		}

		if( i == current_network_num){
			strcpy(networkcal[current_network_num].ap_mac,mac);
			strcpy(networkcal[current_network_num].ssid,ssid);
			networkcal[current_network_num].channel = ch;
			///printf("Current packeture Channel : %d\n",ch);
			current_network_num++;
		}
		return 1;
	}

	return 0;
	//copy to global net   num++	
}


void handle_with_control(char *data, int frame_type)/*add to global mac not classfied but not repeat*/
{

	char *res = NULL;
	int len = 0;
	char mac1[20] = "";
	char mac2[40] = "";
	char broadcast[20] = "ff:ff:ff:ff:ff:ff";
	int i = 0;
	int j = 0;
	int flag = 0;

	
	res = strstr(data,"0x0020");
	len = strlen(data);
	///printf("0020  len : %d\n",len);
	///printf("0020 data : %s\n",data);
	if(res && len >45){
		mac1[i++] = data[10]; mac1[i++] = data[11]; mac1[i++] = ':'; mac1[i++] = data[12]; mac1[i++] = data[13]; mac1[i++] = ':';
		mac1[i++] = data[15]; mac1[i++] = data[16]; mac1[i++] = ':'; mac1[i++] = data[17]; mac1[i++] = data[18]; mac1[i++] = ':';
		mac1[i++] = data[20]; mac1[i++] = data[21]; mac1[i++] = ':'; mac1[i++] = data[22]; mac1[i++] = data[23]; i = 0;
		
		mac2[j++] = data[25]; mac2[j++] = data[26]; mac2[j++] = ':'; mac2[j++] = data[27]; mac2[j++] = data[28]; mac2[j++] = ':';
		mac2[j++] = data[30]; mac2[j++] = data[31]; mac2[j++] = ':'; mac2[j++] = data[32]; mac2[j++] = data[33]; mac2[j++] = ':';
		mac2[j++] = data[35]; mac2[j++] = data[36]; mac2[j++] = ':'; mac2[j++] = data[37]; mac2[j++] = data[38];

		flag =1;
		///printf("Control mac1  -> %s  mac2 -> %s \n",mac1,mac2);
		if(strcmp(mac1,broadcast) == 0)
			memset(mac1,0,sizeof(mac1));
		if(strcmp(mac2,broadcast) == 0)
			memset(mac2,0,sizeof(mac2));
		///printf("Enter ： ！！！\n");
	}

	i = 0;
	
	if(flag == 1){
	for(i = 0; i< current_mac_num; i++){
		if(strcmp(maccal[i].apMacAddress,"") == 0 && strcmp(maccal[i].staMacAddress,mac1) == 0 && 
			strlen(mac2) > 1 && strlen(mac1) > 1){
			
			strcpy(maccal[i].apMacAddress,mac2);
			if(frame_type == FRAME_CONTROL){
			maccal[i].NumOftotal++;
			}
			else{
				maccal[i].NumOfData++;
			}
			break;

			}
		else if(strcmp(maccal[i].apMacAddress,"") == 0 && strcmp(maccal[i].staMacAddress,mac2) == 0 && 
			strlen(mac1) > 1 && strlen(mac2) > 1){
			
			strcpy(maccal[i].apMacAddress,mac1);
			if(frame_type == FRAME_CONTROL){
			maccal[i].NumOftotal++;
			}
			else{
				maccal[i].NumOfData++;
			}
			break;
			}
			
		else if((strcmp(maccal[i].staMacAddress,mac1)== 0 && strcmp(maccal[i].apMacAddress,mac2) == 0) || 
			(strcmp(maccal[i].staMacAddress,mac2)== 0 && strcmp(maccal[i].apMacAddress,mac1) == 0)){
			if(frame_type == FRAME_CONTROL){
				maccal[i].NumOftotal++;}
			else{maccal[i].NumOfData++;}
			break;
			}
		
	}

	
	if( i==current_mac_num ){
		strcpy(maccal[i].staMacAddress,mac1);
		strcpy(maccal[i].apMacAddress,mac2);
		if(frame_type == FRAME_CONTROL){
				maccal[i].NumOftotal = 1;
		}
		else{
			maccal[i].NumOfData = 1;
		}
		current_mac_num++;
	}
		}
}


void ready_to_send()/* classfied the mac and ssid*/
{
	int i = 0;
	int j = 0;
	for(i = 0; i < current_network_num; i++){
		for(j = 0; j<current_mac_num;j++){
			if(strcmp(maccal[j].apMacAddress,networkcal[i].ap_mac) == 0){
				strcpy(maccal[j].ssid,networkcal[i].ssid);
				maccal[j].channel = networkcal[i].channel;
				if(networkcal[i].channel >=36)
					maccal[j].band = 5;
				else if(networkcal[i].channel <=13 && networkcal[i].channel >=1)
					maccal[j].band = 2;
			} else if(strcmp(maccal[j].staMacAddress,networkcal[i].ap_mac) == 0){
				
				strcpy(maccal[j].staMacAddress,maccal[j].apMacAddress);
				strcpy(maccal[j].apMacAddress,networkcal[i].ap_mac);
				strcpy(maccal[j].ssid,networkcal[i].ssid);
				maccal[j].channel = networkcal[i].channel;
				if(networkcal[i].channel >=36)
					maccal[j].band = 5;
				else if(networkcal[i].channel <=13 && networkcal[i].channel >=1)
					maccal[j].band = 2;
				
				}		
		}
	}
}


void write_file(int type, int channel) /* write sendbuf content  and network info to a file*/
{
	char data[1024] = "";
	FILE *fp1 = NULL;
	switch (type)
	{
	case 0:
		fp1 = fopen("/DHD/Channel.txt", "w+");
		if(channel >=1 && channel <= 165 ){}
		else{channel = 6;}
		sprintf(data,"%d",channel);
		fputs(data, fp1);
		fclose(fp1);
		return;
		break;
	
	case 1:
		fp1 = fopen("/DHD/Control.txt", "w+");
		fputs(data, fp1);
		fclose(fp1);
		return;
		break;
	
	default:
		break;
	}	


	int i =0;
	char *p = sendbuf;
	char *p2 = p;
	int *num2 = (int *)p2;

	fp1 = fopen("/DHD/5GMACADDRESS.txt", "w+");
	sprintf(data,"SUM MAC : %d\n",*num2);
	fputs(data, fp1);
	mac_data_static *pp = (mac_data_static *)(p + sizeof(current_mac_num));
	for( i = 0; i< *num2; i++){
		//if(strcmp(pp->apMacAddress,"") != 0 && strcmp(pp->staMacAddress,"") == 0){pp++;continue;}
		sprintf(data," stamac : %20s apmac : %20s  ssid : %20s   channel : %3d total num : %5d  probe request num : %5d   data num: %5d\n",
			pp->staMacAddress,pp->apMacAddress,pp->ssid,pp->channel,pp->NumOftotal,pp->NumOfProbReq,pp->NumOfData);
		fputs(data, fp1);
		pp++;

	}

	sprintf(data,"****************NETWORK*********************\n");
	fputs(data, fp1);
	sprintf(data,"SUM Network : %d\n",current_network_num);
	fputs(data, fp1);
	for( i = 0 ; i < current_network_num; i++){
		sprintf(data,"SSID: %30s   MAC : %20s  Channel : %d\n", networkcal[i].ssid,networkcal[i].ap_mac,networkcal[i].channel);
		fputs(data, fp1);
		

	}
	
	fclose(fp1);
}

void print_ssid_info()
{
	int i = 0;
	for( i= 0; i< current_network_num; i++){
		printf("******************************\n");
	
		printf("network num : %d\n",current_network_num);
		
		printf("ssid : %s    mac :  %s   channel : %d\n",networkcal[i].ssid,networkcal[i].ap_mac,networkcal[i].channel);

	}
	
}


void handle_with_probe_request(char *data)/*add to global mac not classfied but not repeat*/
{

	char *res = NULL;
	int len = 0;
	char mac1[20] = "";
	char mac2[20] = "";
	int i = 0;
	int j = 0;
	int flag = 0;

	res = strstr(data,"0x0020");
	len = strlen(data);
	///printf("0020  len : %d\n",len);
	///printf("0020 data : %s\n",data);
	if(res && len >45){
		mac1[i++] = data[10]; mac1[i++] = data[11]; mac1[i++] = ':'; mac1[i++] = data[12]; mac1[i++] = data[13]; mac1[i++] = ':';
		mac1[i++] = data[15]; mac1[i++] = data[16]; mac1[i++] = ':'; mac1[i++] = data[17]; mac1[i++] = data[18]; mac1[i++] = ':';
		mac1[i++] = data[20]; mac1[i++] = data[21]; mac1[i++] = ':'; mac1[i++] = data[22]; mac1[i++] = data[23]; i = 0;
		
		mac2[j++] = data[25]; mac2[j++] = data[26]; mac2[j++] = ':'; mac2[j++] = data[27]; mac2[j++] = data[28]; mac2[j++] = ':';
		mac2[j++] = data[30]; mac2[j++] = data[31]; mac2[j++] = ':'; mac2[j++] = data[32]; mac2[j++] = data[33]; mac2[j++] = ':';
		mac2[j++] = data[35]; mac2[j++] = data[36]; mac2[j++] = ':'; mac2[j++] = data[37]; mac2[j++] = data[38];

		flag =1;
		printf("probe_request mac1  -> %s  mac2 -> %s \n",mac1,mac2);
	
	}

	i = 0;
	
	if(flag == 1){
		for(i = 0; i< current_mac_num; i++){
			if(strcmp(maccal[i].staMacAddress,mac2)== 0 || strcmp(maccal[i].apMacAddress,mac2)== 0){
				maccal[i].NumOfProbReq++;
				printf("++++++++++equal+++++++++++++++++\n");
				break;
			}
		}
	
	if( i==current_mac_num ){
		strcpy(maccal[current_mac_num].staMacAddress,mac2);
		if(!strstr(mac1,"ff:ff")){
			strcpy(maccal[current_mac_num].apMacAddress,mac1);
			}
		maccal[i].NumOfProbReq = 1;
		current_mac_num++;
			}
	}
}

