import psycopg2
import config
import progressbar
import requests

conn = psycopg2.connect(
    host=config.db_host, user=config.db_user, password=config.db_pass, database=config.db_name
)
cursor = conn.cursor()


def create_tables():
    """
        Creates tables based on predefined scheme
    """
    # bars table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS bars (
            id serial PRIMARY KEY,
            name varchar(30) NOT NULL,
            address text NOT NULL,
            opens SMALLINT,
            closes SMALLINT
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
            name varchar(30) NOT NULL
        );
        """
    )

    # transactions table
    cursor.execute(
        """
        CREATE TABLE IF NOT EXISTS transactions (
            id bigserial PRIMARY KEY,
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

    conn.commit()


def create_bars():
    """
        Gathers bars around central NJ from foursquare
    """

    bar_category_id = "4bf58dd8d48988d116941735"

    # venues = client.Venues.search()

    coordinates = [(40.4019692, -74.3123014), (40.49578698007111, -74.45297241210939)]

    venues = []

    for coors in coordinates:

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

                break

        break

    params = {"v": 20181027, "client_id": config.client_id, "client_secret": config.client_secret}

    for venue in venues:
        response = requests.get(
            "https://api.foursquare.com/v2/venues/{}".format(venue["id"]), params=params
        ).json()

        print(response)


if __name__ == "__main__":

    # create_tables()
    create_bars()

    conn.close()
