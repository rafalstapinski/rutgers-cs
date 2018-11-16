import web
from routes.bars import Bars
from routes.products import Products

urls = ("/bars[/]?(.*)", Bars, "/products[/]?(.*)", Products)

if __name__ == "__main__":
    app = web.application(urls, locals())
    app.run()
