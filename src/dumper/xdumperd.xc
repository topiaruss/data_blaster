#include "xtcp_client.h"
#include "dumper.h"
#include <xs1.h>
#include <print.h>
#include "ethernet_tx_client.h"
//#include "mdns.h"

void printconn(xtcp_connection_t conn){

	  printstr("conn id"); printintln(conn.id);
	  printstr("conn prot"); printintln(conn.protocol);
	  printstr("conn event"); printintln(conn.event);
	  printstr("conn remote");
      printhex(conn.remote_addr[3]); printstr(".");
      printhex(conn.remote_addr[2]); printstr(".");
      printhex(conn.remote_addr[1]); printstr(".");
      printhex(conn.remote_addr[0]); printstrln("");
	  printstr("conn remport"); printintln(conn.remote_port);
	  printstr("conn locport"); printintln(conn.local_port);

}

void xdumperd(chanend tcp_svr,
            chanend led_svr) 
{
	  xtcp_connection_t conn;
	  xtcp_appstate_t state;
	  xtcp_ipaddr_t host = {192,168,2,166};
	  unsigned char data[XTCP_CLIENT_BUF_SIZE] = {'h', 'e', 'l', 'l', 'o'};
#define DATA_LEN 6
#define DELAY 200000
	  int len;
	  int rem_port = DUMP_SERVER_PORT;
	  timer tmr;
	  unsigned t;
	  unsigned ledOn = 1;

	  xtcp_connect(tcp_svr, rem_port, host, XTCP_PROTOCOL_UDP);
	  //xtcp_bind_remote(tcp_svr, conn, host, rem_port);

	  //httpd_init();
	//  xtcp_listen(tcp_svr, 80, XTCP_PROTOCOL_TCP);
	  xtcp_ask_for_event(tcp_svr);
	  while(1) {
	    select
	      {
	      case xtcp_event(tcp_svr, conn):
	            switch (conn.event) {
	               case XTCP_NEW_CONNECTION:
	                  printstr("XTCP_NEW_CONNECTION\n");
	                  //dumper_init_state(tcp_svr, conn);
	                  printconn(conn);
	                  xtcp_init_send(tcp_svr, conn);
	                  break;
	               case XTCP_RECV_DATA:
	                  printstr("XTCP_RECV_DATA\n");
	                  //httpd_recv(tcp_svr, conn);
	                 len = xtcp_recv(tcp_svr, data);
	                 printstr("received "); printstrln(data);
	                  break;
	               case XTCP_REQUEST_DATA:
	                  printstr("XTCP_REQUEST_DATA\n");
	                  xtcp_send(tcp_svr, data, DATA_LEN);
	                  tmr :> t;   /* save the current timer value */
	                  break;
	               case XTCP_RESEND_DATA:
	                  printstr("XTCP_RESEND_DATA\n");
	                  xtcp_send(tcp_svr, data, DATA_LEN);
	                  tmr :> t;   /* save the current timer value */
	                  break;
	               case XTCP_SENT_DATA:
	//                  printstr("sending...\n");
	                tmr when timerafter(t + DELAY) :> void;        /* wait till the send period is over */
	                //x0ledB <: ledOn;  /* toggle the LED */
	                //ledOn = !ledOn;
	                     xtcp_send(tcp_svr, data, DATA_LEN);
	                     tmr :> t;   /* save the current timer value */
	                  //httpd_send(tcp_svr, conn);
	                  break;
	               case XTCP_TIMED_OUT:
	               case XTCP_ABORTED:
	               case XTCP_CLOSED:
	                  printstr("XTCP_TIMED_OUT\n");
	                  //dumper_free_state(conn);
	                  break;
	            }
	        xtcp_ask_for_event(tcp_svr);
	        break;
	      }
	  }

}
