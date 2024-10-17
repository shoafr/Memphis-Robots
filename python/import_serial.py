import requests
from pynput.keyboard import Key, Listener

# Replace with the IP address of the Arduino Access Point
arduino_ip = "http://192.48.56.2"  # This is the default IP of the Arduino as an Access Point

# Function to send commands to the robot
def send_command(command):
    try:
        url = f"{arduino_ip}/{command}"
        
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Command '{command}' sent successfully!")
        else:
            print(f"Failed to send command. Status code: {response.status_code}")
    except Exception as e:
        print(f"Error: {e}")

# Function to handle key presses
def on_press(key):
    try:
        if key == Key.up:
            send_command('forward')
        elif key == Key.down:
            send_command('backward')
        elif key == Key.left:
            send_command('left')
        elif key == Key.right:
            send_command('right')
    except AttributeError:
        pass

# Function to handle key releases
def on_release(key):
    if key in [Key.up, Key.down, Key.left, Key.right]:
        send_command('stop')  # Send stop command when the key is released
    
    # Exit the program when the ESC key is pressed
    if key == Key.esc:
        return False

# Start listening for key presses
def main():
    print("Control the robot with the arrow keys. Press ESC to exit.")
    
    # Listen for keyboard events using pynput
    with Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()

if __name__ == "__main__":
    main()
