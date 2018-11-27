from resources.resource import Resource
from typing import List
from models.billed import Billed as Model


class Billed(Resource):
    def get_billed_by(self, drinker_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """
            SELECT
            t.id, t.bar_id, t.drinker_id as drinker_id, p.id as product_id, bl.price,
            br.name AS bar_name, d.name AS drinker_name, p.name AS product_name
            FROM transactions t
            INNER JOIN billed bl ON bl.transaction_id = t.id
            INNER JOIN drinkers d on d.id = t.drinker_id
            INNER JOIN products p on p.id = bl.product_id
            INNER JOIN bars br on br.id = t.bar_id
            WHERE t.drinker_id = %s;
            """,
            params=(drinker_id,),
        )

        billed = cursor.fetchall()

        return [Model(row) for row in billed]

    def get_billed_of(self, product_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """
            SELECT
            t.id, t.bar_id, t.drinker_id as drinker_id, p.id as product_id, bl.price,
            br.name AS bar_name, d.name AS drinker_name, p.name AS product_name
            FROM transactions t
            INNER JOIN billed bl ON bl.transaction_id = t.id
            INNER JOIN drinkers d on d.id = t.drinker_id
            INNER JOIN products p on p.id = bl.product_id
            INNER JOIN bars br on br.id = t.bar_id
            WHERE p.id = %s
            """,
            params=(product_id,),
        )

        billed = cursor.fetchall()

        return [Model(row) for row in billed]

    def get_billed_at(self, bar_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """
            SELECT
            t.id, t.bar_id, t.drinker_id as drinker_id, p.id as product_id, bl.price,
            br.name AS bar_name, d.name AS drinker_name, p.name AS product_name
            FROM transactions t
            INNER JOIN billed bl ON bl.transaction_id = t.id
            INNER JOIN drinkers d on d.id = t.drinker_id
            INNER JOIN products p on p.id = bl.product_id
            INNER JOIN bars br on br.id = t.bar_id
            WHERE t.bar_id = %s
            """,
            params=(bar_id,),
        )

        billed = cursor.fetchall()

        return [Model(row) for row in billed]
