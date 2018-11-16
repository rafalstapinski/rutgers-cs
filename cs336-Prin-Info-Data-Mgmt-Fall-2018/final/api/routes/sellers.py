from routes.route import Route
from resources.bar import Bar


class Sellers(Route):
    def GET(self, product_id: str = None) -> str:

        if product_id is None or product_id == "":
            self.Status.Unauthorized()
            return

        bars = Bar().get_sellers_of(product_id)
        bars_serializable = [bar.__dict__ for bar in bars]

        return self.write({"sellers": bars_serializable})
