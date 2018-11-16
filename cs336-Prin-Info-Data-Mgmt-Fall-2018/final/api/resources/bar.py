from resources.resource import Resource
from typing import List
from models.bar import Bar as BarObj


class Bar(Resource):
    def get_all(self) -> List[BarObj]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM bars")

        bars = cursor.fetchall()

        cursor.close()
        connection.close()

        return [BarObj(bar) for bar in bars]

    def get_one(self, bar_id: int) -> BarObj:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM bars WHERE id = %s", params=(bar_id,))

        bar = cursor.fetchone()

        cursor.close()
        connection.close()

        if bar is None:
            return bar

        return BarObj(bar)
