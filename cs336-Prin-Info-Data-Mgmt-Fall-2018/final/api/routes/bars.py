from routes.route import Route
from resources.bar import Bar


class Bars(Route):
    def GET(self, resource_id: str = None) -> str:

        if resource_id is None or resource_id == "":

            return self.write({"bars": Bar().get_bars()})

        return resource_id
