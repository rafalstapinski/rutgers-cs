from routes.route import Route
from resources.product import Product


class Sells(Route):
    def GET(self, bar_id: str = None) -> str:

        if bar_id is None or bar_id == "":
            self.Status.Unauthorized()
            return

        products = Product().get_sold_at(bar_id)
        products_serializable = [product.__dict__ for product in products]

        return self.write({"sells": products_serializable})
