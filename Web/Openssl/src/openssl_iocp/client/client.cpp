// client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <atlbase.h>
#include <stdio.h>
#include "../shared/openssl_iocp.h"

struct session_data
{
	HANDLE event_connected;
	HANDLE event_closed;
};

void app_on_session_connect(session *psession)
{
	if(psession->status & CONNECTED)
	{
		printf("Connected to %s\n", psession->addresses_sz[REMOTE]);

		X509* cert = SSL_get_peer_certificate(psession->ssl);
		ssl_print_cert_info(cert);

		printf("Type message to be sent to server and press ENTER, empty message to exit\n");
	}
	else
		printf("Failed to connect %s\n", psession->addresses_sz[REMOTE]);

	SetEvent(((session_data*)psession->pdata)->event_connected);
}

void app_on_session_send(session *psession)
{
}

void app_on_session_recv(session *psession)
{
	printf("Received %d bytes from %s:", psession->ssl_buffer_size[RECV], psession->addresses_sz[REMOTE]);

	char buffer[BUFFER_SIZE+1] = {0};
	strncpy_s(buffer, psession->ssl_buffer[RECV], psession->ssl_buffer_size[RECV]);
	buffer[psession->ssl_buffer_size[RECV]] = 0;
	printf("%s\n", buffer);
}

void app_on_session_close(session *psession)
{
	printf("Session %s - %s is closed\n", psession->addresses_sz[LOCAL], psession->addresses_sz[REMOTE]);

	SetEvent(((session_data*)psession->pdata)->event_closed);

	// inject ctrl-break into console input to make program exit
	GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, 0);
}

session* app_create_session()
{
	session_data *pdata = new session_data;
	pdata->event_connected = CreateEvent(0, TRUE, FALSE, 0);
	pdata->event_closed = CreateEvent(0, TRUE, FALSE, 0);
	return session_new(pdata);
}

void app_delete_session(session *psession)
{
	session_data *pdata = (session_data*)psession->pdata;
	CloseHandle(pdata->event_connected);
	CloseHandle(pdata->event_closed);
	delete pdata;
	session_delete(psession);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ssl_init();

	if(argc >= 2)
	{
		sockaddr_storage addr = {0};
		sockaddr_in *paddrin = (sockaddr_in*)&addr;

		addrinfo hints = {0};
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		addrinfo *paddrinfo = 0;
		if(0 == getaddrinfo(CW2A(argv[1]), "", &hints, &paddrinfo))
		{
			for(addrinfo *p = paddrinfo; p != 0; p = p->ai_next)
			{
				memcpy_s(&addr, sizeof(addr), p->ai_addr, p->ai_addrlen);
				break;
			}
			freeaddrinfo(paddrinfo);

			paddrin->sin_port = htons(443);
			if(argc == 3)
			{
				int port = _wtol(argv[2]);
				if(port > 0 && port < 65535)
					paddrin->sin_port = htons(port);
			}

			// create a new session object and initiate connection
			session *psession = app_create_session();
			session_connect(psession, &addr);
			
			// wait for connection to finish (either succeeded or failed)
			WaitForSingleObject(((session_data*)psession->pdata)->event_connected, INFINITE);
			
			while(true)
			{
				// connection has been closed (either by us or server), exit from loop
				if (WAIT_OBJECT_0 == WaitForSingleObject(((session_data*)psession->pdata)->event_closed, 0))
					break;

				char data[BUFFER_SIZE];
				gets_s(data);

				int len = strlen(data);
				//data[len++] = 13;
				//data[len++] = 10;
				data[len] = 0;

				// send input string to server
				session_send_data(psession, data, len);
			}

			// wait for connection to be closed and all pending operations have finished or been cancelled
			WaitForSingleObject(((session_data*)psession->pdata)->event_closed, INFINITE);

			// free memory and resources
			app_delete_session(psession);
		}
		else
		{
			printf("Unable to resolve server %S\n", argv[1]);
		}
	}
	else
	{
		printf("Usage: %S <server> [<port>]\n", argv[0]);
	}

	ssl_deinit();
	return 0;
}

