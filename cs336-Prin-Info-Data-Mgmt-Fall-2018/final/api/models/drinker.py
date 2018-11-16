class Drinker:

    _id: int
    name: str
    address: str
    city: str
    state: str

    def __init__(self, row: tuple):

        self._id = row[0]
        self.name = row[1]
        self.address = row[2]
        self.city = row[3]
        self.state = row[4]
