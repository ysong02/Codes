import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from .epsilon_profile import EpsilonProfile
from game.SpaceInvaders import SpaceInvaders
		
class QAgent(object):
	def __init__(self, eps_profile:EpsilonProfile, gamma:float, alpha:float):
		self.Q = np.zeros((80,40,30,2,4))
		
		self.env = SpaceInvaders(display = False)
		
		self.n_actions = 4
		
		self.gamma = gamma
		self.alpha = alpha
		
		self.eps_profile = eps_profile
		self.epsilon = self.eps_profile.initial
		
		self.qvalues = pd.DataFrame(data={'episode':[],'value':[]})
		#self.values = pd.DataFrame(data={'player':[192],'invader':[192],'bullet':[2]})
		self.values = pd.DataFrame(data={'player':[80],'invaderX':[40],'invaderY':[30],'bullet':[2]})
	def learn(self, n_episodes, max_steps):
		sum_score = np.zeros(n_episodes)
		av_score = np.zeros(int(n_episodes/10))
		for episode in range(n_episodes):
			state = self.env.reset()
			for step in range(max_steps):
				action = self.select_action(state)
				next_state, reward, terminal = self.env.step(action)
				
				self.updateQ(state, action, reward, next_state)
				sum_score[episode] += reward
				av_score[int(episode/10)] += reward
				if terminal:
					break
				state = next_state
			print('episode:', episode, '|score:', sum_score[episode],'|epsilon:', self.epsilon)
			self.epsilon = max(self.epsilon - self.eps_profile.dec_episode/(n_episodes-30.), self.eps_profile.final)
		fig = plt.figure()
		plt.plot(sum_score)
		plt.xlabel('episode')
		plt.ylabel('score')
		plt.show()
	def updateQ(self, state, action, reward, next_state):
		[s0,s1,s2,s3] = state
		[sn0,sn1,sn2,sn3] = next_state
		if (s0<80) and (s1<40) and (s2<30) and (sn0<80) and (sn1<40) and (sn2<30):
			self.Q[s0][s1][s2][s3][action] = (1.-self.alpha) * self.Q[s0][s1][s2][s3][action] + self.alpha*(reward + self.gamma * np.max(self.Q[sn0][sn1][sn2][sn3]))
	
	def select_action(self, state):
		[s0,s1,s2,s3] = state
		if np.random.rand() < self.epsilon:
			a = np.random.randint(self.n_actions)
		else:
			if (s0>=80) or (s1>=40) or (s2>=30):
				a = np.random.randint(self.n_actions)
			else:
				#print(state)
				mx = np.max(self.Q[s0][s1][s2][s3])
				a = np.random.choice(np.where(self.Q[s0][s1][s2][s3]==mx)[0])
		return a
