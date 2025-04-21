#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "sys/log.h"
#include "net/ipv6/uip-ds6.h"

#define LOG_MODULE "RPL-Server"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;

static void
set_ipaddr(uip_ipaddr_t *ipaddr, uint8_t node_id)
{
  uint16_t seg_6 = 0x7400 + node_id;
  uint16_t seg_8 = (0x100 * node_id) + node_id;
  uip_ip6addr(ipaddr, 0xfd00, 0, 0, 0, 0x212, seg_6, node_id, seg_8);
}

static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  LOG_INFO("Received '%.*s' from ", datalen, (char *)data);
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");

  simple_udp_sendto(&udp_conn, data, datalen, sender_addr);
}

PROCESS(rpl_udp_server_process, "RPL UDP Server");
AUTOSTART_PROCESSES(&rpl_udp_server_process);

PROCESS_THREAD(rpl_udp_server_process, ev, data)
{
  uip_ipaddr_t my_ip;
  uint8_t node_id = linkaddr_node_addr.u8[7];

  PROCESS_BEGIN();

  set_ipaddr(&my_ip, node_id);
  uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);

  LOG_INFO("Server IP: ");
  LOG_INFO_6ADDR(&my_ip);
  LOG_INFO_("\n");

  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, udp_rx_callback);

  PROCESS_END();
}

// #include "contiki.h"
// #include "net/netstack.h"
// #include "net/ipv6/simple-udp.h"
// #include "net/routing/routing.h"
// #include "sys/log.h"
// #include "net/ipv6/uip-ds6.h"

// #define LOG_MODULE "RPL-Server"
// #define LOG_LEVEL LOG_LEVEL_INFO

// #define UDP_CLIENT_PORT 8765
// #define UDP_SERVER_PORT 5678

// static struct simple_udp_connection udp_conn;

// static void
// set_ipaddr(uip_ipaddr_t *ipaddr, uint8_t node_id)
// {
//   uint16_t seg_6 = 0x7400 + node_id;
//   uint16_t seg_8 = (0x100 * node_id) + node_id;
//   uip_ip6addr(ipaddr, 0xfd00, 0, 0, 0, 0x212, seg_6, node_id, seg_8);
// }

// static void
// detect_routing_loop(uint8_t ttl, const uip_ipaddr_t *sender_addr)
// {
//   static uint8_t prev_ttl = 255; // Set to a high number initially
//   static uip_ipaddr_t prev_addr;

//   // Compare the current TTL with the previous one
//   if(ttl == prev_ttl && uip_ipaddr_cmp(sender_addr, &prev_addr)) {
//     LOG_WARN("Routing loop detected from: ");
//     LOG_INFO_6ADDR(sender_addr);
//     LOG_INFO_("\n");
//   }

//   // Update the previous TTL and sender address
//   prev_ttl = ttl;
//   uip_ipaddr_copy(&prev_addr, sender_addr);
// }

// static void
// udp_rx_callback(struct simple_udp_connection *c,
//                 const uip_ipaddr_t *sender_addr,
//                 uint16_t sender_port,
//                 const uip_ipaddr_t *receiver_addr,
//                 uint16_t receiver_port,
//                 const uint8_t *data,
//                 uint16_t datalen)
// {
//   LOG_INFO("Received '%.*s' from ", datalen, (char *)data);
//   LOG_INFO_6ADDR(sender_addr);
//   LOG_INFO_("\n");

//   // Extract TTL from the packet or from other headers
//   uint8_t ttl = 255; // Dummy TTL value; you may want to extract this from the actual packet
//   detect_routing_loop(ttl, sender_addr);

//   // Send the received data back to the sender
//   simple_udp_sendto(&udp_conn, data, datalen, sender_addr);
// }

// PROCESS(rpl_udp_server_process, "RPL UDP Server");
// AUTOSTART_PROCESSES(&rpl_udp_server_process);

// PROCESS_THREAD(rpl_udp_server_process, ev, data)
// {
//   uip_ipaddr_t my_ip;
//   uint8_t node_id = linkaddr_node_addr.u8[7];

//   PROCESS_BEGIN();

//   set_ipaddr(&my_ip, node_id);
//   uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);

//   LOG_INFO("Server IP: ");
//   LOG_INFO_6ADDR(&my_ip);
//   LOG_INFO_("\n");

//   NETSTACK_ROUTING.root_start();

//   simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, udp_rx_callback);

//   PROCESS_END();
// }
