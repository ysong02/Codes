from time import sleep
from game.SpaceInvaders import SpaceInvaders
from controller.keyboard import KeyboardController
from controller.random_agent import RandomAgent
from controller.q_agent import QAgent
from controller.epsilon_profile import EpsilonProfile

def main():

    
    #controller = KeyboardController()
    #controller = RandomAgent(game.na)
    
    eps_profile = EpsilonProfile()
    controller = QAgent(eps_profile = eps_profile, gamma=1, alpha=0.2)
    controller.learn(100, 100000)
    print('test start!')
    game = SpaceInvaders(display=True)
    state = game.reset()
    sum_reward = 0
    for step in range(100000):
        action = controller.select_action(state)
        state, reward, is_done = game.step(action)
        sleep(0.0001)
        sum_reward += reward
        if is_done:
            break
    print(sum_reward)
if __name__ == '__main__' :
    main()
