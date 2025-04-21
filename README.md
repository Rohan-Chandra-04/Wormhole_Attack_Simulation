# Wormhole_Attack_Simulation

A basic implementation of two wormhole nodes (Wormhole-A and Wormhole-B) in Contiki-NG that use a manually set IPv6 address based on their node IDs. These nodes establish a private "tunnel" by exchanging packets between them, bypassing the normal RPL routing structure, even though they are part of the RPL DODAG.

The code creates a Contiki process that registers UDP sockets and listens for incoming packets.

If a packet is received:

It checks if the node is the intended final destination.

If not, it tries to forward the packet, potentially via a wormhole tunnel to another node.

This forms part of a wormhole attack, where malicious nodes (e.g., Node A and Node B) collude via an out-of-band channel to tunnel packets across the network, disrupting normal routing paths.
