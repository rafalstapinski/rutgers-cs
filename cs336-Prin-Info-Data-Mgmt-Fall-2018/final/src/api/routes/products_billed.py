from routes.route import Route
from resources.billed import Billed


class ProductsBilled(Route):
    def GET(self, product_id: str = None) -> str:

        if product_id is None or product_id == "":
            self.Status.Unauthorized()
            return

        else:

            billed = Billed().get_billed_of(product_id)
            billed_serializable = [item.__dict__ for item in billed]

            return self.write({"billed": billed_serializable})
