from resources.resource import Resource
from typing import List


class Bar(Resource):
    def get_all(self) -> List[tuple]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM bars;")

        bars = cursor.fetchall()

        cursor.close()
        connection.close()

        return bars

    def get_one(self, bar_id: int) -> tuple:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM bars WHERE id = %s", params=(bar_id))

        bar = cursor.fetchone()

        cursor.close()
        connection.close()

        return bar
