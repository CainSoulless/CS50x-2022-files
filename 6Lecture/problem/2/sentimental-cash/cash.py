from cs50 import get_float


def get_cash():
    while True:
        try:
            cash = get_float("Change owed: ")
            if cash > 0:
                break
        except ValueError:
            pass
    return cash * 100


def calculate_quarters(cash):
    return int(cash / 25)


def calculate_dimes(cash):
    return int(cash / 10)


def calculate_nickels(cash):
    return int(cash / 5)


def calculate_pennies(cash):
    return int(cash / 1)


# Ask how many cash the customer id owed
cash = get_cash()

# Calculate the number of quarters
quarters = calculate_quarters(cash)
cash = cash - quarters * 25

# Calculate the number of dimes
dimes = calculate_dimes(cash)
cash = cash - dimes * 10

# Calculate the number of nickels
nickels = calculate_nickels(cash)
cash = cash - nickels * 5

# Calculate the number of pennies
pennies = calculate_pennies(cash)
cash = cash - pennies * 1

# The amount of money are added to change var.
change = quarters + dimes + nickels + pennies

print(change)