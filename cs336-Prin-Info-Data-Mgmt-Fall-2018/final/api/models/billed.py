class Billed:

    transaction_id: int
    bar_id: int
    drinker_id: int
    product_id: int
    price: float
    bar_name: str
    drinker_name: str
    product_name: str

    def __init__(self, row: tuple):
        self.transaction_id = row[0]
        self.bar_id = row[1]
        self.drinker_id = row[2]
        self.product_id = row[3]
        self.price = row[4]
        self.bar_name = row[5]
        self.drinker_name = row[6]
        self.product_name = row[7]
