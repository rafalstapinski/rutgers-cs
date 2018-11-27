from routes.route import Route
from resources.product import Product


class Likes(Route):
    def GET(self, drinker_id: str = None) -> str:

        if drinker_id is None or drinker_id == "":
            self.Status.Unauthorized()
            return

        products = Product().get_liked_by(drinker_id)
        products_serializable = [product.__dict__ for product in products]

        return self.write({"likes": products_serializable})
