import socket
from pynput import keyboard

ESP_IP = "192.168.4.1"
ESP_PORT = 8080

sock = socket.create_connection((ESP_IP, ESP_PORT), timeout=5)
sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
print("Connecte a l'ESP32 ! Appuie sur des touches (Echap pour quitter).")


def on_press(key):
    if key == keyboard.Key.esc:
        return False

    if key == keyboard.Key.left:
        code = b"L"
    elif key == keyboard.Key.right:
        code = b"R"
    elif key == keyboard.Key.up:
        code = b"U"
    elif key == keyboard.Key.down:
        code = b"D"
    elif key == keyboard.Key.enter:
        code = b"E"
    else:
        return

    sock.sendall(code)
    print("Envoye :", code)

with keyboard.Listener(on_press=on_press) as listener:
    listener.join()

sock.close()
print("Deconnecte.")