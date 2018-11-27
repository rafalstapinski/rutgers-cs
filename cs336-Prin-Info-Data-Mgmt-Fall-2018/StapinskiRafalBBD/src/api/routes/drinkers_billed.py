from routes.route import Route
from resources.billed import Billed


class DrinkersBilled(Route):
    def GET(self, drinker_id: str = None) -> str:

        if drinker_id is None or drinker_id == "":
            self.Status.Unauthorized()
            return

        else:

            billed = Billed().get_billed_by(drinker_id)
            billed_serializable = [item.__dict__ for item in billed]

            return self.write({"billed": billed_serializable})
