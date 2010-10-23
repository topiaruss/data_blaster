#include <stdio.h>


#include <xccompat.h>
#include "xtcp_client.h"
#include <print.h>
#include "leds.h"
#include "buttons.h"
#include "xc2_firmware_config.h"
#include <string.h>
#include "dumper.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif
typedef unsigned char u8_t;
typedef unsigned int  u32_t;

#define NUM_HTTPD_CONNECTIONS 10

typedef struct dumper_state_t {
  int active;
  int conn_id;
} dumper_state_t;

dumper_state_t connection_states[NUM_HTTPD_CONNECTIONS];


void dumper_init(void)
{
  int i;
  for (i=0;i<NUM_HTTPD_CONNECTIONS;i++) {
    connection_states[i].active = 0;
  }
}


unsigned int parse_ip(char *ipstr) 
{
  char *c = ipstr + strlen(ipstr)-1;
  int error = 0;
  int curNum = 0;
  int ipaddr = 0;
  int ipshift = 24;
  int base = 1;
  while (c >= ipstr && !error) 
    {
      switch (*c) 
        {
        case '.':
          if (ipshift == 0)
            error = 1;
          else {
            base = 1;
            ipaddr += curNum << ipshift;
            curNum = 0;
            ipshift -= 8;
          }
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          curNum += (*c - 48)*base;
          base *= 10;
          break;
        default:
          error = 1;
          break;
        }
      c--;
    }

  if (ipshift != 0)
    error = 1;
  else {
    ipaddr += curNum;
  }


  if (error) {
    return 0;
  }

  return ipaddr;
}


static void handle_args(char *args, chanend led_svr, chanend page_svr) 
{
  if (strncmp(args,"pattern=",8) == 0) {
    switch (*(args+8)) 
      {
      case '1':
        led_pattern(led_svr, 1);
        break;
      case '2':
        led_pattern(led_svr, 2);
        break;
      }
  }
  if (strncmp(args,"ipaddr=",7) == 0) {
    // Set to static config with ip addr
    char *nextargpos;
    unsigned int ipaddr;
    unsigned int netmask;
    nextargpos = strchr(args, '&');    
    if (nextargpos != NULL) {
      *nextargpos=0;
      if (strncmp(nextargpos+1,"netmask=",8)==0) {
        //    printstr("Set to Static\n");
        ipaddr = parse_ip(args+7);

        netmask = parse_ip(nextargpos+9);
        
        if (ipaddr != 0) {
          //page_server_set_ipconfig(page_svr, 1);
          //          set_default_ip_config(config_ch,1);
          //page_server_set_ipaddr(page_svr, ipaddr);
         // page_server_set_netmask(page_svr, netmask);
          //          set_default_ip_addr(config_ch, ipaddr);
          //          set_default_netmask(config_ch, netmask);
          //          commit_config(config_ch);
          //          default_ip_config_cache = 1;
          //httpd_set_ip_addr_string(ipaddr, DEFAULT_IPADDR_STRING_NUM);
          //httpd_set_ip_addr_string(netmask, NETMASK_STRING_NUM);
        }
      }
    }
      
  }

}







void dumper_handle_send_request(chanend tcp_svr,
                               xtcp_connection_t *conn,
                               chanend page_svr,
                               chanend led_svr)
{
  struct dumper_state_t *hs = (struct dumper_state_t *) conn->appstate;
  unsigned char data[XTCP_CLIENT_BUF_SIZE];
  unsigned char *dptr = &data[0];
  int len;
  int maxlen;
/*
  if (hs->pss.cmdptr == 0) {
    xtcp_send(tcp_svr, data, 0);
    xtcp_close(tcp_svr, conn);
    return;
  }
*/
  switch (conn->event) 
    {    
    case XTCP_REQUEST_DATA:
      break;
    case XTCP_SENT_DATA:
      break;
    case XTCP_RESEND_DATA:
//      hs->pss = hs->saved_pss;
      break;
    }

//  hs->saved_pss = hs->pss;

  maxlen = conn->mss;
  len = 0;
  
  // fill up the data buffer to maxlen

  // prepare data to send

  xtcp_send(tcp_svr, data, len);

  return;
}

void dumper_init_state(chanend tcp_svr,
        xtcp_connection_t *conn)
{
  int i;

  for (i=0;i<NUM_HTTPD_CONNECTIONS;i++) {
    if (!connection_states[i].active) 
      break;    
  }
  
  if (i==NUM_HTTPD_CONNECTIONS) {
    printstr("Too many dumper connections!");
  }
  else {
    //    printstr("open ");
    //printintln(i);
    connection_states[i].active = 1;
    connection_states[i].conn_id = conn->id;
/*    connection_states[i].pss.cmdptr = 0;
    connection_states[i].pss.dptr = 0;
    connection_states[i].pss.saved_dptr = 0;
    connection_states[i].pss.bfcount = 0;
    connection_states[i].saved_pss = connection_states[i].pss;
*/

    xtcp_set_connection_appstate(tcp_svr, conn, (xtcp_appstate_t) &connection_states[i]);
  }
  return;
}
                      
void dumper_free_state(xtcp_connection_t *conn)
{
  int i;
  for (i=0;i<NUM_HTTPD_CONNECTIONS;i++)
    if (connection_states[i].conn_id == conn->id) {
      //      printstr("close ");
      //printintln(i);
      connection_states[i].active = 0;
    }
}


void old_dumper_handle_event(chanend tcp_svr,
                        xtcp_connection_t *conn,
                        chanend led_svr) 
{
  switch (conn->event) 
    {
    case XTCP_NEW_CONNECTION:
      dumper_init_state(tcp_svr, conn);
      break;
    case XTCP_RECV_DATA:
      //dumper_recv(tcp_svr, conn, page_svr, led_svr);
      break;
    case XTCP_SENT_DATA:
    case XTCP_REQUEST_DATA:
    case XTCP_RESEND_DATA:
      //dumper_handle_send_request(tcp_svr, conn, page_svr, led_svr);
      break;      
    case XTCP_CLOSED:
    case XTCP_TIMED_OUT:
    case XTCP_ABORTED:
      dumper_free_state(conn);
      break;
    }
}








