from typings import List
from models.product import Product


class TransactionBilled:
    class Billed:
        product_id: int
        product_name: str
        price: float

        def __init__(self, product: Product):
            self.product_id = product._id
            self.product_name = product.name
            self.price = product.price

    _id: int
    drinker_id: int
    bar_id: int
    total: float
    transaction_time: int
    billed: List[Billed]

    def __init__(
        self,
        _id: int,
        drinker_id: int,
        bar_id: int,
        total: float,
        transaction_time: int,
        products: List[Product],
    ):

        self._id = _id
        self.drinker_id = drinker_id
        self.bar_id = bar_id
        self.total = total
        self.transaction_time = transaction_time
        self.billed = []

        for product in products:
            self.billed.append(self.Billed(product=product))

