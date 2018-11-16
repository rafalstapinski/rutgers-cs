import web
from routes.bars import Bars

urls = ("/bars/(.*)", Bars, "/bars", Bars)

if __name__ == "__main__":
    app = web.application(urls, locals())
    app.run()
