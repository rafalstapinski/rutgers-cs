import web
import sys
import json


class Route:

    def __init__(self):

        sys.path.append('../resources')

        web.header("Content-Type", "application/json")
        web.header("Access-Control-Allow-Origin", "*")

    @staticmethod
    def write(payload: dict) -> str:
        '''function write

        Args:
            payload (dict): the payload to be returned by a route
        Returns:
            str: JSON stringified payload
        '''
        return json.dumps(payload)

    @staticmethod
    def status(status: int):
        '''function write

        Args:
            status (int): the status to be processed by webpy
        '''
        codes = {
            200: web.OK,
            201: web.Created,
            500: web.InternalError
        }

        return codes[status]()
