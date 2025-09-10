import os
import struct

ALPHABET = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
BOOKS_DIR = "../../resources/books"
books = os.listdir(BOOKS_DIR)
tetrafrequencies = [0]*26*26*26*26
probabilities = [0.0]*26*26*26*26

#TODO: implement removing non-alpha chars but to an array of words cuz dont wanna count the frequencies of sets with letters from 2 different words

def get_probabilities():
    for book in books:
        text = open(os.path.join(BOOKS_DIR, book), "r").read()
        for i in range(len(text) - 3):
            # print(text[i])
            x = (ALPHABET.index(text[i])*26*26*26 +ALPHABET.index(text[i+1])*26*26 +
            ALPHABET.index(text[i+2])*26 +
            ALPHABET.index(text[i+3]))
            tetrafrequencies[x] += 1
        for i in range(26*26*26*26):
            probabilities[i] = tetrafrequencies[i] / (len(text)-3)

def write_probabilities_bin(filename):
    with open(filename, 'wb') as f:
        for prob in probabilities:
            f.write(struct.pack('d', prob))

get_probabilities()
# write_probabilities_bin("../../resources/probabilities.bin")

