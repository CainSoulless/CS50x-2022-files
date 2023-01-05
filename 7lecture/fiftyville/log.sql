-- Keep a log of any SQL queries you execute as you solve the mystery.
-- I start with crime_scene_reports
SELECT * FROM crime_scene_reports;

.tables
/*
airports              crime_scene_reports   people
atm_transactions      flights               phone_calls
bakery_security_logs  interviews
bank_accounts         passengers
*/

-- Print description of crimes committed on July 28 - 10:15am
SELECT * FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND description LIKE "%CS50%";
/*
| 295 | 2021 | 7     | 28  | Humphrey Street |
Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
Interviews were conducted today with three witnesses who were present at the time
– each of their interview transcripts mentions the bakery. |

SELF NOTES: interview are a table that have transcript column.
*/
.schema interviews
/*    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
*/
SELECT * FROM interviews WHERE transcript LIKE "%bakery%" AND day = 28;
/*| 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
SELF NOTES: -The theft was a car in the bakery parking.
            -The theft made a withdrawing on the same day (Leggett Street).
            -The theft took the erliest flight the July 29.
*/

-- Transaction
.schema atm_transactions
/*
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
*/
-- Getting list of all people whose made a withdraw in this day.
SELECT people.name, atm_transactions.account_number, atm_transactions.amount FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.day = 28
AND atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw" ;
/*
|  name   | account_number | amount | day |
+---------+----------------+--------+-----+
| Bruce   | 49610011       | 50     | 28  |
| Diana   | 26013199       | 35     | 28  |
| Brooke  | 16153065       | 80     | 28  |
| Kenny   | 28296815       | 20     | 28  |
| Iman    | 25506511       | 20     | 28  |
| Luca    | 28500762       | 48     | 28  |
| Taylor  | 76054385       | 60     | 28  |
| Benista | 81061156       | 30     | 28  |
*/

-- Getting suspicious names
SELECT name,passport_number,license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.day = 28
AND atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw";
/*
|  name   | passport_number | license_plate |
+---------+-----------------+---------------+
| Bruce   | 5773159633      | 94KL13X       | - - -
| Diana   | 3592750733      | 322W7JE       | - -
| Iman    | 7049073643      | L93JTIZ       | -
| Luca    | 8496433585      | 4328GD8       | - - -
| Taylor  | 1988161715      | 1106N58       | - - -
*/

-- 2 Earliest flight
-- Identifying the airport
SELECT * FROM airports WHERE city = "Fiftyville";
/*
| id | abbreviation |          full_name          |    city    |
+----+--------------+-----------------------------+------------+
| 8  | CSF          | Fiftyville Regional Airport | Fiftyville|
*/
-- Getting firsts flights on July 29;
SELECT * FROM flights WHERE day = 29 AND month = 7 ORDER BY hour ASC;
/*
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 43 | 8                 | 1                      | 2021 | 7     | 29  | 9    | 30     |
| 23 | 8                 | 11                     | 2021 | 7     | 29  | 12   | 15     |
| 53 | 8                 | 9                      | 2021 | 7     | 29  | 15   | 20     |
| 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      |
*/

-- Getting destination:
SELECT * FROM airports WHERE id = 4;
/*
+----+--------------+-------------------+---------------+
| id | abbreviation |     full_name     |     city      |
+----+--------------+-------------------+---------------+
| 4  | LGA          | LaGuardia Airport | New York City |
*/

-- List of suspicious people.
SELECT passengers.passport_number, seat, license_plate, people.name, people.phone_number
FROM passengers JOIN people ON people.passport_number = passengers.passport_number
WHERE flight_id = 36;
/*
passport_number | seat | license_plate |  name  |  phone_number  |
+-----------------+------+---------------+--------+----------------+
| 7214083635      | 2A   | M51FA04       | Doris  | (066) 555-9701 |
| 1695452385      | 3B   | G412CB7       | Sofia  | (130) 555-0289 |
| 5773159633      | 4A   | 94KL13X       | Bruce  | (367) 555-5533 |
| 1540955065      | 5C   | 130LD9Z       | Edward | (328) 555-1152 |
| 8294398571      | 6C   | 0NTHK55       | Kelsey | (499) 555-9472 |
| 1988161715      | 6D   | 1106N58       | Taylor | (286) 555-6063 |
| 9878712108      | 7A   | 30G67EN       | Kenny  | (826) 555-1652 |
| 8496433585      | 7B   | 4328GD8       | Luca   | (389) 555-5198 |
*/

-- 3 Bakery parking
.schema bakery_security_logs
/*
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
*/

-- Name of person who made a transaction and was parked in the bakery parking lot on Leggett Street, July 28 and made a withdraw on the same day but before 13 pm:
SELECT people.name, atm_transactions.account_number, bakery_security_logs.license_plate FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE atm_transactions.day = 28
AND atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw"
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour < 13;
/*
|  name  | account_number | license_plate |
+--------+----------------+---------------+
| Bruce  | 49610011       | 94KL13X       |
| Bruce  | 49610011       | 94KL13X       |
| Diana  | 26013199       | 322W7JE       |
| Diana  | 26013199       | 322W7JE       |
| Iman   | 25506511       | L93JTIZ       |
| Iman   | 25506511       | L93JTIZ       |
| Luca   | 28500762       | 4328GD8       |
| Luca   | 28500762       | 4328GD8       |
| Taylor | 76054385       | 1106N58       |
| Taylor | 76054385       | 1106N58       |
*/

/* ===================================
THIEF
*/
SELECT * FROM people WHERE name = "Bruce";
/*
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
*/

-- Call maded by Bruno the July 28
SELECT * FROM phone_calls WHERE day = 28 AND duration < 61 AND caller = (SELECT phone_number FROM people WHERE name = "Bruce");
/*
| id  |     caller     |    receiver    | year | month | day | duration |
+-----+----------------+----------------+------+-------+-----+----------+
| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
*/
-- Getting the accomplisher
SELECT * FROM people WHERE phone_number = "(375) 555-8161";
/*
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
*/