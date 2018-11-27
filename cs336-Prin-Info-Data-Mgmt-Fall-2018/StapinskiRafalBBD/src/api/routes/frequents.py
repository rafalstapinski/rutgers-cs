from routes.route import Route
from resources.bar import Bar


class Frequents(Route):
    def GET(self, drinker_id: str = None) -> str:

        if drinker_id is None or drinker_id == "":
            self.Status.Unauthorized()
            return

        bars = Bar().get_frequents_of(drinker_id)
        bars_serializable = [bar.__dict__ for bar in bars]

        return self.write({"frequents": bars_serializable})
