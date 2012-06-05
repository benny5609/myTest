/*
  This exmple program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/

#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_extras.lib")
#pragma comment(lib, "libevent_core.lib")

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <assert.h>

static const char MESSAGE[] = "Hello, World!\n";

static const int PORT = 3000;

static void listener_cb(struct evconnlistener *, evutil_socket_t,
    struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);


void do_accept(evutil_socket_t listener, short event, void *arg)
{
	struct event_base *base = (struct event_base *)arg;
	evutil_socket_t fd;
	struct sockaddr_in sin;
	int slen = sizeof(sin);
	fd = accept(listener, (struct sockaddr *)&sin, &slen);
	if (fd < 0) {
		perror("accept");
		int nLast = WSAGetLastError();
		return;
	}

	printf("%s\n", inet_ntoa(sin.sin_addr));
	printf("ACCEPT: fd = %u\n", fd);

	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);

	bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

int
main(int argc, char **argv)
{
	struct event_base *base;
// 	struct evconnlistener *listener;
	struct sockaddr_in sin;
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	struct event *signal_event;
	base = event_base_new();

	evutil_socket_t listener;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_listen_socket_reuseable(listener);

	
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("bind");
		return 1;
	}

	if (listen(listener, 5) < 0) {
		perror("listen");
		return 1;
	}

	printf ("Listening...\n");

	evutil_make_socket_nonblocking(listener);
	struct event *listen_event;
	listen_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
	event_add(listen_event, NULL);
// 	listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
// 	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
// 	    (struct sockaddr*)&sin,
// 	    sizeof(sin));
// 
// 	if (!listener) {
// 		fprintf(stderr, "Could not create a listener!\n");
// 		return 1;
// 	}

	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

	if (!signal_event || event_add(signal_event, NULL)<0) {
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}

	event_base_dispatch(base);

	//evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done\n");
	return 0;
}

static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
	struct event_base *base = (event_base *)user_data;
	struct bufferevent *bev;

	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);

	bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void conn_readcb(struct bufferevent* bev, void * user_data)
{
	#define MAX_LINE    256
	char line[MAX_LINE+1];
	int n;
	evutil_socket_t fd = bufferevent_getfd(bev);
	while (n = bufferevent_read(bev, line, MAX_LINE), n > 0) 
	{
		line[n] = '\0';
		printf("fd=%u, read line: %s\n", fd, line);
		bufferevent_write(bev, line, n);
	}
}

static void
conn_writecb(struct bufferevent *bev, void *user_data)
{
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		bufferevent_free(bev);
	}
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
		    strerror(errno));
	}else if(events & BEV_EVENT_CONNECTED)
	{
		printf("connected \n");
	}
	bufferevent_free(bev);
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (event_base *)user_data;
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}
