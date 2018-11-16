from resources.resource import Resource
from typing import List


class Beer(Resource):
    def get_all(self) -> List[tuple]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM beers;")

        beers = cursor.fetchall()

        cursor.close()
        connection.close()

        return beers

    def get_one(self, beer_id: int) -> tuple:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM beers WHERE id = %s", params=(beer_id))

        beer = cursor.fetchone()

        cursor.close()
        connection.close()

        return beer
