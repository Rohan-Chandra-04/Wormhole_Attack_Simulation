// // wormhole-a.c
// #include "contiki.h"
// #include "net/ipv6/simple-udp.h"
// #include "net/netstack.h"
// #include "net/routing/routing.h"
// #include "sys/log.h"
// #include "utils.h"

// #define LOG_MODULE "Wormhole-A"
// #define LOG_LEVEL LOG_LEVEL_INFO
// #define UDP_PORT 1234

// static struct simple_udp_connection udp_conn;
// PROCESS(wormhole_node_a_process, "Wormhole Node A");
// AUTOSTART_PROCESSES(&wormhole_node_a_process);

// PROCESS_THREAD(wormhole_node_a_process, ev, data)
// {
//   static struct etimer periodic_timer;
//   uip_ipaddr_t my_ip, tunnel_peer;

//   PROCESS_BEGIN();

//   set_ipaddr(&my_ip, 10); // Node ID = 10
//   uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);
//   LOG_INFO("My IP is ");
//   LOG_INFO_6ADDR(&my_ip);
//   LOG_INFO_("\n");

//   NETSTACK_ROUTING.root_start();

//   simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

//   // Set Wormhole-B IP manually
//   set_ipaddr(&tunnel_peer, 20); // Node ID = 20

//   etimer_set(&periodic_timer, CLOCK_SECOND * 10);
//   while(1) {
//     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
//     const char *msg = "Wormhole packet via tunnel";
//     simple_udp_sendto(&udp_conn, msg, strlen(msg), &tunnel_peer);
//     LOG_INFO("Forwarded to tunnel peer: ");
//     LOG_INFO_6ADDR(&tunnel_peer);
//     LOG_INFO_("\n");
//     etimer_reset(&periodic_timer);
//   }

//   PROCESS_END();
// }

// wormhole-a.c
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/routing/routing.h"
#include "sys/log.h"
//#include "utils.h"

#define LOG_MODULE "Wormhole-A"
#define LOG_LEVEL LOG_LEVEL_INFO
#define UDP_PORT 1234

static struct simple_udp_connection udp_conn;

static void set_ipaddr(uip_ipaddr_t *ipaddr, uint8_t node_id) {
  uip_ip6addr(ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, node_id);
}

PROCESS(wormhole_node_a_process, "Wormhole Node A");
AUTOSTART_PROCESSES(&wormhole_node_a_process);

PROCESS_THREAD(wormhole_node_a_process, ev, data)
{
  static struct etimer periodic_timer;
  uip_ipaddr_t my_ip, tunnel_peer;

  PROCESS_BEGIN();

  LOG_INFO("Initializing Wormhole Node A...\n");

  set_ipaddr(&my_ip, 10); // Node ID = 10
  uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);
  LOG_INFO("My IP is ");
  LOG_INFO_6ADDR(&my_ip);
  LOG_INFO("\n");

  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  // Set Wormhole-B IP manually
  set_ipaddr(&tunnel_peer, 20); // Node ID = 20

  etimer_set(&periodic_timer, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    const char *msg = "Wormhole packet via tunnel";
    simple_udp_sendto(&udp_conn, msg, strlen(msg), &tunnel_peer);
    LOG_INFO("[Tunnel TX] Forwarded to tunnel peer: ");
    LOG_INFO_6ADDR(&tunnel_peer);
    LOG_INFO("\n");
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
