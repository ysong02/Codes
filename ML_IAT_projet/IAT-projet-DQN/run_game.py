from time import sleep
from game.SpaceInvaders import SpaceInvaders
from controller.keyboard import KeyboardController
from controller.random_agent import RandomAgent
from controller.dqn_agent import DQN

def main():

    #controller = KeyboardController()
    #controller = RandomAgent(game.na)
    controller = DQN()
    
    controller.learn()

    game = SpaceInvaders(display=True)
    state = game.reset()
    for i in range(100000):
        action = controller.select_action(state,1)
        state, reward, is_done = game.step(action)
        sleep(0.0001)
        if is_done:
            break
if __name__ == '__main__' :
    main()
