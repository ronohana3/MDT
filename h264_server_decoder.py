import socket
import io
import av
import av.codec
import cv2
import threading

# Connection details
HOST = '172.16.212.67'
PORT_VIDEO = 9999
PORT_SERVER = 9998

# Global variables to store the latest frame
latest_frame = None
lock = threading.Lock()


# Function to receive video stream and decode frames
def receive_video_stream():
    global latest_frame
    codec = av.codec.context.CodecContext.create('h264', 'r')

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sVideo:
        sVideo.connect((HOST, PORT_VIDEO))

        while True:
            data = sVideo.recv(100000)
            if len(data) == 0:
                break

            for packet in codec.parse(data):
                for frame in codec.decode(packet):
                    img = frame.to_ndarray(format='bgr24')
                    img = cv2.resize(img, None, fx=0.5, fy=0.5)
                    # Update the latest frame with thread safety
                    with lock:
                        latest_frame = img.copy()


# Function to handle client requests and send the current frame
def handle_client(client_socket):
    global latest_frame
    while True:
        # Wait for a request (could be a simple "GET" command)
        request = client_socket.recv(1024).decode('utf-8')
        if not request:
            break

        if request.strip().lower() == 'get_frame':
            # Send the current frame
            with lock:
                if latest_frame is not None:
                    _, img_encoded = cv2.imencode('.jpg', latest_frame)
                    client_socket.sendall(len(img_encoded).to_bytes(4, 'big') + img_encoded.tobytes())
                else:
                    client_socket.sendall((0).to_bytes(4, 'big'))
        else:
            break

    client_socket.close()


# Function to start the server that waits for requests
def start_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind(('127.0.0.1', PORT_SERVER))
        server_socket.listen(5)
        print(f'Server listening on 127.0.0.1:{PORT_SERVER}')

        # while True:
        #     if latest_frame is not None:
        #         cv2.imshow("stream", latest_frame)
        #     cv2.waitKey(1)

        while True:
            client_socket, addr = server_socket.accept()
            print(f'Connection from {addr}')
            client_handler = threading.Thread(target=handle_client, args=(client_socket,))
            client_handler.start()


# Start the threads
threading.Thread(target=receive_video_stream, daemon=True).start()
start_server()
