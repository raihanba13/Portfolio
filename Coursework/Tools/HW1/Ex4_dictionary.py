#!/usr/bin/python3

## Game of Life
## author:  Hannah Rittich, Holger Arndt
## version: 18.03.2022
## framework for sheet 1, exercise 4

import json

def main():

    dictionary = load_dictionary()

    # Implement here
    print(dictionary)
    
    while 1:
        try:
            cmd_select = int(input('Look up (1), List(2), New Entry (3), Delete Entry (4), Exit (0)?\n'))
        except:
            print('Please select properly!')
        
        if cmd_select == 0:
            print('Program closed.')
            break
        
        elif cmd_select == 1:
            search_word = input('What is the word?\n')
            translation = dictionary.get(search_word, False)
            if translation:
                print(search_word + '-> ' + translation)
            else:
                print('Word not found!')
        
        elif cmd_select == 2:
            for x in dictionary.keys():
                print(x + ' -> ' + dictionary[x])
                
        elif cmd_select == 3:
            word = input('What is the word?\n')
            translation = input('What is the translation?\n')
            dictionary[word] = translation
            save_dictionary(dictionary)
            
        elif cmd_select == 4:
            word = input('What is the word to delete?\n')
            try:
                del dictionary[word]
                save_dictionary(dictionary)
                print('Task completed successfully!')
            except:
                print('Word not found!')
                
        else:
            print('Please select properly!')

    save_dictionary(dictionary)

def load_dictionary():
    """Loads a dictionary from file and returns it."""

    try:
        with open('words.json', 'r') as fp:
            dictionary = json.load(fp)
    except:
        print('WARNING: No dictionary found.')
        dictionary = {}

    return dictionary

def save_dictionary(dictionary):
    """Saves a dictionary to a file."""

    with open('words.json', 'w') as fp:
        json.dump(dictionary, fp, indent = 2, sort_keys = True)

if __name__ == '__main__':
    main()
