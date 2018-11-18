from routes.route import Route
from resources.drinker import Drinker


class Drinkers(Route):
    def GET(self, drinker_id: str = None) -> str:

        if drinker_id is None or drinker_id == "":

            drinkers = Drinker().get_all()
            drinkers_serializable = [drinker.__dict__ for drinker in drinkers]

            return self.write({"drinkers": drinkers_serializable})

        else:

            drinker = Drinker().get_one(drinker_id)

            if drinker is None:
                self.Status.Unauthorized()
                return

            drinker_serializable = drinker.__dict__

            return self.write({"drinker": drinker_serializable})
