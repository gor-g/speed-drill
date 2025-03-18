import gradio as gr
import socket


DEST_IP = '192.168.4.1'
UDP_PORT = 8989

def send_udp_message(distance_meters, checkpoints, time_seconds):

    # Create the message with the encapsulated values
    message = f"{distance_meters},{time_seconds},{checkpoints}"

    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Send the message
    sock.sendto(message.encode(), (DEST_IP, UDP_PORT))

    print(f'Message "{message}" sent to {DEST_IP}:{UDP_PORT}')

    # Close the socket
    sock.close()

    return f'Message "{message}" sent to {DEST_IP}:{UDP_PORT}'


def request_data():

    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(5)  # Timeout for receiving data


    try:
        # Send message to Arduino
        message = "-2".encode()
        sock.sendto(message, (DEST_IP, UDP_PORT))
        print(f'Sent: {message} to {DEST_IP}:{UDP_PORT}')

        # Listen for a response
        response, addr = sock.recvfrom(1024)  # Buffer size of 1024 bytes
        print(f'Received from {addr}: {response.decode()}')

    except socket.timeout:
        print("No response from Arduino. Timed out.")

    finally:
        sock.close()

# Create the Gradio interface
iface = gr.Interface(
    fn=send_udp_message,
    inputs=[
        gr.Number(label="Distance à parcourir (mètres)"),
        gr.Number(label="Nombre de capteurs"),
        gr.Number(label="Temps (secondes)")
    ],
    outputs="text",
    title="Envoyer un message UDP",
    description="Entrez une distance à parcourir en mètres et un temps en secondes pour envoyer un message UDP."
)

request_data()
# Launch the interface
iface.launch()
