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
    inter_checkpoints_duration: float = data["INTER_CHECKPOINTS_DURATION"]
    data_values: list[int] = data["data"]

    # Create a plot
    plt.figure(figsize=(10, 5))

    # Plot the first curve
    plt.subplot(1, 2, 1)
    plt.plot(data_values, label='Data Values')
    plt.title('Data Values Over Time')
    plt.xlabel('Time')
    plt.ylabel('Values')
    plt.legend()

    # Plot the second curve (example: cumulative sum of data values)
    plt.subplot(1, 2, 2)
    plt.plot(range(len(data_values)), [sum(data_values[:i+1]) for i in range(len(data_values))], label='Cumulative Sum')
    plt.title('Cumulative Sum of Data Values')
    plt.xlabel('Time')
    plt.ylabel('Cumulative Sum')
    plt.legend()

    # Save the plot to a file
    plt.tight_layout()
    plt.savefig('curves.png')
    plt.close()

    return 'curves.png'

# Create the Gradio interface
udp_iface = gr.Interface(
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

# Create another Gradio interface for displaying the curves
curve_iface = gr.Interface(
    fn=request_data,
    inputs=None,
    outputs="image",
    title="Afficher les courbes",
    description="Cliquez sur le bouton pour afficher les courbes."
)

# Launch the interfaces
app = gr.TabbedInterface([udp_iface, curve_iface], ["Paramétrage", "Afficher les résultats"])

app.launch()
