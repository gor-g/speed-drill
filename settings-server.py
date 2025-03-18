import socket

dest_ip = '192.168.4.1'
udp_port = 8989
range_in_centimeters = 123  # Example value

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send the message
message = str(range_in_centimeters).encode()  # Convert to bytes
sock.sendto(message, (dest_ip, udp_port))

print(f'Message "{range_in_centimeters}" sent to {dest_ip}:{udp_port}')

# Close the socket
sock.close()
