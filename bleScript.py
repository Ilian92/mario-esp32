import socket
from pynput import keyboard

ESP_IP = "192.168.4.1"
ESP_PORT = 8080

sock = socket.create_connection((ESP_IP, ESP_PORT), timeout=5)
sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
print("Connecte a l'ESP32 ! Appuie sur des touches (Echap pour quitter).")

pressed = set()
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

    if key in pressed:
        return
    pressed.add(key)

    sock.sendall(code)
    print("Envoye :", code)

def on_release(key):
    pressed.discard(key)
    if key == keyboard.Key.left:
        sock.sendall(b"l")
    elif key == keyboard.Key.right:
        sock.sendall(b"r")

with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()

sock.close()
print("Deconnecte.")