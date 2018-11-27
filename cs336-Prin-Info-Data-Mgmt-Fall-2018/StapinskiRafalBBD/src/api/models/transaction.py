class Transaction:
    _id: int
    total: float
    timestamp: str
    drinker_id: int
    bar_id: int
    drinker_name: str
    bar_name: str

    def __init__(self, row: tuple):
        self._id = row[0]
        self.total = row[1]
        self.timestamp = row[2].strftime("%Y-%m-%d %H:%M:%S")
        self.drinker_id = row[3]
        self.bar_id = row[4]
        self.drinker_name = row[5]
        self.bar_name = row[6]
