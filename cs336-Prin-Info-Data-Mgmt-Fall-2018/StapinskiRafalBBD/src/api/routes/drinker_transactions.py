from routes.route import Route
from resources.transaction import Transaction


class DrinkerTransactions(Route):
    def GET(self, drinker_id: str = None) -> str:

        if drinker_id is None or drinker_id == "":
            self.Status.Unauthorized()
            return

        else:

            transactions = Transaction().get_transactions_of(drinker_id)
            transactions_serializable = [transaction.__dict__ for transaction in transactions]

            return self.write({"transactions": transactions_serializable})
