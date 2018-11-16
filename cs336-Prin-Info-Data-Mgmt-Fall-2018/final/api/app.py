import web
from routes.bars import Bars
from routes.products import Products
from routes.drinkers import Drinkers
from routes.likers import Likers
from routes.likes import Likes
from routes.frequents import Frequents
from routes.patrons import Patrons
from routes.sellers import Sellers
from routes.sells import Sells

urls_styled = (
    ("/bars/(.+)/sells", Sells),
    ("/bars/(.+)/patrons", Patrons),
    ("/products/(.+)/sellers", Sellers),
    ("/products/(.+)/likers", Likers),
    ("/drinkers/(.+)/frequents", Frequents),
    ("/drinkers/(.+)/likes", Likes),
    ("/bars[/]?(.*)", Bars),
    ("/products[/]?(.*)", Products),
    ("/drinkers[/]?(.*)", Drinkers),
)

urls = [v for sl in urls_styled for v in sl]


if __name__ == "__main__":
    app = web.application(urls, locals())
    app.run()
