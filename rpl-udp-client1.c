#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "sys/log.h"
#include "net/ipv6/uip-ds6.h"

#define LOG_MODULE "RPL-Client"
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

PROCESS(rpl_udp_client_process, "RPL UDP Client");
AUTOSTART_PROCESSES(&rpl_udp_client_process);

PROCESS_THREAD(rpl_udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static uip_ipaddr_t server_ip;
  uint8_t my_id = linkaddr_node_addr.u8[7];

  PROCESS_BEGIN();

  uip_ipaddr_t my_ip;
  set_ipaddr(&my_ip, my_id);
  uip_ds6_addr_add(&my_ip, 0, ADDR_MANUAL);

  LOG_INFO("Client IP: ");
  LOG_INFO_6ADDR(&my_ip);
  LOG_INFO_("\n");

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

  // Assuming server node has ID = 1
  set_ipaddr(&server_ip, 1);

  etimer_set(&periodic_timer, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    char msg[] = "Hello from client";
    LOG_INFO("Sending to server ");
    LOG_INFO_6ADDR(&server_ip);
    LOG_INFO_("\n");

    simple_udp_sendto(&udp_conn, msg, sizeof(msg), &server_ip);

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
