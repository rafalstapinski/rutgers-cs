import config
import progressbar
import requests
from datetime import datetime
import json
import mysql.connector
from random import randint, sample, random
import names
import pandas as pd
import pickle
import numpy as np
import time


def create_tables():
    """
        Creates tables based on predefined scheme
    """

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    # bars table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS bars (
            id serial PRIMARY KEY,
            name varchar(100) NOT NULL UNIQUE,
            address text NOT NULL,
            opens SMALLINT,
            closes SMALLINT,
            state varchar(30) NOT NULL
        );
    """
    )

    # drinkers table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS drinkers (
            id serial PRIMARY KEY,
            name varchar(40) NOT NULL,
            address text NOT NULL
        );
    """
    )

    # products table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS products (
            id serial PRIMARY KEY,
            name varchar(30) NOT NULL,
            base_price float(2)
        );
        """
    )

    # transactions table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS transactions (
            id serial PRIMARY KEY,
            sum float(2),
            time TIMESTAMP
        );
        """
    )

    # likes table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS likes (
            drinker_id integer,
            product_id integer
        );
    """
    )

    # sells table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS sells (
            product_id integer,
            bar_id integer,
            price float(2)
        );
    """
    )

    # frequents table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS frequents (
            drinker_id integer,
            bar_id integer
        );
    """
    )

    # billed table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS billed (
            transaction_id integer,
            bar_id integer,
            drinker_id integer,
            product_id integer,
            price float(2)
        );
    """
    )

    connection.commit()
    connection.close()


def create_bars():
    """
        Gathers bars around central NJ from foursquare
    """

    bar_category_id = "4bf58dd8d48988d116941735"

    venues = []

    states = ["New Jersey", "Rhode Island", "Maine"]

    for state in states:

        params = {
            "near": state,
            "intent": "browse",
            "radius": 20000,
            "categoryId": bar_category_id,
            "limit": 50,
            "client_id": config.client_id,
            "client_secret": config.client_secret,
            "v": 20181027,
        }

        response = requests.get("https://api.foursquare.com/v2/venues/search", params=params).json()

        if "response" in response:
            for venue in response["response"]["venues"]:
                venues.append(
                    {
                        "id": venue["id"],
                        "name": venue["name"],
                        "lat": venue["location"]["lat"],
                        "lng": venue["location"]["lng"],
                    }
                )

    params = {"v": 20181027, "client_id": config.client_id, "client_secret": config.client_secret}

    print("premium calls")

    for venue in progressbar.progressbar(venues):
        response = requests.get(
            "https://api.foursquare.com/v2/venues/{}".format(venue["id"]), params=params
        ).json()

        if "response" in response:

            try:

                hours = response["response"]["venue"]["hours"]["timeframes"][-1]["open"][0][
                    "renderedTime"
                ].split("–")

                open_time = datetime.strptime(hours[0], "%I:%M %p")
                close_time = datetime.strptime(hours[1], "%I:%M %p")

                venue["opens"] = int(open_time.hour * 60 + open_time.minute)
                venue["closes"] = int(close_time.hour * 60 + close_time.minute)
                venue["price"] = response["response"]["venue"]["price"]["tier"]
            except Exception:
                continue

    print("writing to file")
    with open("venues.json", "w") as f:
        f.write(json.dumps(venues))


def insert_bars():

    venues = []

    with open("venues.json", "r") as f:
        venues = json.loads(f.read())

    to_insert = []
    processed = []

    for venue in progressbar.progressbar(venues):

        if venue["name"] in processed:
            continue

        params = {
            "format": "json",
            "lat": venue["lat"],
            "lon": venue["lng"],
            "zoom": 18,
            "addressdetails": 1,
        }

        r = requests.get("https://nominatim.openstreetmap.org/reverse", params=params).json()

        try:
            to_insert.append(
                (
                    venue["name"],
                    r["address"]["road"],
                    r["address"]["city"],
                    r["address"]["state"],
                    venue["opens"],
                    venue["closes"],
                )
            )

            processed.append(venue["name"])

        except KeyError:
            continue

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    cursor.executemany(
        "INSERT INTO bars (name, address, city, state, opens, closes) VALUES (%s, %s, %s, %s, %s, %s)",
        to_insert,
    )

    connection.commit()
    connection.close()


