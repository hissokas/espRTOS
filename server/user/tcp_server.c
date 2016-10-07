#include "tcp_server.h"

static struct espconn espconn_struct;
static esp_tcp tcp;

static void data_recv_callback(void *arg, char *pdata, unsigned short len)
{
	//arg contains pointer to espconn struct
	printf("Received data: \"%s\"\n Length: %d\n", pdata, len);
}

static void connect_callback(void *arg)
{
	printf("TCP connection established\n");
}

sint8 start_server(void)
{
	tcp.local_port = 80;
	espconn_struct.type = ESPCONN_TCP;
	espconn_struct.proto.tcp = &tcp;

	if(espconn_regist_connectcb (&espconn_struct, connect_callback)) return (sint8)(-1);
	if(espconn_regist_recvcb (&espconn_struct, data_recv_callback)) return (sint8)(-2);

	espconn_init();
	espconn_accept(&espconn_struct);

	//if(espconn_tcp_set_max_con(1)) return (sint8)(-3);

	return (sint8)0;
}
