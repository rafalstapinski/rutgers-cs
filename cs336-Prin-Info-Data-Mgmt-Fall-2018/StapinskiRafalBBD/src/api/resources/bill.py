from resources.resource import Resource
from models.bill import Bill as Model


class Bill(Resource):
    def get_bill(self, transaction_id: int) -> Model:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """
            SELECT DISTINCT
            t.id, t.sum, t.time, t.drinker_id, t.bar_id, d.name as drinker_name, b.name as bar_name
            FROM transactions t
            INNER JOIN drinkers d ON t.drinker_id = d.id
            INNER JOIN bars b on t.bar_id = b.id
            WHERE t.id = %s
            """,
            params=(transaction_id,),
        )

        transaction = cursor.fetchone()

        cursor.execute(
            """
            SELECT b.price, p.name, p.is_beer
            FROM billed b
            INNER JOIN products p ON b.product_id = p.id
            WHERE b.transaction_id = %s
            """,
            params=(transaction_id,),
        )

        billed = cursor.fetchall()

        cursor.close()
        connection.close()

        if transaction is None:
            return transaction

        return Model(transaction, billed)
