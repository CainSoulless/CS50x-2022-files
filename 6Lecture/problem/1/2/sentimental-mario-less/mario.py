# TODO

n = 0

# Check if the user introduced a property height
while True:
    try:
        n = int(input("Height: "))
        if n >= 1 and n <= 8:
            break
    except:
        pass


# First for loop create all the schemes of the matrix
for i in range(n):
    # Second for loop implement the spaces
    for j in range(n - i - 1):
        print(" ", end="")

    # Third for loop implement the blocks
    for f in range(i + 1):
        print("#", end="")

    print("", end="\n")
