import threading
import time

counter = 0

def wathcer():
    global counter
    for i in range(20):
        print("watcher value of counter is", counter)
        time.sleep(0.1)

def update(lock):
    global counter
    name = threading.current_thread().name
    with lock:
        for i in range(10):
            counter += 1
            time.sleep(0.1)
    time.sleep(0.2)
    print("value of counter is", counter)

def main():
    global counter
    threads = []
    for i in range(5):
        thread = threading.Thread(target=update, name=f'worker%d' % i)
        threads.append(thread)
        thread.start()

    threadWorker = threading.Thread(target=wathcer, name='watcher')
    threadWorker.start()
    threadWorker.join()
    print("final value of counter is", counter)

if __name__ == '__main__':
    main()
