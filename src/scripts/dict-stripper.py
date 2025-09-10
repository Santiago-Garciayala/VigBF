import re 

dict = open("/usr/share/dict/words", "r")
dict_only_alpha = open("../../resources/dictionaries/dict_only_alpha", "w")
dict_only_alpha.close()
dict_only_alpha = open("../../resources/dictionaries/dict_only_alpha", "a")

for line in dict:
    line = line.lower()
    line = re.sub(r'[^a-z]', '', line) 
    dict_only_alpha.write(line + "\n")


dict.close()
dict_only_alpha.close()
