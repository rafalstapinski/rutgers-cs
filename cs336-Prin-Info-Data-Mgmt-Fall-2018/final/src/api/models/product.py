class Product:

    _id: int
    name: str
    price: float
    is_beer: bool

    def __init__(self, row: tuple):

        self._id = row[0]
        self.name = row[1]
        self.price = row[2]
        self.is_beer = bool(row[3])
