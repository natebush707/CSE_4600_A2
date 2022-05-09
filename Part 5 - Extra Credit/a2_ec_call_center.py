"""
a2_ec_call_center.py
execute: python a2_ec_call_center.py

Nathan Bush 007463099
CSE 4600(01) - CSUSB S22

Assignment 2, Part 4 (Call Center Extra Credit)
Documentation used: https://docs.python.org/3/library/threading.html
"""

from threading import BoundedSemaphore, Thread
from time import sleep
from random import randint

# thread class that stores name of caller and call function
class callThread(Thread):
    def __init__(self, name):
        Thread.__init__(self)
        self.name = name
    def run(self):
        customer_call(self.name)

# thread function with critical section
def customer_call(name):
    print(f'{name} is waiting to speak to the operator...')

    # hold music if no available operators
    if sem._value == 0:
        print('All operators are currently busy assisting other callers - MUSIC ...')
    
    # critical section
    with sem:
        print(f'{name} is getting connection to the operator ...')
        sleep(randint(3,10))
        print(f'{name} is now ending the conversation with the operator ...')
    
    # report available operators after releasing semaphore
    print(f'Available operators = {sem._value}')

# initialize thread data    
connection_count = 3
sem = BoundedSemaphore(connection_count)
customers = ['Alice', 'Bob', 'John', 'Mark', 'Alex']
threads = [callThread(customer) for customer in customers]

# report initial state then run threads
print(f'Initial operator availability = {sem._value}')

for thread in threads:
    thread.start()

# join threads once finished
for thread in threads:
    thread.join()

print('All calls finished successfully, exiting main thread.')