#include "tcp_server.h"

#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "string.h"

//ICACHE_FLASH_ATTR 
xSemaphoreHandle  sentFlagSemaphore;
xQueueHandle sendQueue;

static struct espconn espconn_struct;
static esp_tcp tcp;
LOCAL uint16_t server_timeover = 300;//60*60*12; // yes. 12h timeout. so what? :)

typedef enum request {GET=0, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE} request_t;
char* request_list[] = { "GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "NaN" };


typedef enum status {_200=0, _404, _403} status_t;
char* status_list[] = {"200 OK", " 404 Not Found", "403 Forbidden"};

typedef enum con_type {PLAIN, HTML, XML, CSS, JAVASCRIPT, JPG, BMP, PNG} con_type_t;

char *mime_str[] = {
	"text/plain",
	"text/html",
	"text/xml",
	"text/css",
	"text/javascript",
	"image/jpeg",
	"image/bmp",
	"image/png"
};

ICACHE_FLASH_ATTR void sender_thread(void *args)
{
	multi_args_t *multiarg = (multi_args_t*)args;
	xQueueHandle *squeue = (xQueueHandle*) multiarg->arg1;
	xSemaphoreHandle *ssemaphore = (xSemaphoreHandle*) multiarg->arg2;
	queue_struct_t data;
	while(1)//mutex
	{
		
		if(pdTRUE == xQueueReceive(*squeue, &data, 10))
		{
			printf("\nSender_thread: getting from queue\n");
			while(pdFALSE == xSemaphoreTake(*ssemaphore, 10));

			{
				printf("Sender_thread: sending data\n");
				printf("Sender_thread: espconn_send return %d",espconn_send(data.espconn, data.data, data.size));
			}
			//xSemaphoreGive(sentFlagSemaphore); callback give it back
		}
	}
}


ICACHE_FLASH_ATTR void send_header(struct espconn *conn, status_t stat, con_type_t type, unsigned long length)
{
	queue_struct_t qstruct;
	qstruct.espconn = conn;
	sprintf(qstruct.data,"HTTP/1.1 %s\r\nContent-Length: %d\r\nContent-Type: %s\r\nConnection: Close\r\n\r\n\0", status_list[stat], length, mime_str[type]);
	//espconn_send(pespconn, temp_buff, strlen(temp_buff));
	qstruct.size = strlen(qstruct.data);
	xQueueSend(sendQueue, &qstruct, portMAX_DELAY);
}

ICACHE_FLASH_ATTR void send_data(struct espconn *conn, uint8_t *data, uint8_t size)
{
	queue_struct_t qstruct;
	qstruct.espconn = conn;
	memcpy((void*)qstruct.data, data, size);
	qstruct.size = size;
	xQueueSend(sendQueue, &qstruct, portMAX_DELAY);
}


const char * msg_welcome = "HELLO IT WORKS";
ICACHE_FLASH_ATTR static void data_recv_callback(void *arg, char *pdata, unsigned short len)
{
	//arg contains pointer to espconn struct
	struct espconn *pespconn = (struct espconn *) arg;

	printf("Received data: \"%s\"\n Length: %d\n", pdata, len);
	//printf("Ret sent: %d\n",espconn_sent(pespconn, (uint8*)msg_welcome, 4/*os_strlen(msg_welcome)*/));
	////////////DZIALA////printf("Ret send: %d\n",espconn_send(pespconn, (uint8*)msg_welcome, 3));
	//send_header(_200, PLAIN, 0);
	
	//char x[] = "\n<h1>TEST</h1>";
	//espconn_send(&espconn_struct, x, strlen(x));
	
	char* chr;
	char fname[50];
	chr = strchr(pdata, ' ');
	
	uint8_t i;
	for(i=0; i<sizeof(request_list)/sizeof(request_list[0]); i++){
		if(!strncmp(request_list[i], &pdata[0], chr-pdata))
			break;
	}
	
	unsigned int name_len = strchr(chr+1, ' ')-pdata;
	char *test = "\n<h1>TEST</h1>";
	switch(i){
		case GET:
			memcpy(&fname[0], chr+1, name_len);
			fname[name_len] = '\0';
			printf("\nNAME:%s\n", &fname[0]);
			
			if(strncmp("/favico",&fname[0], 7)){
				send_header(pespconn, _200, HTML, 13);
				//printf("Ret sent: %d\n",espconn_send(pespconn, test, 13));
				send_data(pespconn, test, 13);
				printf("\nPoszlo\n");
			}
				
			
		break;
		
		default:
			printf("\nRecv Callback: sending default\n");
			send_header(pespconn, _200, PLAIN, 0);
			break;
	}
	
}

ICACHE_FLASH_ATTR static void data_sent_callback(void *arg)
{
	printf("\nSend Callback: Data sent\n");
	if(pdTRUE == xSemaphoreGive(sentFlagSemaphore)){
		printf("\nSend Callback: semaphore released\n");
	}
	printf("\nSend Callback: cannot release semaphore\n");
}

ICACHE_FLASH_ATTR static void connect_callback(void *arg)
{
	struct espconn *pespconn = (struct espconn *)arg;
	printf("TCP connection established\n");
	espconn_regist_recvcb (pespconn, data_recv_callback);

    // espconn_regist_reconcb(pespconn, tcpserver_recon_cb);
    //espconn_regist_disconcb(pespconn, shell_tcp_disconcb);
    espconn_regist_sentcb(pespconn, data_sent_callback);
}


ICACHE_FLASH_ATTR sint8 start_server(void)
{
	tcp.local_port = 80;
	espconn_struct.type = ESPCONN_TCP;
	espconn_struct.state = ESPCONN_NONE;
	espconn_struct.proto.tcp = &tcp;
	//espconn_struct.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
    espconn_struct.proto.tcp->local_port = 80;

	if(espconn_regist_connectcb (&espconn_struct, connect_callback)) return (sint8)(-1);
	
	espconn_set_opt(&espconn_struct, ESPCONN_REUSEADDR | ESPCONN_NODELAY);
	//espconn_init();
	espconn_accept(&espconn_struct);
	espconn_regist_time(&espconn_struct, server_timeover, 0);
	//if(espconn_tcp_set_max_con(1)) return (sint8)(-3);

	return (sint8)0;
}
