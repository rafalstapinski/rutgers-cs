from routes.route import Route
from resources.verify import Verify as VerifyResource


class Verify(Route):
    def GET(self) -> str:

        return self.write({"verification": VerifyResource().all()})

