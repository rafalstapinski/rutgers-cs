from resources.resource import Resource
from typing import List


class Drinker(Resource):
    def get_all(self) -> List[tuple]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM drinkers;")

        drinkers = cursor.fetchall()

        cursor.close()
        connection.close()

        return drinkers

    def get_one(self, drinker_id: int) -> tuple:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM drinkers WHERE id = %s", params=(drinker_id))

        drinker = cursor.fetchone()

        cursor.close()
        connection.close()

        return drinker
