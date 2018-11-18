from routes.route import Route
from resources.bill import Bill


class Bills(Route):
    def GET(self, transaction_id: str = None) -> str:

        if transaction_id is None or transaction_id == "":
            self.Status.Unauthorized()
            return

        else:

            transaction = Bill().get_bill(transaction_id)

            if transaction is None:
                self.Status.Unauthorized()
                return

            transaction_serializable = {
                "transaction": transaction.transaction.__dict__,
                "billed": [item.__dict__ for item in transaction.billed],
            }

            return self.write({"bill": transaction_serializable})