def insert_products():

    products = [
        ("Coors Light", 2, True),
        ("Budweiser", 3, True),
        ("Stella Artois", 4, True),
        ("Stone IPA", 4, True),
        ("Angry Orchards", 4, True),
        ("Dos Equis", 3, True),
        ("Miller Lite", 2, True),
        ("Blue Moon", 4, True),
        ("Modelo Especial", 5, True),
        ("Rodenbach Grand Cru", 6, True),
        ("Kolsch", 6, True),
        ("Tyskie", 5, True),
        ("Lech", 5, True),
        ("Nachos", 8, False),
        ("Burger", 11, False),
        ("Fries", 5, False),
    ]

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    cursor.executemany("INSERT INTO products (name, price, is_beer) VALUES (%s, %s, %s)", products)

    connection.commit()
    connection.close()


def insert_sells():

    venues = []

    with open("venues.json", "r") as f:
        venues = json.loads(f.read())

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    cursor.execute("SELECT * FROM products")

    products = cursor.fetchall()

    for venue in venues:

        cursor.execute("SELECT * FROM bars WHERE name = %s", (venue["name"],))
        bar = cursor.fetchone()

        if bar is None:
            continue

        # for each venue, choose a random number of products and insert + price premium

        to_sell = sample(products, randint(7, 14))

        price_premium = 0 if "price" not in venue else 0.25 * venue["price"]

        this_sells = [(product[0], bar[0], product[2] + price_premium) for product in to_sell]

        cursor.executemany(
            "INSERT INTO sells (product_id, bar_id, price) VALUES (%s, %s, %s)", this_sells
        )

    connection.commit()
    connection.close()


def insert_drinkers():

    drinkers = []

    sample_size = 250

    states = [
        "./openaddr-short/me.csv",
        "./openaddr-short/nj.csv",
        "./openaddr-short/ri.csv",
        "./openaddr-short/ct.csv",
    ]

    dfs = []

    for state in states:
        df = pd.read_csv(state)
        sample = df.sample(n=sample_size)
        dfs.append(sample)

    houses = pd.concat(dfs)

    for i, row in progressbar.progressbar(houses.iterrows(), max_value=sample_size * len(states)):

        params = {
            "format": "json",
            "lat": row["LAT"],
            "lon": row["LON"],
            "zoom": 18,
            "addressdetails": 1,
        }

        r = requests.get("https://nominatim.openstreetmap.org/reverse", params=params).json()

        try:
            drinkers.append(
                (
                    names.get_full_name(),
                    "{} {}".format(r["address"]["house_number"], r["address"]["road"]),
                    r["address"]["city"],
                    r["address"]["state"],
                )
            )
        except KeyError:
            continue

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )
    cursor = connection.cursor()

    cursor.executemany(
        "INSERT INTO drinkers (name, address, city, state) VALUES (%s, %s, %s, %s)", drinkers
    )

    connection.commit()
    connection.close()


def insert_likes():

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    cursor.execute("SELECT * FROM drinkers;")

    drinkers = cursor.fetchall()

    cursor.execute("SELECT * FROM products;")

    products = cursor.fetchall()

    to_insert = []

    for drinker in drinkers:

        drinker_likes = sample(products, randint(3, 7))

        to_insert += [(drinker[0], like[0]) for like in drinker_likes]

    cursor.executemany("INSERT INTO likes (drinker_id, product_id) VALUES (%s, %s)", to_insert)

    connection.commit()
    connection.close()


