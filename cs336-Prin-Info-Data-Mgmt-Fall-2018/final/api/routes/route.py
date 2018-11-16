import web
import sys
import json


class Route:
    class Status:
        OK = web.OK
        Created = web.Created
        Unauthorized = web.Unauthorized
        InternalError = web.InternalError

    def __init__(self):

        sys.path.append("../resources")

        web.header("Content-Type", "application/json")
        web.header("Access-Control-Allow-Origin", "*")

    @staticmethod
    def write(payload: dict) -> str:
        """function write

        Args:
            payload (dict): the payload to be returned by a route
        Returns:
            str: JSON stringified payload
        """
        return json.dumps(payload)
