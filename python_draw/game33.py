import numpy as np
import pandas as pd
import time

np.random.seed(2)  # reproducible

N_STATES = 6   # the length of the 1 dimensional world
ACTIONS = ['left', 'right']     # available actions
EPSILON = 0.9   # greedy police
ALPHA = 0.1     # learning rate
GAMMA = 0.9    # discount factor
MAX_EPISODES = 13   # maximum episodes
FRESH_TIME = 0.3    # fresh time for one move


def build_q_table(n_states, actions):
    table = pd.DataFrame(
        np.zeros((n_states, len(actions))),     # q_table initial values
        columns=actions,    # actions's name
    )
    # print(table)    # show table
    return table


def choose_action(state, q_table):
    # This is how to choose an action
    state_actions = q_table.iloc[state, :]
    if (np.random.uniform() > EPSILON) or ((state_actions == 0).all()):  # act non-greedy or state-action have no value
        action_name = np.random.choice(ACTIONS)
    else:   # act greedy
        action_name = state_actions.idxmax()    # replace argmax to idxmax as argmax means a different function in newer version of pandas
    return action_name


def get_env_feedback(S, A):
    # This is how agent will interact with the environment
    if A == 'right':    # move right
        if S == N_STATES - 2:   # terminate 只有到达终点才有奖励
            S_ = 'terminal'
            R = 1
        else:
            S_ = S + 1
            R = 0
    else:   # move left走左边的奖励永远是0
        R = 0
        if S == 0:
            S_ = S  # reach the wall
        else:
            S_ = S - 1
    return S_, R


def update_env(S, episode, step_counter):
    # This is how environment be updated
    env_list = ['-']*(N_STATES-1) + ['T']   # '---------T' our environment
    if S == 'terminal':
        interaction = 'Episode %s: total_steps = %s' % (episode+1, step_counter)
        print('\r{}'.format(interaction), end='')
        time.sleep(2)
        print('\r                                ', end='')
    else:
        env_list[S] = 'o'
        interaction = ''.join(env_list)
        print('\r{}'.format(interaction), end='')
        time.sleep(FRESH_TIME)


def rl():
    # main part of RL loop
    q_table = build_q_table(N_STATES, ACTIONS)  # 创建一个全为0的q表，用的是pandas
    for episode in range(MAX_EPISODES):         # 主循环，看一共需要训练多少次，也就是一共成功找到宝藏多少次
        step_counter = 0
        S = 0
        is_terminated = False
        update_env(S, episode, step_counter)    # 在环境中更新状态
        while not is_terminated:                # 如果没找到宝藏就要一直找

            A = choose_action(S, q_table)       # 通过q表来选择 下一步的动作
            S_, R = get_env_feedback(S, A)      # 用这个动作来走，并且得到他的奖励
            q_predict = q_table.loc[S, A]       # 得到q表的预测值，也就是走这一步之前会先看看不走也就是不更新q表会是什么情况 其实也就是前一个状态执行这个动作的q值
            if S_ != 'terminal':
                q_target = R + GAMMA * q_table.iloc[S_, :].max()   # GAMMA是衰减率 乘上实际应该走的q值里最大的
            else:
                q_target = R     # next state is terminal
                is_terminated = True    # terminate this episode

            q_table.loc[S, A] += ALPHA * (q_target - q_predict)  # update 更新的值实际上是q_predict在q表中的值，也就是上一个状态执行这个动作的q值
            S = S_  # move to next state

            update_env(S, episode, step_counter+1)
            step_counter += 1
    return q_table


if __name__ == "__main__":
    q_table = rl()
    print('\r\nQ-table:\n')
    print(q_table)
