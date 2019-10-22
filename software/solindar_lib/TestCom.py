import numpy as np 
import random
i=0

class testCom():
    def __init__(self,len_fifo,n_block):
        self.position = [x for x in range(0,len_fifo,1)]
        self.sonar = [x for x in range(0,len_fifo,1)]
        self.lidar = [x for x in range(0,len_fifo,1)]
        self.position_fifo=np.asarray(self.position)
        self.sonar_fifo=np.asarray(self.sonar)
        self.lidar_fifo=np.asarray(self.lidar)
        self.len_fifo = len_fifo
        self.n_block = n_block
    
    def update(self):
        global i
        #print('Update')
        for y in range (0,self.n_block,1):
            self.sonar[i] = random.randrange(10,80)
            self.sonar_fifo = np.asarray(self.sonar) #np.array([random.randrange(10,80)])
            #print(self.sonar_fifo)
            self.lidar_fifo[i] = random.randrange(10,80)
            i += 1
            if i == self.len_fifo:
                i = 0


