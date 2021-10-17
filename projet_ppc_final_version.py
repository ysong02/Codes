from multiprocessing import Value, Array, Queue, Event, Process, Lock
import time
import random
from random import randint
import os
import signal



# days counter
calender = 0


SIGNALS = [signal.SIGUSR1, signal.SIGUSR2, signal.SIGVTALRM]

class Home(object):
    def __init__(self, product_rate, consume_rate, storage, type, exit_flag, synchronization, deficit_flag, taker_queue,weather_param):
        self.product_rate = product_rate
        self.consume_rate = consume_rate
        self.weather_param = weather_param
        self.money = random.randint(10, 1000)
        self.storage = storage
        self.type = type
        self.exit_flag = exit_flag
        self.synchronization = synchronization
        self.market_queue = Queue()
        self.deficit_flag = deficit_flag
        self.taker_queue = taker_queue

    def run(self):         
        if self.consume_rate < 0:
            self.consume_rate = 0
        while not self.exit_flag.value:
            self.synchronization.wait()
            #calculation of new consumption rate related
            self.consume_rate = self.consume_rate - self.weather_param.value/2
            #print(self.weather_param.value)
            self.storage = self.product_rate - self.consume_rate + self.storage + random.uniform(-10,20)

            # storage shortage
            if self.storage < 50:
                self.deficit_flag.set()

                time.sleep(0.5)
                #take energy from other homes
                while (not self.taker_queue.empty()) and (self.storage <= 50):
                    transaction_home = self.taker_queue.get()
                    print('[Home process {}] takes {} energy.'.format(os.getpid(), transaction_home.get('energy')), flush=True)
                    self.storage += transaction_home.get('energy')
                if self.storage >= 50:
                    self.deficit_flag.clear()
                #if not enough then buy energy in market
                if self.storage < 50:
                    self.request = 50 - self.storage
                    self.storage = 50
                    print('[Home process {}] buys {} energy from the market.'.format(os.getpid(), self.request), flush=True)
                    self.deficit_flag.clear()

            #homes have spare energy        
            else:
                amount_sell = self.storage - 50 
                #type1:give to market
                if self.type == 1:
                    self.taker_queue.put({'type': 1, 'energy': amount_sell})
                    print('[Home process {}] gives away {} energy.'.format(os.getpid(), amount_sell), flush=True)
                #type2:always sell to market
                elif self.type == 2:
                    self.market_queue.put({'type': 2, 'energy': amount_sell})
                    print('[Home process {}] sells {} energy.'.format(os.getpid(), amount_sell), flush=True)
                #type3:Firstly give energy to homes who need energy, and sell the rest
                elif self.type == 3:
                    if self.deficit_flag.is_set():
                        self.taker_queue.put({'type': 3, 'energy': amount_sell})
                        print('[Home process {}] gives away {} energy.'.format(os.getpid(), amount_sell), flush=True)                  
                    else:
                        self.market_queue.put({'type': 3, 'energy': amount_sell})
                        print('[Home process {}] sells {} energy.'.format(os.getpid(), amount_sell), flush=True)
                        
                self.storage = 50
                time.sleep(0.5)
                if self.type in [2, 3]:
                    while not self.market_queue.empty():
                        transaction_market = self.market_queue.get()
                        print('[Home process {}] earns {} money from the market.'.format(os.getpid(), transaction_market.get('money')), flush=True)
                        self.money += transaction_market.get('money')
        print('[Home Process {}] ends.'.format(os.getpid()))


class Weather(object):
    def __init__(self, exit_flag, synchronization, verrou,shared_memory):
        self.shared_memory = shared_memory  
        self.exit_flag = exit_flag
        self.synchronization = synchronization
        self.verrou = verrou


    def run(self):
        while not self.exit_flag.value:
            #lock the shared memory when weather is written, avoid the incoherence            
            self.synchronization.wait()
            verrou.acquire()
            self.temp_change = random.uniform(-8,8)
            self.shared_memory.value = self.shared_memory.value + self.temp_change
            if self.shared_memory.value > 40:
                self.shared_memory.value = 40
            if self.shared_memory.value < -20:
                self.shared_memory.value = - 20
            verrou.release()
        print('Weather Process ends.')


