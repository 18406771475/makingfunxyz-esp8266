/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "modules/tcpServerClient.h"


/****************************
 *   TCP CLIENT FUNCTIONS   *
 ****************************/

/**********************************
 *  TCP CLIENT STATIC PROTOTYPES  *
 **********************************/
static void tcp_client_sent_cb(void *arg);
static void tcp_client_recv_cb(void *arg,char *pdata,unsigned short length);
static void tcp_client_recon_cb(void *arg,sint8 error);
static void tcp_client_discon_cb(void *arg);
static void tcp_client_connect_cb(void *arg);

/**********************************
 *   TCP CLIENT STATIC VARIABLES  *
 **********************************/
os_timer_t tcp_client_send_data_timer;
struct espconn tcp_client;
uint8 i;

/**********************************
 *   TCP CLIENT STATIC FUNCTIONS  *
 **********************************/

/**
 * TCP Client���ݷ��ͻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void *arg){
	os_printf("tcp client send data successful\r\n");
}

/**
 * TCP Client���ݽ��ջص��������������⴦���յ�Server����������
 */
static void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("tcp client receive tcp server data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Client�����ص������������ڴ˺������������Ӵ���
 */
static void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void *arg,sint8 error){
	os_printf("tcp client connect tcp server error %d\r\n",error);
	os_timer_disarm(&tcp_client_send_data_timer);//ȡ����ʱ�������ݶ�ʱ��
}

/**
 * TCP Client�Ͽ����ӻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void *arg){
	os_printf("tcp client disconnect tcp server successful\r\n");
	os_timer_disarm(&tcp_client_send_data_timer);
}

/**
 * TCP Client���ӳɹ��ص�����
 */
static void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void *arg){
	struct espconn *pespconn = arg;

	os_printf("tcp client connect tcp server successful\r\n");
	espconn_regist_recvcb(pespconn,tcp_client_recv_cb);//ע��������ݻص�����
	espconn_regist_sentcb(pespconn,tcp_client_sent_cb);//ע�����ݷ�����ɻص�����
	espconn_regist_disconcb(pespconn,tcp_client_discon_cb);//ע��Ͽ����ӻص�����

	os_timer_disarm(&tcp_client_send_data_timer);
	os_timer_setfn(&tcp_client_send_data_timer, (os_timer_func_t *) tcp_client_send_data,NULL);//ע��Client��ʱ�������ݻص�����
	os_timer_arm(&tcp_client_send_data_timer, 1000, true);//ʱ������Ϊ1s
}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS  *
 **********************************/
/**
 * TCP Client��ʱ�������ݻص�����
 */
void ICACHE_FLASH_ATTR
tcp_client_send_data(void){
	char buf[256],length;
	os_printf("tcp client send data tcp server\r\n");
	length = os_sprintf(buf,(char *)"Hi this is ESP8266 client! message num %d",i);
	i++;
	espconn_sent(&tcp_client,buf,length);
}

/**
 * TCP Client��ʼ������
 * @remote_ip    Ҫ���ӵ�TCP Server IP��ַ
 * @local_ip     ESP8266 IP��ַ
 * @remote_port  Ҫ���ӵ�TCP Server�˿ں�
 */
void ICACHE_FLASH_ATTR
tcp_client_init(uint8 *remote_ip,struct ip_addr *local_ip, int remote_port){

	uint32 server_ip = ipaddr_addr(remote_ip);

	os_printf("tcp client connect to tcp server\r\n");
	tcp_client.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	tcp_client.type = ESPCONN_TCP;

	os_memcpy(tcp_client.proto.tcp->remote_ip,&server_ip,4);//����Ҫ���ӵ�Server IP��ַ
	tcp_client.proto.tcp->remote_port = remote_port;//����Ҫ���ӵ�Server �˿ں�
	os_memcpy(tcp_client.proto.tcp->local_ip,local_ip,4);//���ñ���IP��ַ
	tcp_client.proto.tcp->local_port = espconn_port();//���ñ��ض˿ں�

	espconn_regist_connectcb(&tcp_client,tcp_client_connect_cb);//ע�����ӳɹ��ص�����
	espconn_regist_reconcb(&tcp_client,tcp_client_recon_cb);//ע������������ӻص�����

	espconn_connect(&tcp_client);//Client����Server
}



