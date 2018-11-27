from routes.route import Route
from resources.product import Product


class Products(Route):
    def GET(self, product_id: str = None) -> str:

        if product_id is None or product_id == "":

            products = Product().get_all()
            products_serializable = [product.__dict__ for product in products]

            return self.write({"products": products_serializable})

        else:

            product = Product().get_one(product_id)

            if product is None:
                self.Status.Unauthorized()
                return

            product_serializable = product.__dict__
            return self.write({"product": product_serializable})
