import gradio as gr
import socket

def send_udp_message(distance_meters, checkpoints, time_seconds):
    dest_ip = '192.168.4.1'
    udp_port = 8989

    # Create the message with the encapsulated values
    message = f"{distance_meters},{time_seconds},{checkpoints}"

    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Send the message
    sock.sendto(message.encode(), (dest_ip, udp_port))

    print(f'Message "{message}" sent to {dest_ip}:{udp_port}')

    # Close the socket
    sock.close()

    return f'Message "{message}" sent to {dest_ip}:{udp_port}'

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

# Launch the interface
iface.launch()