class Market(object):
    def __init__(self, exit_flag, weather_param, synchronization, ques):
        #variables
        self.exit_flag = exit_flag
        self.weather_param = weather_param
        self.synchronization = synchronization
        self.last_price = 0.5 
        self.gama = 0.99
        self.alpha1 = 0.001
        self.diplomatic_tension = 0
        self.beta1 = 0.01
        self.war = 0
        self.beta2 = 0.02
        self.money_change_rate = 0
        self.beta3 = 0.01
        self.ques = ques

        #handlers of signal when events happens

        signal.signal(SIGNALS[0], self.set_diplomatic_tension)
        signal.signal(SIGNALS[1], self.set_war)
        signal.signal(SIGNALS[2], self.set_money_change_rate)
   
    #deal with different events if they happened
    def set_diplomatic_tension(self, s, frame):
        self.diplomatic_tension = 1
        print("diplomatic tension happens!")

    def set_war(self, s, frame):
        self.war = 1
        print('world war happens!')

    def set_money_change_rate(self, s,frame):
        self.money_change_rate = 1
        print('Money change rate happens!')

    def run(self):
        while not self.exit_flag.value:
            self.synchronization.set()
            time.sleep(0.05)
            self.synchronization.clear()

            #calculation of new price
            #print(self.weather_param.value)
            current_price = self.last_price * self.gama + 1.0 / self.weather_param.value * self.alpha1 + \
                        self.diplomatic_tension * self.beta1 + self.war * self.beta2 + self.money_change_rate * self.beta3

            for queue in self.ques:
                if not queue.empty():
                    d = queue.get()
                    queue.put({'money': d.get('energy') * current_price})
            global calender
            print('[price for DAY {}], yesterday price {:.3f} €/kW, current price {:.3f} €/kW.'.format(calender+1,self.last_price, current_price), flush=True)
            self.last_price = current_price

            #global calender
            calender += 1
            if calender >= MAX_COUNT:
                self.exit_flag.value = 1
            
            time.sleep(1)
            self.synchronization.set()

        print('Market Process ends.')
      

class Politics(object):
    def __init__(self, exit_flag, market_pid,synchronization):
        self.exit_flag = exit_flag
        self.marked_pid = market_pid
        self.synchronization = synchronization

    def run(self):
        synchronization.wait()
        while not self.exit_flag.value:           
            # diplomatic tension
            if random.random() <= 0.2:
                os.kill(self.marked_pid, SIGNALS[0])
            # war
            if random.random() <= 0.1:
                os.kill(self.marked_pid, SIGNALS[1])

            time.sleep(2)

        print('Politics Process ends.')


class Economics(object):
    def __init__(self, exit_flag, market_pid,synchronization):
        self.exit_flag = exit_flag
        self.marked_pid = market_pid
        self.synchronization = synchronization

    def run(self):
        synchronization.wait()
        while not self.exit_flag.value:            
            # money change rate
            if random.random() <= 0.15:
                os.kill(self.marked_pid, SIGNALS[2])

            time.sleep(1)

        print('Economics Process ends.')




if __name__ == '__main__':
    exit_flag = Value('i', 0)
    shared_memory = Value('f',random.uniform(-10,30)) 
    synchronization = Event()
    deficit_flag = Event()
    taker_queue = Queue(1024)
    verrou = Lock()

    weather = Weather(exit_flag, synchronization,verrou,shared_memory)
    weather_param = weather.shared_memory
    weather_process = Process(target=weather.run)
    weather_process.start()
    MAX_COUNT = int(input("enter the number of days: "))
    home_num = int(input("enter the number of homes: "))
    homes = []
    home_processes = []
    for i in range(home_num):
        home = Home(randint(1, 50), randint(1, 50), randint(0, 100), randint(1, 3), exit_flag, synchronization, deficit_flag, taker_queue, weather_param)
        home_process = Process(target=home.run)
        home_process.start()
        homes.append(home)
        home_processes.append(home_process)

    print('-'*100)
    print('basic information')
    for i in range(home_num):
        print('home {}: product_rate {}, consume_rate {}, type {}, storage {}'.format(i, homes[i].product_rate,
                                                                                      homes[i].consume_rate,
                                                                                      homes[i].type,
                                                                                      homes[i].storage))
    print('-'*100)

    market = Market(exit_flag, weather_param, synchronization, [home.market_queue for home in homes])

    politics = Politics(exit_flag, os.getpid(),synchronization)
    politics_process = Process(target=politics.run)
    politics_process.start()

    economics = Economics(exit_flag, os.getpid(),synchronization)
    economics_process = Process(target=economics.run)
    economics_process.start()

    market.run()

    weather_process.join()
    politics_process.join()
    economics_process.join()
    for home_process in home_processes:
        home_process.join()
