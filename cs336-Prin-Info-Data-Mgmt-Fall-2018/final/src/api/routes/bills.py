from routes.route import Route
from resources.bill import Bill


class Bills(Route):
    def GET(self, transaction_id: str = None) -> str:

        if transaction_id is None or transaction_id == "":
            self.Status.Unauthorized()
            return

        else:

            bill = Bill().get_bill(transaction_id)

            if bill is None:
                self.Status.Unauthorized()
                return

            bill_serializable = {
                "transaction": bill.transaction.__dict__,
                "billed": [item.__dict__ for item in bill.billed],
                "tip": bill.tip,
            }

            return self.write({"bill": bill_serializable})
