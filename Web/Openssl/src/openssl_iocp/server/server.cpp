// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>
#include <stdio.h>

#include "cert.h"
#include "../shared/openssl_iocp.h"

void app_on_session_connect(session *psession)
{
	printf("Connected from %s\n", psession->addresses_sz[REMOTE]);
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

	// upper-case the data and send them back to client
	_strupr_s(buffer);
	session_send_data(psession, buffer, strlen(buffer));
}

void app_on_session_close(session *psession)
{
	printf("Session %s - %s is closed and will be deleted\n", psession->addresses_sz[LOCAL], psession->addresses_sz[REMOTE]);

	// whent this function is called, all pending operations associated with the psession have either finished or been cancelled.
	// the framework won't touch psession anymore so it is safe to delete psession here directly.
	session_delete(psession);
}

// create a listening socket and associate it with iocp
SOCKET create_listen_socket(int port)
{
	sockaddr_storage addr = {0};
	sockaddr_in *paddrin = (sockaddr_in*)&addr;
	addr.ss_family = AF_INET;
	paddrin->sin_port = htons(port);
	paddrin->sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET s = WSASocket(addr.ss_family, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	iocp_associate_handle((HANDLE)s);
	bind(s, (sockaddr*)&addr, sizeof(addr));
	listen(s, SOMAXCONN);
	return s;
}

// load server certificate and RSA private key from memory and use them on glocal SSL_CTX object.
void set_cert()
{
	int length = strlen(server_cert_key_pem);
	BIO *bio_cert = BIO_new_mem_buf((void*)server_cert_key_pem, length);
	X509 *cert = PEM_read_bio_X509(bio_cert, nullptr, nullptr, nullptr);
	printf("Certificate used for server:\n");
	ssl_print_cert_info(cert);
	EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio_cert, 0, 0, 0);
	ssl_set_ctx_cert_and_key(cert, pkey);
	X509_free(cert);
	EVP_PKEY_free(pkey);
	BIO_free(bio_cert);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ssl_init();
	set_cert();

	int port = 65000;
	if(argc == 2)
	{
		int port_arg = _wtol(argv[1]);
		if(port_arg > 0 && port_arg < 65535)
			port = port_arg;
	}

	SOCKET s = create_listen_socket(port);
	
	// initiate accept. once an incoming connection is accepted, a new session will be created 
	// automatically and passed to another call to session_accept(). the framework will accept 
	// multiple clients automatically. see implementation of session_on_accept(). after the 
	// connection was closed, memory and resource will be freeed in app_on_session_close().
	session *psession = session_new(0);
	psession->s_listening = s;
	session_accept(psession);

	printf("\nServer is now running and listening on port %d, press any key to exit\n", port);
	getchar();

	shutdown(s, SD_BOTH);
	closesocket(s);
	ssl_deinit();
	return 0;
}
