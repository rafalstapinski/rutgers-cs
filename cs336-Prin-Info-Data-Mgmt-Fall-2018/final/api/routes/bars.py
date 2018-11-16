from routes.route import Route
from resources.bar import Bar


class Bars(Route):
    def GET(self, bar_id: str = None) -> str:

        if bar_id is None or bar_id == "":

            bars = Bar().get_all()
            bars_serializable = [bar.__dict__ for bar in bars]

            return self.write({"bars": bars_serializable})

        else:

            bar = Bar().get_one(bar_id)

            if bar is None:
                self.Status.Unauthorized()
                return

            bar_serializable = bar.__dict

            return self.write({"bar": bar_serializable})
