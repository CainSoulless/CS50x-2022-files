# TODO
'''
    Takes a text and then makes a Coleman test, knowing the Grade of
    the text typed.

    Coleman equation: index = 0.0588 * L - 0.296 * S - 15.8
    L = letterscount / wordscount * 100
    S = sentences / wordscount * 100
'''
# Global
words_count, letters_count, sentences_count, = 0, 0, 0


def counter(text):
    global words_count, letters_count, sentences_count
    i = 0

    # For that loops the number total of characteres on the sentence.
    while i < len(text):
        # Words counter.
        if text[i] == ' ' and text[i + 1] != ' ':
            words_count += 1
        # Recognize if the text variable has or not a sentence.
        elif (words_count == 0 and
                letters_count and
                text[i - 1] != ' '):
            words_count += 1
            if text[i] == "'":
                words_count -= 1
        # Letter count
        if text[i].isalpha():
            letters_count += 1
        # Sentences count
        if text[i] == '.' or text[i] == '?' or text[i] == '!':
            sentences_count += 1

        i += 1


def coleman(text):
    counter(text)

    # Coleman test
    L = letters_count / words_count * 100
    S = sentences_count / words_count * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    round_index = int(round(index))

    # Discrimination
    if round_index >= 16:
        print("Grade 16+")
    elif round_index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {round_index}")


def main():

    text = input("Text: ")

    coleman(text)


main()