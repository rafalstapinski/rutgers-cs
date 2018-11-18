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
from routes.bar_transactions import BarTransactions
from routes.drinker_transactions import DrinkerTransactions
from routes.bills import Bills
from routes.drinkers_billed import DrinkersBilled
from routes.products_billed import ProductsBilled
from routes.bars_billed import BarsBilled

urls_styled = (
    ("/bars/(.+)/transactions", BarTransactions),
    ("/bars/(.+)/sells", Sells),
    ("/bars/(.+)/patrons", Patrons),
    ("/bars/(.+)/billed", BarsBilled),
    ("/products/(.+)/sellers", Sellers),
    ("/products/(.+)/likers", Likers),
    ("/products/(.+)/billed", ProductsBilled),
    ("/drinkers/(.+)/transactions", DrinkerTransactions),
    ("/drinkers/(.+)/frequents", Frequents),
    ("/drinkers/(.+)/likes", Likes),
    ("/drinkers/(.+)/billed", DrinkersBilled),
    ("/bars[/]?(.*)", Bars),
    ("/products[/]?(.*)", Products),
    ("/drinkers[/]?(.*)", Drinkers),
    ("/bills/(.+)", Bills),
)

urls = [v for sl in urls_styled for v in sl]


if __name__ == "__main__":
    app = web.application(urls, locals())
    app.run()
