import os, re, struct

ALPHABET = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
BOOKS_DIR = "../../resources/books"
books = os.listdir(BOOKS_DIR)
tetrafrequencies = [0]*26*26*26*26
probabilities = [0.0]*26*26*26*26

def get_probabilities():
    for book in books:
        raw_text = open(os.path.join(BOOKS_DIR, book), "r").read()
        text = re.sub('[^a-zA-Z]+', '', raw_text)
        text = text.upper()
        # print(text)
        for i in range(len(text) - 3):
            # print(i)
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
write_probabilities_bin("../../resources/probabilities.bin")

