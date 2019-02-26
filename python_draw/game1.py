# -*- coding: UTF-8 -*-

if __name__ == '__main__':
    print('开始学习')

    import gym

    env = gym.make('Ant-v2')
    env.reset()
    for _ in range(100000):
        env.render()
        env.step(env.action_space.sample())  # take a random action