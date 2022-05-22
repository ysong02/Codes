import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
import matplotlib.pyplot as plt
from game.SpaceInvaders import SpaceInvaders

# Hyper Parameters
BATCH_SIZE = 32
BATCH_SIZE2 = 16
LR = 0.01                   # learning rate
EPSILON = 1.              # greedy policy
EPS_FINAL = 0.1
EPS_ITER = 30
GAMMA = 0.95                 # reward discount
TARGET_REPLACE_ITER = 100   # target update frequency
MEMORY_CAPACITY = 2000
MEMORY_CAPACITY2 = 50
#env = gym.make('CartPole-v0')
#env = env.unwrapped
env = SpaceInvaders(display = False)
N_ACTIONS = 4
N_STATES = 5
#ENV_A_SHAPE = 0 if isinstance(env.action_space.sample(), int) else env.action_space.sample().shape     # to confirm the shape


class Net(nn.Module):
    def __init__(self, ):
        super(Net, self).__init__()
        self.fc1 = nn.Linear(N_STATES, 128)
        self.fc1.weight.data.normal_(0, 0.1)   # initialization
        self.fc2 = nn.Linear(128,128)
        self.fc2.weight.data.normal_(0, 0.1)
        self.out = nn.Linear(128, N_ACTIONS)
        self.out.weight.data.normal_(0, 0.1)   # initialization

    def forward(self, x):
        x = self.fc1(x)
        x = F.relu(x)
        x = self.fc2(x)
        x = F.relu(x)
        actions_value = self.out(x)
        return actions_value


