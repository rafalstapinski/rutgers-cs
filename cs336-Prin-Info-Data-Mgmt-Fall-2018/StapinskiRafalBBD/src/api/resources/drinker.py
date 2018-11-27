from resources.resource import Resource
from typing import List
from models.drinker import Drinker as Model


class Drinker(Resource):
    def get_all(self) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM drinkers")

        drinkers = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(drinker) for drinker in drinkers]

    def get_one(self, drinker_id: int) -> Model:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM drinkers WHERE id = %s", params=(drinker_id,))

        drinker = cursor.fetchone()

        cursor.close()
        connection.close()

        if drinker is None:
            return drinker

        return Model(drinker)

    def get_likers_of(self, product_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT * FROM drinkers WHERE id IN
                (SELECT drinker_id FROM likes WHERE product_id = %s)""",
            params=(product_id,),
        )

        drinkers = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(drinker) for drinker in drinkers]

    def get_patrons_of(self, bar_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT * FROM drinkers WHERE id IN
                (SELECT drinker_id FROM frequents WHERE drinker_id = %s)""",
            params=(bar_id,),
        )

        drinkers = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(drinker) for drinker in drinkers]
