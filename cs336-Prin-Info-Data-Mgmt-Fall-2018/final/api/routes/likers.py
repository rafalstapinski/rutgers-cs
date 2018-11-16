from routes.route import Route
from resources.drinker import Drinker


class Likers(Route):
    def GET(self, product_id: str = None) -> str:

        if product_id is None or product_id == "":
            self.Status.Unauthorized()
            return

        drinkers = Drinker().get_likers_of(product_id)
        drinkers_serializable = [drinker.__dict__ for drinker in drinkers]

        return self.write({"likers": drinkers_serializable})