/****************************
 *   TCP SERVER FUNCTIONS   *
 ****************************/
/**********************************
 *  TCP SERVER STATIC PROTOTYPES  *
 **********************************/
static void tcp_server_sent_cb(void *arg);
static void tcp_server_recv_cb(void *arg,char *pdata,unsigned short length);
static void tcp_server_recon_cb(void *arg,sint8 error);
static void tcp_server_discon_cb(void *arg);
static void tcp_server_listen_cb(void *arg);

/**********************************
 *   TCP SERVER STATIC VARIABLES  *
 **********************************/
os_timer_t tcp_server_send_data_timer;
struct espconn tcp_server;
uint8 z;

/**********************************
 *   TCP server STATIC FUNCTIONS  *
 **********************************/

/**
 * TCP Server���ݷ��ͻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void *arg){
	os_printf("tcp server send data successful\r\n");

}

/**
 * TCP Server���ݽ��ջص��������������⴦���յ�Client����������
 */
static void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("tcp server receive tcp client data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Server�����ص������������ڴ˺������������Ӵ���
 */
static void ICACHE_FLASH_ATTR
tcp_server_recon_cb(void *arg,sint8 error){
	os_printf("tcp server connect tcp client error %d\r\n",error);
	os_timer_disarm(&tcp_server_send_data_timer);
}

/**
 * TCP Server�Ͽ����ӻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void *arg){
	os_printf("tcp server disconnect tcp client successful\r\n");
	os_timer_disarm(&tcp_server_send_data_timer);
}

/**
 * TCP Server����Client���ӻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_server_listen_cb(void *arg){
	struct espconn *pespconn = arg;

	os_printf("tcp server have tcp client connect\r\n");
	espconn_regist_recvcb(pespconn,tcp_server_recv_cb);//ע���յ����ݻص�����
	espconn_regist_sentcb(pespconn,tcp_server_sent_cb);//ע�ᷢ�������ݻص�����
	espconn_regist_disconcb(pespconn,tcp_server_discon_cb);//ע��Ͽ����ӻص�����

	os_timer_disarm(&tcp_server_send_data_timer);
	os_timer_setfn(&tcp_server_send_data_timer, (os_timer_func_t *) tcp_server_send_data,NULL);//ע��Server��ʱ�������ݻص�����
	os_timer_arm(&tcp_server_send_data_timer, 1000, true);//����ʱ��Ϊ1s
}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS  *
 **********************************/

/**
 * TCP Server��ʱ�������ݻص�����
 */
void ICACHE_FLASH_ATTR
tcp_server_send_data(void){
	char buf[256],length;
	os_printf("tcp server send data tcp client\r\n");
	length = os_sprintf(buf,(char *)"Hi this is ESP8266 server! message num %d",z);
	z++;
	espconn_send(&tcp_server,buf,length);
}

/**
 * TCP Server��ʼ������
 * @local_port ���ؼ����˿ںţ���Client���ӵĶ˿ں�һ��
 */
void ICACHE_FLASH_ATTR
tcp_server_init(uint16 local_port){

	os_printf("tcp server waiting tcp client connect!\r\n");
	tcp_server.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	tcp_server.type = ESPCONN_TCP;

	tcp_server.proto.tcp->local_port = local_port;//���ñ��ؼ����Ķ˿ںţ��ȴ�Client����

	espconn_regist_connectcb(&tcp_server,tcp_server_listen_cb);//ע��Server�����ص�����
	espconn_regist_reconcb(&tcp_server,tcp_server_recon_cb);//ע������������ӻص�����

	espconn_accept(&tcp_server);//����Server,��ʼ����
	espconn_regist_time(&tcp_server,360,0);//��ʱ�Ͽ�����ʱ��
}