def insert_frequents():

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    cursor.execute("SELECT * FROM drinkers")
    drinkers = cursor.fetchall()

    to_insert = []

    for drinker in progressbar.progressbar(drinkers):

        # get 1 - 3 bars that sell something this person likes

        query = """
            SELECT * FROM bars b WHERE b.id IN 
            (
                SELECT bar_id FROM sells s WHERE s.product_id IN 
                (
                    SELECT product_id FROM likes l WHERE l.drinker_id = {}
                )
            ) AND b.state = '{}'
        
        """.format(
            drinker[0], drinker[4]
        )

        cursor.execute(query)
        possibilities = cursor.fetchall()

        if len(possibilities) == 0:
            continue

        frequents = sample(possibilities, randint(1, 3))

        to_insert += [(drinker[0], bar[0]) for bar in frequents]

    cursor.executemany("INSERT INTO frequents (drinker_id, bar_id) VALUES (%s, %s)", to_insert)

    connection.commit()
    connection.close()


def str_time_prop(start, end, fmt, prop):
    """Get a time at a proportion of a range of two formatted times.

    start and end should be strings specifying times formated in the
    given format (strftime-style), giving an interval [start, end].
    prop specifies how a proportion of the interval to be taken after
    start.  The returned time will be in the specified format.
    """

    stime = time.mktime(time.strptime(start, fmt))
    etime = time.mktime(time.strptime(end, fmt))

    ptime = stime + prop * (etime - stime)

    return time.strftime(fmt, time.localtime(ptime))


def random_date(start, end, prop):
    return str_time_prop(start, end, "%m/%d/%Y %I:%M %p", prop)


def random_dt():
    ts = random_date("1/1/2018 8:00 AM", "10/27/2018 11:59 PM", random())
    return datetime.strptime(ts, "%m/%d/%Y %I:%M %p")


def insert_billed_transactions():

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    cursor.execute("SELECT * FROM frequents")
    frequents = cursor.fetchall()

    billed = []

    for f in progressbar.progressbar(frequents):

        cursor.execute("SELECT * FROM drinkers WHERE id = {}".format(f[0]))
        drinker = cursor.fetchone()

        cursor.execute("SELECT * FROM bars WHERE id = {}".format(f[1]))
        bar = cursor.fetchone()

        cursor.execute("SELECT * FROM sells WHERE bar_id = {}".format(bar[0]))
        sells = cursor.fetchall()

        sells_keyed = {s[0]: s for s in sells}

        for t in range(randint(10, 20)):
            bought = np.random.choice(list(sells_keyed.keys()), randint(2, 12), replace=True)

            # returns numpy int64 list so convert back to standard python int
            bought = [int(i) for i in bought]

            transaction_sum = round(
                sum([sells_keyed[item][2] for item in bought])
                * 1.07
                * (1 + (randint(0, 20) / 100)),
                2,
            )

            dt = random_dt()

            # if open until after midnight
            # bar[5] = opens
            # bar[6] = closes
            if bar[6] < bar[5]:
                # good if minutes since midnight < closes or > opens

                while not (dt.hour * 60 + dt.minute < bar[6] or dt.hour * 60 + dt.minute > bar[5]):
                    dt = random_dt()
            else:
                # good if minutes since midnight > opens and < closes
                while not (dt.hour * 60 + dt.minute > bar[5] and dt.hour * 60 + dt.minute < bar[6]):
                    dt = random_dt()

            cursor.execute(
                "INSERT INTO transactions (sum, time) VALUES (%s, %s)", (transaction_sum, dt)
            )
            transaction_id = cursor.lastrowid

            billed += [
                (transaction_id, bar[0], drinker[0], product_id, sells_keyed[product_id][2])
                for product_id in bought
            ]

    cursor.executemany(
        "INSERT INTO billed (transaction_id, bar_id, drinker_id, product_id, price) VALUES (%s,%s, %s, %s, %s)",
        billed,
    )

    connection.commit()
    connection.close()


if __name__ == "__main__":

    # create_tables()
    # create_bars()
    # insert_bars()
    # insert_drinkers()
    # insert_products()
    # insert_sells()
    # insert_likes()
    # insert_frequents()
    insert_billed_transactions()

