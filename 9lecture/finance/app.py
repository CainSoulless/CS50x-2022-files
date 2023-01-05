import os
import requests

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
#if not os.environ.get("API_KEY"):
#    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Getting user wallet
    user_wallet = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    user_wallet = user_wallet[0]['cash']

    # Updating current value
    database = db.execute("SELECT * FROM purchanse WHERE id_username = ? ;", session['user_id'])
    total_adds = 0

    for symbol in database:
        get_lookup = lookup(symbol['symbol'])
        total = get_lookup['price'] * symbol['shares']
        total_adds += total
        db.execute("UPDATE purchanse SET current_value = ?, total = ? WHERE id_username = ? AND symbol = ?;",
                   get_lookup['price'], total, session['user_id'], get_lookup['symbol'])

    database = db.execute("SELECT * FROM purchanse WHERE id_username = ?;", session['user_id'])

    print(database)
    return render_template("profile.html", database=database, user_wallet=usd(user_wallet), total_adds=usd(total_adds + user_wallet))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    elif request.method == "POST":
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Insert valid symbol", 400)
        symbol = request.form.get("symbol")
        user_wallet = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Check if the user provided a valid input.
        get_share = lookup(symbol)

        if not shares or not shares or not symbol or shares < 1:
            return apology("Insert valid shares", 400)
        if not symbol or not get_share:
            return apology("Insert valid symbol", 400)

        investment = get_share['price'] * float(shares)

        # users.cash update
        if user_wallet[0]['cash'] > investment:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", (float(user_wallet[0]['cash'] - investment)), session["user_id"])
        else:
            return apology("Not enough money", 402)

        # purchanse table inserting values.
        query = db.execute("SELECT shares, symbol FROM purchanse WHERE symbol = ? AND id_username = ?",
                           symbol.upper(), session['user_id'])

        if not query:
            db.execute("INSERT INTO purchanse(shares, current_value, symbol, id_username, company_name) VALUES (?, ?, ?, ?, ?);",
                       shares, investment, symbol.upper(), session["user_id"], get_share['name'])
        else:
            db.execute("UPDATE purchanse SET shares = ? WHERE id_username = ? AND symbol = ?;",
                       query[0]['shares'] + int(shares), session["user_id"], symbol)

        # Updating history database
        db.execute("INSERT INTO history(symbol, shares, price, id_username) VALUES (?, ?, ?, ?);",
                   symbol, shares, investment, session["user_id"])

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    database = db.execute("SELECT * FROM history WHERE id_username = ?;", session['user_id'])

    return render_template("history.html", database=database)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")
#

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        get_lookup = lookup(symbol)

        if not get_lookup or not symbol:
            return apology("Quote not allowed", 400)
        else:

            return render_template("quoted.html", symbol=symbol.upper(), company_name=get_lookup['name'], share_cost=usd(get_lookup['price']))

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password_again = request.form.get("confirmation")
        users = db.execute("SELECT username FROM users WHERE username = ?;", username)

        # The user must provide a valid username
        if not username or users:
            return apology("must provide valid username", 400)

        elif not password or password_again != password:
            return apology("must provide valid password", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, generate_password_hash(password))

        return render_template("login.html")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbol_user = db.execute("SELECT symbol,shares FROM purchanse WHERE id_username = ?;", session["user_id"])

    if request.method == "GET":
        options = [x['symbol'] for x in symbol_user]
        return render_template("sell.html", options=options)

    shares = int(request.form.get("shares"))
    symbol = request.form.get("symbol")

    # Check if the user introduced proper input.
    if (len(symbol_user) != 1 or not symbol or
        shares > symbol_user[0]['shares'] or
            not symbol in symbol_user[0]['symbol'] or shares < 1):

        return apology("Symbol or shares not valid", 400)

    # Data querying
    get_lookup = lookup(symbol)
    current_shares = db.execute('SELECT shares FROM purchanse WHERE id_username = ? AND symbol = ?;',
                                session["user_id"], symbol)
    current_wallet = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])

    # Update the database.
    db.execute("UPDATE users SET cash = ? WHERE id = ?;", current_wallet[0]['cash'] + get_lookup['price'] * shares,
               ksession["user_id"])

    # Check if the actual update could be removed from the database.
    try:
        shares_updated = current_shares[0]["shares"] - int(shares)
    except:
        shares_updated = 0

    if shares_updated > 0:
        db.execute("UPDATE purchanse SET shares = ? WHERE id_username = ? AND symbol = ?;", int(shares_updated),
                   session["user_id"], symbol)
    else:
        db.execute("DELETE FROM purchanse WHERE id_username = ? AND symbol = ?;", session["user_id"], symbol)

    # Updating history database
    db.execute("INSERT INTO history(symbol, shares, price, id_username) VALUES (?, ?, ?, ?);",
               symbol, -abs(int(shares)), get_lookup['price'], session["user_id"])

    return redirect("/")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "GET":
        return render_template("password_change.html")
    password = request.form.get("password")
    new_password = request.form.get("new-password")
    new_password_again = request.form.get("confirmation")
    password_in_db = db.execute("SELECT hash FROM users WHERE id = ?;", session["user_id"])
    print(generate_password_hash(password), "\n", password_in_db[0]['hash'])

    if (len(password_in_db) != 1 or not password or new_password != new_password_again or not
            check_password_hash(password_in_db[0]["hash"], request.form.get("password"))):
        return apology("Data not match", 403)

    db.execute("UPDATE users SET hash = ? WHERE id = ?;", generate_password_hash(new_password), session["user_id"])

    session.clear()

    return redirect("/")