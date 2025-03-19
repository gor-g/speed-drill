import gradio as gr
import socket
import json
import matplotlib.pyplot as plt

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
        return process_curve(json.loads(response.decode()))

    except socket.timeout:
        print("No response from Arduino. Timed out.")
        return None

    finally:
        sock.close()

def process_curve(data: dict):
    inter_checkpoints_duration: float = data["INTER_CHECKPOINTS_DURATION"]/1000
    data_values: list[int] = [d/1000 for d in data["data"]]
    distance: float = data["distance"]

    # Create time values for the x-axis
    time_values = list(range(len(data_values)))

    # Create the plot
    plt.figure(figsize=(10, 5))
    plt.plot(time_values, [inter_checkpoints_duration * i for i in range(1, len(time_values) + 1)], label='Temps maximal entre les checkpoints', linestyle='--')
    plt.plot(time_values, data_values, label='Temps actuel', marker='o')
    plt.title(f'Temps entre les checkpoints pour un {distance} mètres')
    plt.xlabel('Checkpoint')
    plt.ylabel('Temps (en s)')
    plt.legend()
    plt.grid(True)

    # Save the plot to a file
    plot_path = 'curves.png'
    plt.savefig(plot_path)
    plt.close()

    return plot_path

# Create the Gradio interface
udp_iface = gr.Interface(
    fn=send_udp_message,
    inputs=[
        gr.Number(label="Distance à parcourir (mètres)"),
        gr.Number(label="Nombre de capteurs"),
        gr.Number(label="Temps (secondes)")
    ],
    outputs="text",
    title="Paramètres",
    description="Entrez une distance à parcourir en mètres et un temps en secondes pour envoyer un message UDP."
)

# Create another Gradio interface for displaying the curves
curve_iface = gr.Interface(
    fn=request_data,
    inputs=None,
    outputs="image",
    title="Afficher les résultats",
    description="Cliquez sur le bouton pour afficher les résultats."
)

# Launch the interfaces
app = gr.TabbedInterface([udp_iface, curve_iface], ["Paramétrage", "Afficher les résultats"])

app.launch()
