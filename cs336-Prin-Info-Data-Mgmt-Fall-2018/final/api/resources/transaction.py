from resources.resource import Resource
from typing import List
from models.transaction import Transaction as Model


class Transaction(Resource):
    def get_transactions_at(self, bar_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT DISTINCT
            t.id, t.sum, t.time, b.drinker_id, b.bar_id, d.name as drinker_name, br.name as bar_name
            FROM transactions t
            INNER JOIN billed b ON t.id = b.transaction_id
            INNER JOIN drinkers d ON b.drinker_id = d.id
            INNER JOIN bars br on b.bar_id = br.id
            WHERE b.bar_id = %s""",
            params=(bar_id,),
        )

        transactions = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(transaction) for transaction in transactions]

    def get_transactions_of(self, drinker_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT DISTINCT
            t.id, t.sum, t.time, b.drinker_id, b.bar_id, d.name as drinker_name, br.name as bar_name
            FROM transactions t
            INNER JOIN billed b ON t.id = b.transaction_id
            INNER JOIN drinkers d ON b.drinker_id = d.id
            INNER JOIN bars br on b.bar_id = br.id
            WHERE b.drinker_id = %s""",
            params=(drinker_id,),
        )

        transactions = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(transaction) for transaction in transactions]
