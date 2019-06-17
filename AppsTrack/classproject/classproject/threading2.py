import threading
import time

counter = 0

def update():
    global counter
    for i in range(10):
        counter += 1

def activity(tag):
    thread_id = threading.current_thread().ident
    for i in range(5):
        print("hello from", tag, "i =", i, "thread_id =", thread_id)
        update()
        time.sleep(1)

def worker1():
    activity("worker1")

def worker2():
    activity("worker2")

def main():
    thread1 = threading.Thread(target=worker1)
    thread2 = threading.Thread(target=worker2)

    thread1.start()
    thread2.start()

if __name__ == '__main__':
    global counter
    main()
    print(counter)