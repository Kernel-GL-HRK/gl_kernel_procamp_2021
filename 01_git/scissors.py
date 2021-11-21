#!/usr/bin/env python3
import random
import hashlib

quit = 0
things_short = ['r', 'p', 's']
things = {'r':'rock', 'p':'paper', 's':'scissors'}
while quit == 0:
    game = 0;
    x = input('Please choose: rock (r) - paper (p) - scissors (s), quit(q)')
    player_choice = x.strip()
    my_choice = random.choice(things_short)
    print(hashlib.md5(my_choice.encode('utf-8')).hexdigest())
    if player_choice == 'q':
        break
    if player_choice in things_short:
        if my_choice == player_choice:
            game = 1
        elif (my_choice == 'p' and player_choice == 'r') or (my_choice == 'r' and player_choice == 's') or (my_choice == 's' and player_choice == 'p'):
            game = 2;
        print("You choose %s, I choose %s" % (things[player_choice], things[my_choice]))
        if game == 1:
            print('Standoff...')
        else:
            if game == 2:
                winner = 'I'
                game_things = [my_choice, player_choice]
            else:
                winner = 'You'
                game_things = [player_choice, my_choice]
            print("%s win: %s beats %s" % (winner, things[game_things[0]], things[game_things[1]]))

