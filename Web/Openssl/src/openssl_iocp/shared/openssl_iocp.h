#pragma once

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/x509v3.h"

typedef CRITICAL_SECTION	ssl_lock;

enum OVERLAPPED_TYPE{
	RECV = 0,
	SEND,
	CONNECT
};

enum ADDRESS_TYPE{
	LOCAL = 0,
	REMOTE
};

enum SOCKET_STATUS{
	NONE		= 0x0,
	ACCEPTING	= 0x1,
	CONNECTING	= 0x2,
	HANDSHAKING	= 0x4,
	CONNECTED	= 0x8,
	RECEIVING	= 0x10,
	SENDING		= 0x20,
	CLOSING		= 0x40,
	CLOSED		= 0x80,
	OPERATING	= ACCEPTING | CONNECTING | HANDSHAKING | RECEIVING | SENDING
};

#define ADDR_SZ_SIZE	46
#define BUFFER_SIZE		1024

struct session;

struct session_overlapped
{
	OVERLAPPED overlapped;
	DWORD result;
	session *psession;
};

struct session
{
	SOCKET s; // handle to socket
	SOCKET s_listening; // handle to external listening socket
	sockaddr_storage addresses[2]; // local and remote address
	char addresses_sz[2][ADDR_SZ_SIZE]; // addresses in string format
	char socket_buffer[2][BUFFER_SIZE]; // memory used for read/write from/to socket
	char ssl_buffer[2][BUFFER_SIZE]; // memory used for read/write from/to ssl memory bio
	DWORD ssl_buffer_size[2]; // indicates the bytes of valid data in ssl_buffer
	unsigned int status; // stores current socket status, bit-masked value of one or more of SOCKET_STATUS
	session_overlapped overlapped[3]; // structure for overlapped operations
	WSABUF wsabuf[2]; // structure used for pass buffer to overlapped operations
	DWORD bytes_transferred[2]; // store the bytes of buffer that received/sent from/to the socket
	DWORD wsa_flags[2]; // store the flags send/receive from overlapped operations, not used
	SSL *ssl; // SSL structure used by OpenSSL
	BIO *bio[2]; // memory BIO used by OpenSSL
	ssl_lock lock; // synchronization object for multiple-thread data access
	void *pdata; // user supplied contextual data, not used by openssl processing
};

void iocp_associate_handle(HANDLE h);

void ssl_init();
void ssl_set_ctx_cert_and_key(X509 *cert, EVP_PKEY *pkey);
void ssl_deinit();
void ssl_print_cert_info(X509 *cert);

session* session_new(void *pdata);
void session_delete(session *psession);
void session_lock(session *psession);
void session_unlock(session *psession);
void session_connect(session *psession, const sockaddr_storage *remote_addr);
void session_accept(session *psession);
int session_send_data(session *psession, const char *data, int len);
void session_close(session *psession);
bool session_process(session *psession);

// callback functions which are called under the protection of the psession->lock.
extern void app_on_session_connect(session *psession);
extern void app_on_session_send(session *psession);
extern void app_on_session_recv(session *psession);

// callback function which is called when psession is no longer in use and be safely
// deleted by calling session_delete().
extern void app_on_session_close(session *psession);