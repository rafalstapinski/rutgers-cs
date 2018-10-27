import config
import progressbar
import requests
from datetime import datetime
import json
import mysql.connector
from random import randint, choice


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

    coordinates = [(40.4019692, -74.3123014), (40.49578698007111, -74.45297241210939)]

    venues = []

    for coors in coordinates:

        print(coors)

        params = {
            "ll": "{},{}".format(coors[0], coors[1]),
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
                    r["display_name"].replace(",", ""),
                    venue["opens"],
                    venue["closes"],
                    r["address"]["state"],
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
        "INSERT INTO bars (name, address, opens, closes, state) VALUES (%s, %s, %s, %s, %s)",
        to_insert,
    )

    connection.commit()
    connection.close()


def insert_sells():

    products = {
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
    }

    venues = []

    with open("venues.json", "r") as f:
        venues = json.loads(f.read())

    connection = mysql.connector.connect(
        user=config.db_user, password=config.db_pass, host=config.db_host, database=config.db_name
    )

    cursor = connection.cursor()

    for venue in venues:

        print("SELECT * FROM bars WHERE name = '{}';".format(venue["name"]))

        # cursor.execute()
        # bar_id = cursor.fetchone()
        # print(bar_id)

    connection.close()


if __name__ == "__main__":

    # create_tables()
    # create_bars()
    insert_bars()
    # insert_sells()
