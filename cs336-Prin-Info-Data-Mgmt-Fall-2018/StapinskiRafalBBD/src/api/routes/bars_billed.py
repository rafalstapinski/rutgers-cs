from routes.route import Route
from resources.billed import Billed


class BarsBilled(Route):
    def GET(self, bar_id: str = None) -> str:

        if bar_id is None or bar_id == "":
            self.Status.Unauthorized()
            return

        else:

            billed = Billed().get_billed_at(bar_id)
            billed_serializable = [item.__dict__ for item in billed]

            return self.write({"billed": billed_serializable})
