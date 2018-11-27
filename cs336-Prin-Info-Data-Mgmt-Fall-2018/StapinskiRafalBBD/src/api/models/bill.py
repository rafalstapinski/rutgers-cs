from typing import List
from models.transaction import Transaction


class Bill:
    class Billed:
        price: float
        product_name: str
        is_beer: bool

        def __init__(self, product: tuple):
            self.price = product[0]
            self.product_name = product[1]
            self.is_beer = bool(product[2])

    transaction: Transaction
    tip: float
    billed = List[Billed]

    def __init__(self, transaction: Transaction, billed: List[tuple]):

        self.transaction = Transaction(transaction)
        self.billed = []

        for item in billed:
            self.billed.append(self.Billed(item))

        item_total = sum([b.price for b in self.billed])
        self.tip = round(self.transaction.total - (item_total * 1.07), 2)
