from routes.route import Route
from resources.transaction import Transaction


class BarTransactions(Route):
    def GET(self, bar_id: str = None) -> str:

        if bar_id is None or bar_id == "":
            self.Status.Unauthorized()
            return

        else:

            transactions = Transaction().get_transactions_at(bar_id)
            transactions_serializable = [transaction.__dict__ for transaction in transactions]

            return self.write({"transactions": transactions_serializable})
