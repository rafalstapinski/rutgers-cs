from resources.resource import Resource
from typing import List
from models.billed import Billed as Model


class Billed(Resource):
    def get_billed_by(self, drinker_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT
            b.transaction_id, b.bar_id, b.drinker_id, b.product_id, b.price, d.name, p.name, br.name
            FROM billed b
            INNER JOIN drinkers d ON b.drinker_id = d.id
            INNER JOIN products p ON b.product_id = p.id
            INNER JOIN bars br ON b.bar_id = br.id
            WHERE b.drinker_id = %s""",
            params=(drinker_id,),
        )

        billed = cursor.fetchall()

        return [Model(row) for row in billed]
