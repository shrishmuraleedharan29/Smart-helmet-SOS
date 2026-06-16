import os
import time

# Update this path to your specific logs folder
DIRECTORY_TO_WATCH = './logs'
LOG_FILE = 'serial.txt'
TRIGGER_KEYWORD = 'ALARM TRIGGER'

def get_latest_file(directory):
    """Finds the most recently created file in the directory."""
    files = [os.path.join(directory, f) for f in os.listdir(directory) 
             if os.path.isfile(os.path.join(directory, f))]
    if not files:
        return None
    return max(files, key=os.path.getctime)

def watch_logs():
    last_file = None
    print(f"Monitoring folder: {DIRECTORY_TO_WATCH}...")

    while True:
        current_latest = get_latest_file(DIRECTORY_TO_WATCH)
        
        # If a new file is created, switch to it
        if current_latest and current_latest != last_file:
            print(f"Detected new file: {current_latest}")
            last_file = current_latest

        if last_file and os.path.exists(last_file):
            with open(last_file, 'r') as f:
                # Seek to end to monitor only new entries
                f.seek(0, 2) 
                while True:
                    line = f.readline()
                    if not line:
                        # Periodically check if a newer file exists
                        if get_latest_file(DIRECTORY_TO_WATCH) != last_file:
                            break
                        time.sleep(0.1)
                        continue
                    
                    if TRIGGER_KEYWORD in line:
                        with open(LOG_FILE, 'a') as serial:
                            serial.write(f"TRIGGER DETECTED: {line.strip()} at {time.ctime()}\n")
                            print("Match found! Added to serial.txt")
        
        time.sleep(1)

if __name__ == "__main__":
    watch_logs()