class DQN(object):
    def __init__(self):
        self.eval_net, self.target_net = Net(), Net()

        self.learn_step_counter = 0                                     # for target updating
        self.memory_counter = 0
        self.memory_counter_shoot = 0                                         # for storing memory
        self.memory = np.zeros((MEMORY_CAPACITY, N_STATES * 2 + 2))     # initialize memory
        self.memory_shoot = np.zeros((MEMORY_CAPACITY2, N_STATES * 2 + 2))
        self.optimizer = torch.optim.Adam(self.eval_net.parameters(), lr=LR)
        self.loss_func = nn.MSELoss()
        self.epsilon = EPSILON

    def select_action(self, x,test=0):
        x = torch.unsqueeze(torch.FloatTensor(x), 0)
        # input only one sample
        if np.random.uniform() > self.epsilon:   # greedy
           
            actions_value = self.eval_net.forward(x)
            #print(actions_value)
            if test == 1:
                print(actions_value)
            action = torch.max(actions_value, 1)[1].data.numpy()[0]
            #action = action[0] if ENV_A_SHAPE == 0 else action.reshape(ENV_A_SHAPE)  # return the argmax index
        else:   # random
            action = np.random.randint(0, N_ACTIONS)
            #action = action if ENV_A_SHAPE == 0 else action.reshape(ENV_A_SHAPE)
        return action

    def store_transition(self, shoot, s, a, r, s_):
        transition = np.hstack((s, [a, r], s_))
        # replace the old memory with new memory
        if shoot == 0:
            index = self.memory_counter % MEMORY_CAPACITY
            self.memory[index, :] = transition
            self.memory_counter += 1
        else:
            index = self.memory_counter_shoot % MEMORY_CAPACITY2
            self.memory_shoot[index, :] = transition
            self.memory_counter_shoot += 1

    def updateQ(self):
        # target parameter update
        if self.learn_step_counter % TARGET_REPLACE_ITER == 0:
            self.target_net.load_state_dict(self.eval_net.state_dict())
        self.learn_step_counter += 1

        # sample batch transitions
        sample_index0 = np.random.choice(MEMORY_CAPACITY, BATCH_SIZE)
        
        b_memory0 = self.memory[sample_index0, :]
        b_s0 = torch.FloatTensor(b_memory0[:, :N_STATES])
        b_a0 = torch.LongTensor(b_memory0[:, N_STATES:N_STATES+1].astype(int))
        b_r0 = torch.FloatTensor(b_memory0[:, N_STATES+1:N_STATES+2])
        b_s_0 = torch.FloatTensor(b_memory0[:, -N_STATES:])
        
        sample_index1 = np.random.choice(MEMORY_CAPACITY2, BATCH_SIZE2)
        b_memory1 = self.memory_shoot[sample_index1, :]
        b_s1 = torch.FloatTensor(b_memory1[:, :N_STATES])
        b_a1 = torch.LongTensor(b_memory1[:, N_STATES:N_STATES+1].astype(int))
        b_r1 = torch.FloatTensor(b_memory1[:, N_STATES+1:N_STATES+2])
        b_s_1 = torch.FloatTensor(b_memory1[:, -N_STATES:])

        b_s = torch.vstack((b_s0, b_s1))
        b_a = torch.vstack((b_a0, b_a1))
        b_r = torch.vstack((b_r0, b_r1))
        b_s_ = torch.vstack((b_s_0, b_s_1))
        
        sample_index = np.random.choice(BATCH_SIZE+BATCH_SIZE2,BATCH_SIZE)
        b_s = b_s[sample_index, :]
        b_a = b_a[sample_index, :]
        b_r = b_r[sample_index, :]
        b_s_ = b_s_[sample_index, :]
        #print(b_r)
        
        # q_eval w.r.t the action in experience
        q_eval = self.eval_net(b_s).gather(1, b_a)  # shape (batch, 1)
        q_next = self.target_net(b_s_).detach()     # detach from graph, don't backpropagate
        q_target = b_r + GAMMA * q_next.max(1)[0].view(BATCH_SIZE, 1)   # shape (batch, 1)
        loss = self.loss_func(q_eval, q_target)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

    def learn(self):
        print('\nCollecting experience...')
        s_shoot = np.zeros(N_STATES)
        sn_shoot = np.zeros(N_STATES)
        a_shoot = 0
        r_shoot = 0
        for i_episode in range(50):
            s = env.reset()
            ep_r = 0
            score = 0
            for i in range(100000):
                a = self.select_action(s)
                bullet_state = env.get_bullet_state()
                # take action
                s_, reward, done = env.step(a)
                r = reward
                # modify the reward
                           
                invader_Y = env.get_invaders_Y()[-1]
                invader_X = env.get_invaders_X()[-1]
                player_X = env.get_player_X()
                bullet_X = env.get_bullet_X()
                bullet_Y = env.get_bullet_Y()
                #reward_shoot = 0
                reward_threshold = 0
                #if reward == 1:
                #    reward_shoot = 1
                if player_X > 100 and player_X < 700:
                    reward_threshold = 0.
                elif player_X <= 100:
                    reward_threshold = (player_X-100)/100
                else:
                    reward_threshold = (700-player_X)/100
                #reward1 = 0.01 - 0.01*invader_Y / 600
                reward2 = 1-abs(player_X - invader_X)/800
                #reward3 = 0.01 - 0.01*abs(bullet_X - invader_X)/800
                reward = reward + reward_threshold + reward2
                
                if bullet_state == 'rest' and a == 2:
                    s_shoot = s
                    sn_shoot = s_
                    a_shoot = 2
                    r_shoot = reward
                else:
                    self.store_transition(r,s, a, reward, s_)
                
                if r == 1:
                    self.store_transition(1,s_shoot,a_shoot,r_shoot+1,s_shoot)
                    
                ep_r += reward
                score += r
                if self.memory_counter > MEMORY_CAPACITY and self.memory_counter_shoot > MEMORY_CAPACITY2:
                    self.updateQ()
                
                if done:
                    break
                s = s_
            print('Ep: ', i_episode,'| Ep_r: ', round(ep_r, 2),'| Score: ', score)
            self.epsilon = max(EPS_FINAL, self.epsilon-(1./EPS_ITER))

	
	
	
	
	
	
	
	
	
	
	
