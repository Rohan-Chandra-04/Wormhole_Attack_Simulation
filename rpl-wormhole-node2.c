// // wormhole-b.c
// #include "contiki.h"
// #include "net/ipv6/simple-udp.h"
// #include "net/netstack.h"
// #include "net/routing/routing.h"
// #include "sys/log.h"
// #include "utils.h"

// #define LOG_MODULE "Wormhole-B"
// #define LOG_LEVEL LOG_LEVEL_INFO
// #define UDP_PORT 1234

// static struct simple_udp_connection udp_conn;
// PROCESS(wormhole_node_b_process, "Wormhole Node B");
// AUTOSTART_PROCESSES(&wormhole_node_b_process);

// static void
// udp_rx_callback(struct simple_udp_connection *c,
//                 const uip_ipaddr_t *sender_addr,
//                 uint16_t sender_port,
//                 const uip_ipaddr_t *receiver_addr,
//                 uint16_t receiver_port,
//                 const uint8_t *data,
//                 uint16_t datalen)
// {
//   LOG_INFO("Received tunnel data from ");
//   LOG_INFO_6ADDR(sender_addr);
//   LOG_INFO_(": %.*s\n", datalen, (char *)data);
// }

// PROCESS_THREAD(wormhole_node_b_process, ev, data)
// {
//   uip_ipaddr_t my_ip;

//   PROCESS_BEGIN();

//   set_ipaddr(&my_ip, 20); // Node ID = 20
//   uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);
//   LOG_INFO("My IP is ");
//   LOG_INFO_6ADDR(&my_ip);
//   LOG_INFO_("\n");

//   simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
//   PROCESS_END();
// }

// wormhole-b.c
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/routing/routing.h"
#include "sys/log.h"
//#include "utils.h"

#define LOG_MODULE "Wormhole-B"
#define LOG_LEVEL LOG_LEVEL_INFO
#define UDP_PORT 1234

static struct simple_udp_connection udp_conn;

static void set_ipaddr(uip_ipaddr_t *ipaddr, uint8_t node_id) {
  uip_ip6addr(ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, node_id);
}

PROCESS(wormhole_node_b_process, "Wormhole Node B");
AUTOSTART_PROCESSES(&wormhole_node_b_process);

static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  LOG_INFO("[Tunnel RX] Received tunnel data from ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO(": %.*s\n", datalen, (char *)data);
}

PROCESS_THREAD(wormhole_node_b_process, ev, data)
{
  uip_ipaddr_t my_ip;

  PROCESS_BEGIN();

  LOG_INFO("Initializing Wormhole Node B...\n");

  set_ipaddr(&my_ip, 20); // Node ID = 20
  uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);
  LOG_INFO("My IP is ");
  LOG_INFO_6ADDR(&my_ip);
  LOG_INFO("\n");

  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);

  PROCESS_END();
}
